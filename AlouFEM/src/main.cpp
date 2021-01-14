/*
	Program developed by Yves DUBOIS-PELERIN and Thomas ZIMMERMANN 
	at the Laboratory for Structural and Continuum mechanics, Ecole
	Polytechnique Federale de Lausanne under Swiss National	Science 
	Foundation grant number 21-27589.89, June 1992.
	
	Expurgated version developed by Yves Dubois-Pelerin for ZACE 
	Services Ltd, November 1992.
*/

//  MAIN
//  Solves finite element problems.
//
//  The DEBUG option MUST be used (check in file 'debug.def').
//  See also file 'main2.c'.

#include "domain.hxx"
#include "freestor.h"
#include "compiler.def"

#include <iostream>
#include <string>


int main(int argc, char* argv[])
{
	/*
#ifdef BORLAND
	 set_new_handler (&freeStoreError) ;     // prevents memory overflow
#endif
	 */

	if (argc > 1) {


		int n = 1;
		std::cout << 9/2 << std::endl;


		Domain* mesh;

		mesh = new Domain();

		std::string filename(argv[1]);
		//std::string filename = "PL4.DAT";
		//std::cout << name << std::endl;
		mesh->getDataFileName(filename);

		//filename.assign("stress_strain.out");
		//mesh->setVectorOutPutFileName(filename);
		mesh->solveYourself();

		mesh->getOutPutContainer_ptr()->print_container("disp");
		mesh->exportVecAtLast("disp.out", "disp");
		delete mesh;

		return 0;

	}
	else {
		std::cout << "no input file is given" << std::endl;
		exit(1);
	}
}



