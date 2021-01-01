
//#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <boost/exception/all.hpp>
#include <boost/program_options.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/fpclassify.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/exceptions.hpp>


#include <string>
#include <iostream>



bool can_be_opened(const std::string& filename)
{
	std::ifstream file(filename.c_str());
	if (!file.is_open())
	{
		std::cout << " Could not open file : " << filename << std::endl;
		return false;
	}
	else
	{
		file.close();
	}
	return true;
}

bool L1_norm(std::istream& is_ref, std::istream& is_res, const double& tol)
{
	bool res = true;
	size_t size1, size2;
	size_t line_num(0);
	is_ref >> size1;
	is_res >> size2;

	if (size1 != size2)
	{
		std::cout << " The number of values in files does not match ( "
			<< size1 << " - " << size2 << " )" << std::endl;
		return false;
	}

	while (size1-- > 0 && res == true)
	{
		++line_num;

		double val_ref(1), val_res(-1);
		std::string str_ref, str_res;

		is_ref >> str_ref;
		is_res >> str_res;

		//    try
		//    {
		val_ref = boost::lexical_cast<double>(str_ref);
		//    }
		//    catch( boost::exception const & e )
		//    {
		//      boost::rethrow_exception( boost::copy_exception(e) );
		//    }   

		//    try
		//    {
		val_res = boost::lexical_cast<double>(str_res);
		//    }
		//    catch( boost::exception const & e )
		//    {
		//      boost::rethrow_exception( boost::copy_exception(e) );
		//    }  

		if (boost::math::isfinite(val_ref))        // finite value 
		{
			if (val_ref != 0.0)                        // non zero
			{
				res = std::abs((val_ref - val_res) / val_ref) < tol;
			}
			else                                        // exact zero
			{
				res = std::abs(val_res) < tol;
			}
		}
		else                                          // non finite value
		{
			res = ((boost::math::isinf(val_ref) &&
				boost::math::isinf(val_res) &&
				(val_ref == val_res)) ||
				(boost::math::isnan(val_ref) &&
					boost::math::isnan(val_res)));
		}

		if (!res)
		{
			std::cout << " the local values (respect to the tolerance) are different, at line "
				<< line_num << " : test value is \"" << str_res
				<< "\" while reference value is \"" << str_ref << "\"" << std::endl;
		}
	}

	return res;
}

bool L2_norm(std::istream& is_ref, std::istream& is_res, const double& tol)
{
	bool res = true;
	size_t size_ref, size2_res;
	is_ref >> size_ref;
	is_res >> size2_res;
	if (size_ref != size2_res)
	{
		std::cout << " The number of values in files does not match ( "
			<< size_ref << " - " << size2_res << " )" << std::endl;
		return false;
	}

	double norm_ref(0.0);
	double norm_res(0.0);
	double norm_diff(0.0);

	double val_ref(0.0);
	double val_res(0.0);
	std::string str_ref, str_res;

	for (size_t i = 0; i < size_ref; ++i)
	{
		is_ref >> str_ref;
		is_res >> str_res;

		try
		{
			val_ref = boost::lexical_cast<double>(str_ref);
		}
		catch (boost::bad_lexical_cast const& e)
		{
			std::cout << "Error: " << e.what() << std::endl;
			boost::rethrow_exception(boost::copy_exception(e));
		}

		try
		{
			val_res = boost::lexical_cast<double>(str_res);
		}
		catch (boost::bad_lexical_cast const& e)
		{
			std::cout << "Error: " << e.what() << std::endl;
			boost::rethrow_exception(boost::copy_exception(e));
		}
		norm_ref += val_ref*val_ref;
		norm_res += val_res*val_res;
		norm_diff += (val_ref - val_res) * (val_ref - val_res);
	}
	norm_ref = std::sqrt(norm_ref);
	norm_res = std::sqrt(norm_res);
	norm_diff = std::sqrt(norm_diff);
	if (std::abs(norm_ref) > std::numeric_limits<double>::epsilon())
	{
		res = (std::abs((norm_ref - norm_res) / norm_ref) < tol);
	}
	else
	{
		res = (std::abs(norm_res) < tol);
	}

	return res;
}

int main(int argc, char* argv[])
{
	std::vector<std::string> reference_files_name(4), files_name(4);
	bool is_all_same (true);
	bool verbosity(false);
	bool l2(true), l1(false);
	double tolerance(1.0e-8);

	boost::program_options::options_description prg_description("Allowed options:");
	prg_description.add_options()
		("help,h", "print usage message")
		("filecheck", "enable filecheck")
		("verbose,v", boost::program_options::bool_switch(&verbosity), "enable verbosity")
		("l1_norm", boost::program_options::bool_switch(&l1), "use relative L1 norm")
		("l2_norm", boost::program_options::bool_switch(&l2), "use relative L2 norm")
		("tolerance,t", boost::program_options::value<double>(&tolerance)->default_value(1.0e-8), "relative tolerance")
		("reference,r", boost::program_options::value< std::vector<std::string> >(&reference_files_name), "reference files, reference files sequence to compare")
		("file,f", boost::program_options::value< std::vector<std::string> >(&files_name), "computated file to comapre");


	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::parse_command_line(argc, argv, prg_description), vm);
	boost::program_options::notify(vm);

	if (vm.count("help"))
	{
		std::cout << prg_description << std::endl;
		return 0;
	}

	std::vector<std::string>::const_iterator iter_ref_filename = reference_files_name.begin();
	std::vector<std::string>::const_iterator iter_testing_filename = files_name.begin();
	for (; iter_ref_filename != reference_files_name.end(); ++iter_ref_filename, ++iter_testing_filename)
	{
		const std::string& filename_ref = *iter_ref_filename;
		const std::string& filename_res = *iter_testing_filename;

		if (can_be_opened(filename_ref) && can_be_opened(filename_res))
		{
			std::ifstream if_ref(filename_ref.c_str());
			std::ifstream if_res(filename_res.c_str());
			bool is_the_same(false);
			if (l1)
			{
				is_the_same = L1_norm(if_ref, if_res, tolerance);
			}
			else
			{
				is_the_same = L2_norm(if_ref, if_res, tolerance);
			}

			if_ref.close();
			if_res.close();
			if (verbosity)
			{
				if (is_the_same)
				{
					std::cout << "Ok" << std::endl;
				}
				else
				{
					std::cout << "Not ok" << std::endl;
				}
			}
			is_all_same = is_all_same && is_the_same;
		}
		else
		{
			exit(2);
		}
	}

	return (is_all_same ? 0 : 1);
}