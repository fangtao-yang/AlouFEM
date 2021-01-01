

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
class OutPutConstContainer
{
	typedef boost::shared_ptr<Domain> Domain_ptr;
	typedef boost::shared_ptr<OutPutConstContainer<N_node, N_dof, N_ele, N_gauss>> OutPutContatiner_ptr_type;
public:
	OutPutConstContainer();
	//OutPutContatiner (const std::string & filename);

protected:

	bool is_ele_uniform;
	std::string filename_output;

	//boost::array<size_t, > my_array = {};


	// double is time (t in TimeStep), int is component number, vector<double> is the component (displacement, strain, stress) value 
	std::map<double, boost::array<double, N_node*N_dof>> disp_container;
	std::map<double, boost::array<double, N_ele*N_gauss>> strain_container;
	std::map<double, boost::array<double, N_ele*N_gauss>> stress_container;

	Domain_ptr _domain;
};

class OutPutContainer
{
public:
	typedef unsigned int size_type;
	typedef double time_type;
	typedef double value_type;
	typedef std::vector<value_type> output_vec_type;
	typedef output_vec_type::iterator output_vec_iter_type;
	typedef std::map<time_type, output_vec_type> output_container_type;
	typedef boost::shared_ptr<output_container_type> output_container_ptr_type;
	typedef output_container_type::iterator output_container_iter_type;
	
	typedef std::vector<time_type> output_time_sequencer_type;
	typedef boost::shared_ptr<output_time_sequencer_type> output_time_sequencer_ptr_type;

	OutPutContainer();
	OutPutContainer(size_type nb_node, size_type nb_dof, size_type nb_ele, size_type nb_gauss);

	// member function
	void set_node_info(size_type nb_node, size_type nb_dof) { _nb_node = nb_node; _nb_dof = nb_dof; }
	void set_ele_info(size_type nb_ele, size_type nb_gauss) { _nb_ele = nb_ele; _nb_gauss = nb_gauss; }
	void add_time_sequencer(time_type time);

	void add_disp(time_type time, value_type x);

	size_type get_disp_len(time_type time) const;
	time_type get_last_time() const;

	// io interface
	void print_info();
	void print_container(const std::string & container_name) const;

	void exportVecAtLast(const std::string & fileName, const std::string & containerName) const;
	void exportVecAtLast_Disp() const;

	// manipulate interface
	output_container_ptr_type & get_disp_container() const { return _disp_container; }
	output_container_ptr_type & get_strain_container() const { return _strain_container; }
	output_container_ptr_type & get_stress_container() const { return _stress_container; }
	output_vec_type & get_disp_vec(time_type time) const { return (*_disp_container)[time]; }


protected:
	size_type			_nb_node;
	size_type			_nb_dof;
	size_type			_nb_ele;
	size_type			_nb_gauss;

	output_time_sequencer_ptr_type _time_sequencer;

	mutable boost::shared_ptr<std::ofstream> _ofs;

	mutable output_container_ptr_type _disp_container;
	mutable output_container_ptr_type _strain_container;
	mutable output_container_ptr_type _stress_container;

};