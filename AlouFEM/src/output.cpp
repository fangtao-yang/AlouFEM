
#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

#include "output.hxx"

#include "boost/shared_ptr.hpp"
#include "boost/make_shared.hpp"

template <unsigned int N_node, unsigned int N_dof, unsigned int N_ele, unsigned int N_gauss>
OutPutConstContainer<N_node, N_dof, N_ele, N_gauss>::OutPutConstContainer()
{

}


OutPutContainer::OutPutContainer()
	: _nb_node(0),
	_nb_dof(0),
	_nb_ele(0),
	_nb_gauss(0) {
	_time_sequencer = boost::make_shared<output_time_sequencer_type>();
	_disp_container = boost::make_shared<output_container_type>();
	_stress_container = boost::make_shared<output_container_type>();
	_strain_container = boost::make_shared<output_container_type>();
}

OutPutContainer::OutPutContainer(size_type nb_node, size_type nb_dof, size_type nb_ele, size_type nb_gauss) 
: _nb_node(nb_node),
_nb_dof(nb_dof),
_nb_ele(nb_ele),
_nb_gauss(nb_gauss) {
	_time_sequencer = boost::make_shared<output_time_sequencer_type>();
	_disp_container = boost::make_shared<output_container_type>();
	_stress_container = boost::make_shared<output_container_type>();
	_strain_container = boost::make_shared<output_container_type>();
}

void OutPutContainer::print_info() {
	std::cout << "information of OutPutContainer:\n";
	std::cout << "_nb_node = " << _nb_node << "\n";
	std::cout << "_nb_dof = " << _nb_dof << "\n";
	std::cout << "_nb_ele = " << _nb_ele << "\n";
	std::cout << "_nb_gauss = " << _nb_gauss << "\n" << std::endl;
}

void print_value(OutPutContainer::output_container_ptr_type &container_ptr) {
	for (OutPutContainer::output_container_iter_type iter = (*container_ptr).begin(); iter != (*container_ptr).end(); ++iter) {
		std::vector<OutPutContainer::value_type> &vec = iter->second;
		std::cout << "time = " << iter->first << ", size = " << vec.size() << ", capacity = " << vec.capacity() << "\n";
		for (OutPutContainer::output_vec_iter_type iter_v = vec.begin(); iter_v != vec.end(); ++iter_v) {
			std::cout << (*iter_v) << " ";
		}
		std::cout << "\n";
	}
}

void OutPutContainer::print_container(const std::string & container_name) const {
	std::cout << "values of OutPutContainer:\n";

	if (container_name == "disp") {
		//std::setprecision(5)
		std::cout << "disp_container, size =  " << (*_disp_container).size() << "\n";
		print_value(_disp_container);
		std::cout << std::endl;
	}
	else if (container_name == "strain") {
		std::cout << "strain_container, size =  " << (*_strain_container).size() << "\n";
		print_value(_strain_container);
		std::cout << std::endl;
	}
	else if (container_name == "stress") {
		std::cout << "stress_container, size =  " << (*_stress_container).size() << "\n";
		print_value(_stress_container);
		std::cout << std::endl;
	}
	else {
		std::cout << "the container: " << container_name << " is not defined" << "\n";
		exit(1);
	}
}

void OutPutContainer::add_time_sequencer(time_type time) {
	(*_time_sequencer).push_back(time);
	//disp_container[time]=std::vector<value_type>(_nb_node*_nb_dof, 0.0);
	//strain_container[time] = std::vector<value_type>(_nb_ele*_nb_gauss, 0.0);
	//stress_container[time] = std::vector<value_type>(_nb_ele*_nb_gauss, 0.0);
	(*_disp_container)[time].reserve(_nb_node*_nb_dof);
	(*_strain_container)[time].reserve(_nb_ele*_nb_gauss);
	(*_stress_container)[time].reserve(_nb_ele*_nb_gauss);
}

void OutPutContainer::add_disp(time_type time, value_type x) {
	(*_disp_container)[time].push_back(x);
}

OutPutContainer::size_type OutPutContainer::get_disp_len(time_type time) const {
	return (size_type)(*_disp_container)[time].size();
}

OutPutContainer::time_type OutPutContainer::get_last_time() const {
	return (*_time_sequencer).back();
}

//void OutPutContainer::exportVecAtLast_inFile(const std::string & fileName, const std::string & containerName) {
//	_ofs(fileName);
//	if (ofs.is_open()) {
//		_ofs = ofs;
//	}
//}

void OutPutContainer::exportVecAtLast(const std::string & fileName, const std::string & containerName) const {
	boost::shared_ptr<std::ofstream> ofs = boost::make_shared<std::ofstream>(fileName);
	if ((*ofs).is_open())
	{
		_ofs = ofs;
		if (containerName == "disp") {
			exportVecAtLast_Disp();
			(*_ofs).close();
		}
		else
		{
			std::cout << "containerName in OutPutContainer::exportVecAtLast is not defined";
			exit(1);
		}
	}

}


void OutPutContainer::exportVecAtLast_Disp() const {
	time_type time_last = get_last_time();
	(*_ofs) << get_disp_len(time_last) << "\n";
	output_vec_type disp_vec = get_disp_vec(time_last);
	for (output_vec_iter_type iter = disp_vec.begin(); iter != disp_vec.end(); ++iter)
	{
		(*_ofs) << (*iter) << "\n";
	}
	//(*_ofs) << std::endl;
	std::cout << "Displacement field at last step is exported in file" << std::endl;
}