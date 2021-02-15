
CMAKE_MINIMUM_REQUIRED (VERSION 3.4)
PROJECT (AlouFEM)

FUNCTION(COLLECT_SRC_FROM_DIRS DIRS)
  UNSET(SOURCE_FILES PARENT_SCOPE)
  FOREACH(dir ${${DIRS}})
    FILE(GLOB source_file_in_dir ${dir}/[^.]*.cpp ${dir}/[^.]*.hxx)
	LIST(APPEND SOURCE_FILE_LIST ${source_file_in_dir})
  ENDFOREACH()
  SET(SOURCE_FILES ${SOURCE_FILE_LIST} PARENT_SCOPE)
ENDFUNCTION ()

FUNCTION(COLLECT_SHARED_LIBS PACKAGE_LIBRARIES)
  FOREACH(lib ${${PACKAGE_LIBRARIES}})
    GET_FILENAME_COMPONENT(lib_ext ${lib} EXT)
    GET_FILENAME_COMPONENT(lib_name_we ${lib} NAME_WE)
	IF(${lib_ext} STREQUAL ".dll")
	  LIST(APPEND shared_libs ${lib})
	ELSEIF(${lib_ext} STREQUAL ".lib")
	  IF(WIN32)
	    GET_FILENAME_COMPONENT(lib_dir ${lib} DIRECTORY)
		SET(lib_collect ${lib_dir}/${lib_name_we}.dll)
		IF(EXISTS lib_collect)
		  LIST(APPEND shared_libs ${lib_collect})
		ELSE()
		  SET(lib_collect ${lib_dir}/../bin/${lib_name_we}.dll)
		  
		  IF(EXISTS ${lib_collect})
		    LIST(APPEND shared_libs ${lib_collect})
	      ENDIF()
		ENDIF()
	  ENDIF()
	ENDIF()
  ENDFOREACH()
  SET(SHARED_LIBS_COLLECTED ${SHARED_LIBS_COLLECTED} ${shared_libs} PARENT_SCOPE)
ENDFUNCTION()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)
#MESSAGE("CMAKE_MODULE_PATH == ${CMAKE_MODULE_PATH}")

FIND_PACKAGE(NETCDF REQUIRED)
IF(NETCDF_FOUND)
  MESSAGE(STATUS "NETCDF is found")
  INCLUDE_DIRECTORIES(SYSTEM ${NETCDF_INCLUDE_DIR})
  COLLECT_SHARED_LIBS(NETCDF_LIBRARY)
ENDIF()

FIND_PACKAGE(ExodusII REQUIRED)
IF(ExodusII_FOUND)
  MESSAGE(STATUS "ExodusII is found")
  INCLUDE_DIRECTORIES(SYSTEM ${ExodusII_INCLUDE_DIR})
  COLLECT_SHARED_LIBS(ExodusII_LIBRARIES)
ENDIF()

#SET(ENV{BOOST_ROOT} D:/Boost/boost_1_75_0_binary)
#if (NOT "$ENV{BOOST_ROOT}" STREQUAL "")
#if (DEFINED ENV{BOOST_ROOT})
#  #MESSAGE("ENV{BOOST_ROOT} defined == $ENV{BOOST_ROOT}")
#  #set(BOOST_RO "$ENV{EnvironmentVariableName}" CACHE INTERNAL "Copied from environment variable")
#ENDIF()

#Command to compile boost library
#b2.exe --prefix=D:\Boost\boost_1_75_0_binary toolset=msvc-14.0 variant=debug link=static threading=multi runtime-link=shared install
FIND_PACKAGE(Boost REQUIRED COMPONENTS filesystem exception program_options log)

SET(Boost_USE_DEBUG_LIBS ON)
SET(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME ON)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()


FIND_PACKAGE(LATEX)

FIND_PACKAGE(Python3 COMPONENTS Interpreter)
IF (Python3_FOUND)
  MESSAGE("Python3_EXECUTABLE -- ${Python3_EXECUTABLE}")
ENDIF()



ENABLE_TESTING()

ADD_SUBDIRECTORY(vector_validation)
ADD_SUBDIRECTORY(AlouFEM)
ADD_SUBDIRECTORY(boost_test)
ADD_SUBDIRECTORY(ExodusII_test)
ADD_SUBDIRECTORY(documentation)



