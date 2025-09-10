#include "interpolate.hpp"
#include <Eigen/Dense>

interpolation_step::interpolation_step(const std::tuple<double, double>& p0, const std::tuple<double, double>& p1)
{
	double x0 = std::get<0>(p0), y0 = std::get<1>(p0),
		x1 = std::get<0>(p1), y1 = std::get<1>(p1);

	Eigen::Matrix2d m;
	m <<	x0, 1,
			x1, 1;

	Eigen::Vector2d v;
	v <<	y0, y1;

	Eigen::Vector2d c = m.colPivHouseholderQr().solve(v);
	m_coefficients.reserve(2);
	m_coefficients.push_back(c(0));
	m_coefficients.push_back(c(1));
}

interpolation_step::interpolation_step(const std::tuple<double, double>& p0, const std::tuple<double, double>& p1, const std::tuple<double, double>& p2)
{
	double x0 = std::get<0>(p0), y0 = std::get<1>(p0),
		x1 = std::get<0>(p1), y1 = std::get<1>(p1),
		x2 = std::get<0>(p2), y2 = std::get<1>(p2);

	Eigen::Matrix3d m;
	m <<	x0 * x0, x0, 1,
			x1 * x1, x1, 1,
			x2 * x2, x2, 1;

	Eigen::Vector3d v;
	v <<	y0, y1, y2;

	Eigen::Vector3d c = m.colPivHouseholderQr().solve(v);

	m_coefficients.reserve(3);
	m_coefficients.push_back(c(0));
	m_coefficients.push_back(c(1));
	m_coefficients.push_back(c(2));
}

double interpolation_step::operator()(double progress) const
{
	double y = 0.0;
	for (double d : m_coefficients) {
		y = y * progress + d;
	}
	return y;
}

interpolate::interpolate(std::vector<std::tuple<double, double>> points)
{
	size_t n = points.size();
	if (n < 2) {
		assert(false && "At least 2 points has to be defined!");
		return;
	}

	double minX = std::get<0>(points.front());
	double maxX = std::get<0>(points.back());
	double span = (maxX > minX) ? (maxX - minX) : 1.0;

	for (auto& p : points) {
		double& x = std::get<0>(p);
		x = (x - minX) / span;
	}

	size_t i = 0;
	for (; i + 2 < n; i += 2) {
		const auto& p0 = points[i];
		const auto& p1 = points[i + 1];
		const auto& p2 = points[i + 2];

		interpolation_step step(p0, p1, p2);
		m_steps.push_back(std::make_pair(std::get<0>(p0), step));
	}

	if (i + 1 < n) {
		const auto& p0 = points[i];
		const auto& p1 = points[i + 1];

		interpolation_step step(p0, p1);
		m_steps.push_back(std::make_pair(std::get<0>(p0), step));
	}
}

double interpolate::operator()(double progress) const
{
	if (m_steps.empty())
		return 0.0;

	auto it = std::upper_bound(m_steps.begin(), m_steps.end(), progress, 
		[](double x, const std::pair<double, interpolation_step>& s) {
		return x < s.first;
	});

	if (it == m_steps.begin())
		return it->second(progress);

	return std::prev(it)->second(progress);
}
