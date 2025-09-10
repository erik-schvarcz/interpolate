#include <iostream>
#include <interpolate.hpp>

int main()
{
	interpolate intr({ {0,0}, {1,3}, {2, 8}, {3, 5} , {4, 7}, {5, 2}, {6, 4} });

	for (size_t i = 0; i < 100; ++i) {
		double d = intr(i / 100.0);

		for (double j = 0.0f; j < d; j += 0.1) {
			std::cout << "x";
		}

		std::cout << "\n";
	}

	return EXIT_SUCCESS;
}
