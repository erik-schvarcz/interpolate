#pragma once
#include <tuple>
#include <initializer_list>

class interpolation_step
{
public:
	interpolation_step(const std::tuple<double, double>& p0, const std::tuple<double, double>& p1);
	interpolation_step(const std::tuple<double, double>& p0, const std::tuple<double, double>& p1, const std::tuple<double, double>& p2);

	double operator()(double progress) const;

private:
	std::vector<double> m_coefficients;
};

class interpolate
{
public:
	interpolate(std::vector<std::tuple<double, double>> points);
	double operator()(double progress) const;

private:
	std::vector<std::pair<double, interpolation_step>> m_steps;
};
