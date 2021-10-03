#include "data_helper.hpp"

data_helper::db_pair data_helper::get_wire_diam(float diam) const
{
	db_pair_it it = get_near_data(m_diam_area, diam);

	if ( it == m_diam_area.end() )
		return m_diam_area.back(); 

	return *it;
}

data_helper::db_pair data_helper::get_heat_cf(float max_temp) const
{
	db_pair_it it = get_near_data(m_temp_heat_cf, max_temp);

	if (it == m_temp_heat_cf.end())
		return m_temp_heat_cf.back();

	return *it;
}

data_helper::db_pair_it data_helper::get_near_data(const std::vector<db_pair>& vec, float val) const
{
	auto comp = [](db_pair a, db_pair b)
	{
		return a.first < b.first;
	};

	db_pair val_pr = std::make_pair(val, -1);
	return std::lower_bound(vec.begin(), vec.end(), val_pr, comp);
}


void data_helper::init_wire_lib()
{
	m_diam_area.reserve(80);

	m_diam_area.push_back(std::make_pair(0.05, 0.00196));
	m_diam_area.push_back(std::make_pair(0.06, 0.00283));
	m_diam_area.push_back(std::make_pair(0.07, 0.00385));
	m_diam_area.push_back(std::make_pair(0.08, 0.00503));
	m_diam_area.push_back(std::make_pair(0.09, 0.006));
	m_diam_area.push_back(std::make_pair(0.10, 0.00785));
	m_diam_area.push_back(std::make_pair(0.11, 0.0095));
	m_diam_area.push_back(std::make_pair(0.12, 0.01131));
	m_diam_area.push_back(std::make_pair(0.13, 0.01327));
	m_diam_area.push_back(std::make_pair(0.14, 0.01539));
	m_diam_area.push_back(std::make_pair(0.15, 0.01767));
	m_diam_area.push_back(std::make_pair(0.16, 0.0201));
	m_diam_area.push_back(std::make_pair(0.17, 0.0227));
	m_diam_area.push_back(std::make_pair(0.18, 0.0255));
	m_diam_area.push_back(std::make_pair(0.19, 0.0284));
	m_diam_area.push_back(std::make_pair(0.2, 0.0314));
	m_diam_area.push_back(std::make_pair(0.21, 0.0346));
	m_diam_area.push_back(std::make_pair(0.23, 0.0415));
	m_diam_area.push_back(std::make_pair(0.25, 0.0491));
	m_diam_area.push_back(std::make_pair(0.27, 0.0593));
	m_diam_area.push_back(std::make_pair(0.29, 0.0661));
	m_diam_area.push_back(std::make_pair(0.31, 0.0755));
	m_diam_area.push_back(std::make_pair(0.33, 0.0855));
	m_diam_area.push_back(std::make_pair(0.35, 0.0962));
	m_diam_area.push_back(std::make_pair(0.38, 0.1134));
	m_diam_area.push_back(std::make_pair(0.41, 0.1320));
	m_diam_area.push_back(std::make_pair(0.44, 0.1521));
	m_diam_area.push_back(std::make_pair(0.47, 0.1735));
	m_diam_area.push_back(std::make_pair(0.49, 0.1886));
	m_diam_area.push_back(std::make_pair(0.51, 0.2043));
	m_diam_area.push_back(std::make_pair(0.53, 0.221));
	m_diam_area.push_back(std::make_pair(0.55, 0.238));
	m_diam_area.push_back(std::make_pair(0.57, 0.255));
	m_diam_area.push_back(std::make_pair(0.59, 0.273));
	m_diam_area.push_back(std::make_pair(0.62, 0.302));
	m_diam_area.push_back(std::make_pair(0.64, 0.322));
	m_diam_area.push_back(std::make_pair(0.67, 0.353));
	m_diam_area.push_back(std::make_pair(0.69, 0.374));
	m_diam_area.push_back(std::make_pair(0.72, 0.407));
	m_diam_area.push_back(std::make_pair(0.74, 0.430));
	m_diam_area.push_back(std::make_pair(0.77, 0.466));
	m_diam_area.push_back(std::make_pair(0.8, 0.503));
	m_diam_area.push_back(std::make_pair(0.83, 0.541));
	m_diam_area.push_back(std::make_pair(0.86, 0.581));
	m_diam_area.push_back(std::make_pair(0.9, 0.636));
	m_diam_area.push_back(std::make_pair(0.93, 0.679));
	m_diam_area.push_back(std::make_pair(0.96, 0.724));
	m_diam_area.push_back(std::make_pair(1.0, 0.785));
	m_diam_area.push_back(std::make_pair(1.04, 0.849));
	m_diam_area.push_back(std::make_pair(1.08, 0.916));
	m_diam_area.push_back(std::make_pair(1.12, 0.985));
	m_diam_area.push_back(std::make_pair(1.16, 1.057));
	m_diam_area.push_back(std::make_pair(1.20, 1.131));
	m_diam_area.push_back(std::make_pair(1.25, 1.227));
	m_diam_area.push_back(std::make_pair(1.3, 1.327));
	m_diam_area.push_back(std::make_pair(1.35, 1.431));
	m_diam_area.push_back(std::make_pair(1.4, 1.539));
	m_diam_area.push_back(std::make_pair(1.45, 1.651));
	m_diam_area.push_back(std::make_pair(1.5, 1.767));
	m_diam_area.push_back(std::make_pair(1.56, 1.911));
	m_diam_area.push_back(std::make_pair(1.62, 2.06));
	m_diam_area.push_back(std::make_pair(1.68, 2.217));
	m_diam_area.push_back(std::make_pair(1.74, 2.38));
	m_diam_area.push_back(std::make_pair(1.81, 2.57));
	m_diam_area.push_back(std::make_pair(1.88, 2.78));
	m_diam_area.push_back(std::make_pair(1.95, 2.99));
	m_diam_area.push_back(std::make_pair(2.02, 3.205));
	m_diam_area.push_back(std::make_pair(2.1, 3.46));
	m_diam_area.push_back(std::make_pair(2.26, 4.01));
	m_diam_area.push_back(std::make_pair(2.44, 4.68));
	m_diam_area.push_back(std::make_pair(2.63, 5.43));
	m_diam_area.push_back(std::make_pair(2.83, 6.29));
	m_diam_area.push_back(std::make_pair(3.05, 7.31));
	m_diam_area.push_back(std::make_pair(3.28, 8.45));
	m_diam_area.push_back(std::make_pair(3.53, 9.79));
	m_diam_area.push_back(std::make_pair(3.8, 11.31));
	m_diam_area.push_back(std::make_pair(4.1, 13.2));
	m_diam_area.push_back(std::make_pair(4.5, 15.9));
	m_diam_area.push_back(std::make_pair(4.8, 18.1));
	m_diam_area.push_back(std::make_pair(5.2, 21.2));
}

