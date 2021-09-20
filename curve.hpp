/**
* 
* This class describe the curve interface. 
* The curve points are passed through data file.
*
*/

#ifndef _POINT_H_
#define _POINT_H_

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>

// TODO!!! enhance Point interface to make template
class Point
{
public:
	Point() : m_x(0.0), m_y(0.0)
	{}
	Point(double x, double y)
		: m_x(x), m_y(y)
	{}

	Point(const Point& p1)
	{
		m_x = p1.m_x;
		m_y = p1.m_y;
	}

	Point& operator=(const Point& p1)
	{
		if (this != &p1)
		{
			m_x = p1.m_x;
			m_y = p1.m_y;
		}

		return *this;
	}

    bool operator<(const Point&) const 
    {
        return true;
    }

public:
	void set_X(double x) { m_x = x; }
	void set_Y(double y) { m_y = y; }

	double get_X() const  { return m_x; }
	double get_Y() const { return m_y; }

private:
	double m_x;
	double m_y;
};

class Less_Than_Point
{
	using t_p_fun = double (Point::*)()const;

public:
	Less_Than_Point(t_p_fun Fun) :m_fun(Fun)
	{}

public:
	bool operator() (const Point& p1, const Point& p2)
	{
		return ( (p1.*m_fun)() < (p2.*m_fun)() );
	}

private:
	t_p_fun m_fun;
};
 

class Eq_To_Point
{
	typedef double (Point::*t_p_fun)()const;
public:
	Eq_To_Point(t_p_fun Fun) :m_fun(Fun)
	{}

public:
	bool operator() (const Point& p1, const Point& p2)
	{
		return ( (p1.*m_fun)() == (p2.*m_fun)() );
	}

private:
	t_p_fun m_fun;
};

class obj
{
    public:
        bool operator() (const Point*, const Point* )
        {
            return true;
        }
};

template <typename Iterator, typename Fun>
Iterator find_curve_point(Iterator begin, Iterator end, const Point& point, Fun f)
{
	Less_Than_Point less(f);
	Iterator begin_1 = std::lower_bound(begin, end, point, less);
//	//std::cout << begin->get_X();
//	
	if (begin_1 == end)
		return end;

	Eq_To_Point eq(f);
	if (eq(point, *begin_1))
		return begin_1;

	return end;
}


//
// TODO !!! enhance and implement curve functionality
//
class Curve
{
protected:
	typedef std::vector<Point> t_curve_points;
	typedef t_curve_points::iterator t_points_it;
	typedef t_curve_points::const_iterator t_points_const_it;

public:
	Curve(const std::string& file_path);
	
	virtual ~Curve()
	{}
	
	double get_value_X(double Y) const; ///< Returns X respective to Y
	double get_value_Y(double X) const; ///< Returns Y respective to X

	std::vector<double> get_all_X() const;
	std::vector<double> get_all_Y() const;

	/**
	* Will initialize the curve from file
	*/
	void init_curve();

protected:
	/**
	* Looks for the range which includes given value.
	*/
	typedef double(Point::*fun)() const;
	std::pair<t_points_const_it, t_points_const_it> get_point_range(const t_curve_points& v, Point pt, fun f) const;
	//std::pair<t_points_const_it, t_points_const_it> get_point_range(const t_curve_points& v, Point pt, fun f) const;
	double f_x(Point p1, Point p2, double y) const;

private:
	std::string m_file_path; ///< The file path to initialize data from.

	t_curve_points m_points_X; ///< Stores points sorted by X
	t_curve_points m_points_Y; ///< Stores points sorted by Y

	char m_delim; ///< Used to parse line and get point's values
};

class Curve_BH : public Curve 
{
public:
	Curve_BH(const std::string& file_path);

	~Curve_BH()
	{}

	/**
	* Will return the H for given B
	*/
	double get_H_from_B(double B) const
	{
		return get_value_X(B);
	}
};

#endif // _POINT_H_
