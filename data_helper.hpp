#ifndef _DATA_HELPER_H_
#define _DATA_HELPER_H_

#include <algorithm>
#include <vector>

class data_helper
{
	data_helper(const data_helper&) = delete;
	data_helper& operator=(const data_helper&) = delete;

private:

	data_helper()
	{
		init_wire_lib();
		init_temp_heat();
	};

public:
	using db_pair = std::pair<float, float>;
	using db_pair_it = std::vector<data_helper::db_pair>::const_iterator;

	static data_helper& get_inst()
	{
		static data_helper obj;
		return obj;
	}

public:
	db_pair get_wire_diam(float diam) const;
	db_pair get_heat_cf(float max_temp) const;
    float get_isolation(float diam) const;

protected:
	void init_wire_lib();
	void init_temp_heat();

	db_pair_it get_near_data(const std::vector<db_pair>& vec, float val) const;

private:
	std::vector<db_pair> m_diam_area;
	std::vector<db_pair> m_temp_heat_cf;
};

#endif //_DATA_HELPER_H_
