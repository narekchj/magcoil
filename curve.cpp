#include "curve.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include "lagranj.hpp"

Curve::Curve(const std::string& file_path)
: m_file_path(file_path)
, m_delim(':')
{
	init_curve();
}

void Curve::init_curve()
{
	std::ifstream file;
	file.open(m_file_path);

	// first fetch the size of the file
	std::string rows_keyword, rows_count;
	std::getline(file, rows_keyword, m_delim);
	std::getline(file, rows_count, '\n');
	if (rows_keyword == "points")
	{
		m_points_X.reserve(std::stoi(rows_count) );
		m_points_Y.reserve(std::stoi(rows_count) );
	}
	
		std::string x_line, y_line;
	// parse file to fetch x and y
	while ( std::getline(file, y_line, m_delim) && std::getline(file, x_line, '\n') )
	{
		//std::cout << y_line <<":"<< x_line << std::endl;
		Point p(std::stod(x_line), std::stod(y_line));
		m_points_X.push_back(p);
		m_points_Y.push_back(p);
	}

	Less_Than_Point eq_x(&Point::get_X);
	std::sort(m_points_X.begin(), m_points_X.end(), eq_x);

	Less_Than_Point eq_y(&Point::get_Y);
	std::sort(m_points_Y.begin(), m_points_Y.end(), eq_y);
}


std::vector<double> Curve::get_all_X() const
{
	std::vector<double> x;
	x.reserve(m_points_X.size());

	for (t_points_const_it it = m_points_X.cbegin(); it != m_points_X.cend(); ++it)
		x.push_back((*it).get_X());

	return x;
}

std::vector<double> Curve::get_all_Y() const
{
	std::vector<double> y;
	y.reserve(m_points_Y.size());

	for (t_points_const_it it = m_points_Y.cbegin(); it != m_points_Y.cend(); ++it)
		y.push_back((*it).get_Y());

	return y;
}

double Curve::get_value_X(double y) const
{
	// first try to find value in existing values
	t_points_const_it it = find_curve_point(m_points_Y.begin(), m_points_Y.end(), Point(0, y), &Point::get_Y );

	if (it != m_points_Y.end())
		return (*it).get_X(); // we found in the values we have

	if (m_points_Y.size() < 2)
		return 0; // we don't have points to get range

	std::vector<Point> vec;
	std::pair<t_points_const_it, t_points_const_it> main_range = get_point_range(m_points_Y, Point(0, y), &Point::get_Y);


	// calculate left sub range
	size_t cnt = 1;
	t_points_const_it left_range_bnd = main_range.first;
	if (main_range.first != m_points_Y.begin())
	{
		left_range_bnd = main_range.first - 1;
		vec.push_back(*left_range_bnd);
		++cnt;
	}

	vec.push_back(*main_range.first);
	vec.push_back(*main_range.second);

	// calculate right sub range
	t_points_const_it right_range_bnd = main_range.second;
	if ((main_range.second + 1) != m_points_Y.end())
	{
		right_range_bnd = main_range.second + 1;
		vec.push_back(*right_range_bnd);
		++cnt;
	}

//	double left_value = f_x(*left_range_bnd, *main_range.first, y);
//
//	double main_value = f_x(*main_range.first, *main_range.second, y); // the main value
//
//	double right_value = f_x(*main_range.second, *right_range_bnd, y);
//
//	std::cout << left_value << "+ " << main_value << "+ " << right_value << " / " << cnt << std::endl;
// // TODO
//    return (left_value + main_value + right_value) / cnt; // TODO! change to the correct formula

	LagApprop<Point> lag;

	return lag.getY(y, vec);
}

double Curve::f_x(Point p1, Point p2, double y) const
{
	if ((p1.get_X() - p2.get_X()) == 0)
	{
	//	std::cout << "Error 0\n";
		return 0;
	}

	const double slope = (p1.get_Y() - p2.get_Y()) / (p1.get_X() - p2.get_X());

	if (slope == 0)
		return -1;

	// now calculate x
	return (y - p1.get_Y()) / slope + p1.get_X(); 
}

std::pair<Curve::t_points_const_it, Curve::t_points_const_it> Curve::get_point_range(const t_curve_points& v, Point pt, fun f) const
{
	// now find two range points
	Less_Than_Point less_Y(f);
	std::pair<t_points_const_it, t_points_const_it> range = std::equal_range(v.begin(), v.end(), pt, less_Y);

	if (range.second == v.end())
	{
		--range.second;
		range.first -= 2;
	}
	else if (range.second == v.begin())
	{
		++range.second;
	}
	else if (range.first != v.begin())
	{
		--range.first;
	}

	return range;
}

//// TODO! move to curve and implement
//double Curve_BH::get_value_Y(double x) const
//{
//	return 0;
//}

Curve_BH::Curve_BH(const std::string& file_path)
: Curve(file_path)
{}


#if 0
int main()
{
	//Curve_BH cr(L"D:/PhD/sources/steel_bh/Steel_2211_2312.txt");
	Curve_BH cr(L"D:/PhD/sources/steel_bh/Steel_3.txt");
	//std::vector<double> x_vec = cr.get_all_X();
	//std::cout <<"size of x"<< x_vec.size() <<std::endl; 
	//std::vector<double> y_vec = cr.get_all_Y();
	//std::cout << "size of y" << y_vec.size() << std::endl;

	std::cout << cr.get_H_from_B(0.1) << std::endl;
	std::cout << cr.get_H_from_B(0.2) << std::endl;
	std::cout << cr.get_H_from_B(0.3) << std::endl;
	std::cout << cr.get_H_from_B(0.4) << std::endl;
	std::cout << cr.get_H_from_B(0.43) << std::endl;
	std::cout << cr.get_H_from_B(0.4378915) << std::endl;
	std::cout << cr.get_H_from_B(0.5) << std::endl;
	std::cout << cr.get_H_from_B(0.6) << std::endl;
	std::cout << cr.get_H_from_B(2.7) << std::endl;
	std::cout << cr.get_H_from_B(2.75) << std::endl;
	std::cout << cr.get_H_from_B(2.8) << std::endl;
	//std::cout << y << std::endl;

	std::cin.get();

	return 0; 
}
#endif
