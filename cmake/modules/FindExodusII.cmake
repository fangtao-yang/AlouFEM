# This module finds ExodusII library.
#
# The user can set ExodusII_ROOT in cmake cache:
#  ExodusII_ROOT          path to root directory of ExodusII library
#                         which may contain include, lib or bin sub-directory.
# 
# The environment variable ExodusII_DIR can be used instead of ExodusII_ROOT:
#  ENV{ExodusII_DIR}      path to root directory of ExodusII library
#                         which may contain include, lib or bin sub-directory.
#
# The following variables are configured by this module:
#  ExodusII_FOUND         Set to false, or undefined, if ExodusII isn't found.
#  ExodusII_INCLUDE_DIR   set to include directory if ExodusII is found.
#  ExodusII_LIBRARIES     set to library files if ExodusII is found.

#Using Environment varialble instead
if (NOT ExodusII_ROOT)
  set(ExodusII_ROOT $ENV{ExodusII_DIR})
endif(NOT ExodusII_ROOT)

if (ExodusII_ROOT)
  # Search include directory
  find_path(ExodusII_INCLUDE_DIR exodusII.h PATHS ${ExodusII_ROOT} PATH_SUFFIXES include NO_DEFAULT_PATH)
  # Search library
  find_library(ExodusII_LIBRARIES exodus PATHS ${ExodusII_ROOT} PATH_SUFFIXES lib NO_DEFAULT_PATH)
  # Search shared_library
  
endif(ExodusII_ROOT)


# handle the QUIETLY and REQUIRED arguments and set ExodusII_FOUND to TRUE if 
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  ExodusII
  DEFAULT_MSG
  ExodusII_LIBRARIES ExodusII_INCLUDE_DIR
)




mark_as_advanced(ExodusII_ROOT ExodusII_INCLUDE_DIR ExodusII_LIBRARIES)



