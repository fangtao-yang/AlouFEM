//   ********************
//   *** CLASS DOMAIN ***
//   ********************

 
#ifndef domain_hxx

#include "list.hxx"
#include "output.hxx"
#include <stdio.h>

#include <string>

#include <boost/shared_ptr.hpp>

class Element ; class Node ; class Material ; class TimeIntegrationScheme ;
class TimeStep ; class Load ; class LoadTimeFunction ; class LinearSystem ;
class FileReader ;


class Domain
/*
   This class implements the domain of a finite element problem.
 DESCRIPTION :
   The domain stores its components (elements, nodes, materials, loads, load-
   time functions) in lists : 'elementList', 'nodeList', etc. Its component
   'timeIntegrationScheme' needs no list, since it is unique. The domain
   also possesses the system of linear algebraic equations of the problem
   ('linearSystem').
   The domain possesses two streams 'inputStream' and 'outputStream' that
   the components use in order to read their input and write their output in
   the data file.
 TASKS :
   - User interfacing. The domain is the primary interlocutor of the user.
     In particular, it understands the message 'solveYourself', which trig-
     gers the solving process.
   - Managing its components and the linear system.
     The domain maintains lists of its components. The domain is responsible
     for creating these components (methods 'giveElement','giveLinearSystem',
     etc). Asking the domain is the only way for a component (e.g., an ele-
     ment) to have access to another component (e.g., its material), or to
     the data file, or to the linear system.
   - Returning the input/output streams. Since these two streams act on the
     same file (the data file), the domain is responsible for always closing
     one when the other one is to be used.
*/
{
	
   public :
		 typedef boost::shared_ptr<OutPutContainer> OutPutContainer_ptr_type;
		 typedef OutPutContainer::size_type size_type;
		 typedef OutPutContainer::time_type time_type;

      Domain () ;                             // constructors
      Domain (char*) ;
      ~Domain () ;                            // destructor

      // solving
      void               solveYourself () ;
      void               solveYourselfAt (TimeStep*) ;
      void               formTheSystemAt (TimeStep*) ;
      int                giveNumberOfElements () ;
      void               terminate (TimeStep*, bool use_vec_format) ;

			void               recordAt(TimeStep* stepN);
			void               exportVecAtLast(const std::string & fileName, const std::string & containerName) const;

      // management of the mesh components
      Element*           giveElement (int) ;
      LinearSystem*      giveLinearSystem ()     { return linearSystem ;}
      Load*              giveLoad (int) ;
      LoadTimeFunction*  giveLoadTimeFunction (int) ;
      Material*          giveMaterial (int) ;
      Node*              giveNode (int) ;
      TimeIntegrationScheme*  giveTimeIntegrationScheme () ;
      void               instanciateYourself () ;

      // input / output
      char*              giveDataFileName () ;
	  void               getDataFileName (std::string & name) ;
      FileReader*        giveInputStream () ;
      FILE*              giveOutputStream () ;
	  FILE*              gvieNewOutputStream () ;
      int                readNumberOf (char*) ;

	  void outPutContainer_Init();
		//void outPutContainer_Init(unsigned int nb_node, unsigned int nb_dof, unsigned int nb_ele, unsigned int nb_gauss);
		void outPutContainer_Init(size_type nb_node, size_type nb_dof, size_type nb_ele, size_type nb_gauss);

		OutPutContainer_ptr_type & getOutPutContainer_ptr() { return outPutContatiner_ptr; }

	  // member operation
	  void				setVectorOutPutFileName(std::string & name);

private:
	char*       	      dataFileName;
	std::string		  vectorOutPutFileName;
	List*        	      elementList;
	List*        	      nodeList;
	List*        	      materialList;
	List*        	      loadList;
	List*                   loadTimeFunctionList;
	TimeIntegrationScheme*  timeIntegrationScheme;
	int                     numberOfElements;
	unsigned int            _nb_node;
	LinearSystem*  	      linearSystem;
	FileReader*  	      inputStream;
	FILE*      	      outputStream;

	OutPutContainer_ptr_type   outPutContatiner_ptr;


} ;

#define domain_hxx
#endif









