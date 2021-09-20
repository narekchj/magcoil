#ifndef __LAGRANJ_APP__
#define __LAGRANJ_APP__

#include <vector>

template <typename T>
class LagApprop
{

public:
	double getX(double x, const std::vector<T>& points) const
	{
		size_t num = points.size();
		double result_x = 0;

		// count sum 
		for (size_t i = 0; i < num; ++i)
		{
			// count mult
			double P1 = 1;
			for (size_t j = 0; i && j <= (i - 1); ++j)
				P1 *= x - points[j].get_X();

			double P2 = 1;
			for (size_t j = i + 1; j < num; ++j)
				P2 *= x - points[j].get_X();

			double P3 = 1;
			for (size_t j = 0; i && j <= (i - 1); ++j)
				P3 *= points[i].get_X() - points[j].get_X();

			double P4 = 1;
			for (size_t j = i + 1; j < num; ++j)
				P4 *= points[i].get_X() - points[j].get_X();

			double div = (P3*P4);
			div = div ? div : 1;

			double val = (P1*P2) / div * points[i].get_Y();

			result_x += val;
		}

		return result_x;
	};

	double getY(double y, const std::vector<T>& points) const
	{
		size_t num = points.size();
		double result_y = 0;

		// count sum 
		for (size_t i = 0; i < num; ++i)
		{
			// count mult
			double P1 = 1;
			for (size_t j = 0; i && j <= (i - 1); ++j)
				P1 *= y - points[j].get_Y();

			double P2 = 1;
			for (size_t j = i + 1; j < num; ++j)
				P2 *= y - points[j].get_Y();

			double P3 = 1;
			for (size_t j = 0; i && j <= (i - 1); ++j)
				P3 *= points[i].get_Y() - points[j].get_Y();

			double P4 = 1;
			for (size_t j = i + 1; j < num; ++j)
				P4 *= points[i].get_Y() - points[j].get_Y();

			double div = (P3*P4);
			div = div ? div : 1;

			double val = (P1*P2) / div * points[i].get_X();

			result_y += val;
		}

		return result_y;

	};
};

#endif // __LAGRANJ_APP__