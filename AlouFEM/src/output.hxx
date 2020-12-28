

#include <string>
#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_io.hpp>

#include <boost/array.hpp>

class Domain;

//namespace fs = boost::filesystem;
template <unsigned int N_node, unsigned int N_dof, unsigned int N_ele, unsigned int N_gauss>
class OutPutContatiner
{
	typedef boost::shared_ptr<Domain> Domain_ptr;
public:
	OutPutContatiner();
	//OutPutContatiner (const std::string & filename);

protected:

	bool is_ele_uniform = true;
	std::string filename_output;

	//boost::array<size_t, > my_array = {};


	// double is time (t in TimeStep), int is component number, vector<double> is the component (displacement, strain, stress) value 
	std::map<double, boost::array<double, N_node>> disp_container;
	std::map<double, boost::array<double, N_ele>> strain_container;
	std::map<double, boost::array<double, N_ele>> stress_container;

	Domain_ptr _domain;
};