void data_helper::init_temp_heat()
{
	m_temp_heat_cf.reserve(10);

	m_temp_heat_cf.push_back(std::make_pair(40,  9.84*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(45, 10.01*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(50, 10.19*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(55, 10.37*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(60, 10.54*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(65, 10.72*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(70, 10.99*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(75, 11.17*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(80, 11.35*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(85, 11.52*10e-6));
	m_temp_heat_cf.push_back(std::make_pair(160, 14.4*10e-6));

}

float data_helper::get_isolation(float diam) const 
{
	if (diam >= 0.05 && diam < 0.1)
		return 0.07;

	if (diam >= 0.1 && diam < 0.2)
		return 0.075;

	if (diam >= 0.2 && diam < 0.27)
		return 0.09;

	if (diam >= 0.27 && diam < 0.31)
		return 0.1;

	if (diam >= 0.31 && diam < 0.38)
		return 0.105;

	if (diam >= 0.38 && diam <= 0.51)
		return 0.11;

	if (diam >= 0.51 && diam <= 0.72)
		return 0.115;

	if (diam >= 0.72 && diam < 1)
		return 0.125;

	if (diam >= 1 && diam < 1.5)
		return 0.135;

	if (diam >= 1.5 && diam < 2.26)
		return 0.155;

	if (diam >= 2.26 && diam <= 5.2)
		return 0.33; // approximate

	return -1; 
}
