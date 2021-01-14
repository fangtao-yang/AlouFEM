//   *****************
//   *** CLASS DOF ***
//   *****************
 

#include "dictionr.hxx"
#include <stdio.h>

#include "domain.hxx"

/*class Domain ;*/ class Node ; class TimeStep ; class BoundaryCondition ; 
class InitialCondition ;


class Dof 
/*
   This class implements a nodal degree of freedom. A dof is usually attri-
   bute of one node.
 DESCRIPTION
   'number' and 'node' are used for reading/writing data in the data file.
   'equationNumber' is the number of the associated equation in the linear
   system ; it is 0 if the dof is subjected to a boundary condition (b.c.).
   'bc' is the number of the b.c. the dof is subjected to, if any.
   'unknowns' and 'pastUnknowns' are the dictionaries where the dof stores
   its unknowns (e.g., the displacement 'd', the velocity 'v' and the acce-
   leration 'a'), at the current time step and at the previous one.
 TASKS
   - equation numbering, in method 'giveEquationNumber' ;
   - managing its b.c. and its i.c., if any (methods 'hasBc', 'giveBc', etc);
   - managing its unknowns. This includes retrieving the associated solution
     from the linear system, calculating the unknowns (through Newmark, sta-
     tic, etc, formulae), printing the unknowns in the data file ;
 REMARKS
   - class Dof is not a subclass of FEMComponent : a dof belongs to a single
     node, not to the domain ;
   - class Dof is not restricted to structural analysis problems. Unknowns
     may also be pressures, temperatures, etc.
*/
{
   enum { FALSE , TRUE } ;

	 typedef Domain::OutPutContainer_ptr_type OutPutContainer_ptr_type;
	 typedef OutPutContainer::output_container_ptr_type output_container_ptr_type;
   public :

      Dof (int,Node*) ;                                       // constructor
      ~Dof ()   { delete unknowns ; delete pastUnknowns ;}    // destructor.

      double              computeNewmarkUnknown (char,TimeStep*) ;
      double              computeStaticUnknown (char,TimeStep*) ;
      double              computeUnknown (char,TimeStep*) ;
      double              getSolution () ;
      BoundaryCondition*  giveBc () ;
      int                 giveEquationNumber () ;
      InitialCondition*   giveIc () ;
      double              givePastUnknown (char,TimeStep*) ;
      double              giveUnknown (char,TimeStep*) ;
      int                 hasBc () ;
      int                 hasIc () ;
      int                 hasIcOn (char) ;
      void                print (char,TimeStep*, bool use_vec_format) ;
      void                print (char,char,char,TimeStep*) ;
			void                printOutputAt(TimeStep*, bool use_vec_format);
			void                printNewmarkOutputAt(TimeStep* stepN, bool use_vec_format) { this->print('d', 'v', 'a', stepN); }
			void                printStaticOutputAt(TimeStep* stepN, bool use_vec_format) { this->print('d', stepN, use_vec_format); }
      void                printYourself () ;
      void                updateYourself () ;

			void                record(char u, TimeStep* stepN);
			void                recordAt(TimeStep* stepN);
			void                recordStaticAt(TimeStep* stepN) { record('d', stepN); }

private:
	int          number;
	Node*        node;
	int          equationNumber;
	int          bc;
	int          ic;
	Dictionary*  unknowns;
	Dictionary*  pastUnknowns;
} ;

