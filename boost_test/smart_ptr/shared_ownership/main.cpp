#include <boost/exception/all.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

int main()
{
	std::cout << "set p1" << '\n';
  boost::shared_ptr<int> p1{new int{1}};

	std::cout << "see: p1" << '\n';
  std::cout << *p1 << '\n';
	std::cout << p1.use_count() << std::endl;
	std::cout << std::boolalpha << p1.unique() << std::endl;

	std::cout << "set p2" << '\n';
  boost::shared_ptr<int> p2{p1};
	
	std::cout << "see: p1" << '\n';
	std::cout << *p1 << '\n';
	std::cout << p1.use_count() << std::endl;
	std::cout << std::boolalpha << p1.unique() << std::endl;

	std::cout << "see: p2" << '\n';
	std::cout << *p2 << '\n';
	std::cout << p2.use_count() << std::endl;
	std::cout << std::boolalpha << p2.unique() << "\n" << std::endl;

	std::cout << "p1.reset(new int{2})" << '\n';
  p1.reset(new int{2});
  std::cout << "*p1.get(): " << *p1.get() << '\n';
	std::cout << "*p1: " << *p1 << '\n';
	std::cout << "p1.use_count(): " << p1.use_count() << '\n';
	std::cout << std::boolalpha << "p1.unique(): " << p1.unique() << std::endl;

	std::cout << "p1.reset();" << '\n';
  p1.reset();
//	try {
//		std::cout << "*p1: " << *p1 << '\n';
//		std::cout << "p1.use_count(): " << p1.use_count() << '\n';
//		std::cout << std::boolalpha << "p1.unique(): " << p1.unique() << std::endl;
//	}
//	catch (boost::exception &e) {
//		std::cerr << boost::diagnostic_information(e);
//	}


  std::cout << std::boolalpha << static_cast<bool>(p2) << '\n' << std::endl;

	std::cout << "p3 = boost::make_shared<int>(1);" << '\n';
	auto p3 = boost::make_shared<int>(1);
	std::cout << typeid(p3).name() << '\n';
	auto p4 = boost::make_shared<int[]>(10);
	std::cout << typeid(p4).name() << '\n';
	p4[0] = 1;
	std::cout << "(*p4)[0] = " << p4[0] << '\n';
	std::cout << "(*p4)[1] = " << p4[1] << '\n' << std::endl;

	return 0;
}