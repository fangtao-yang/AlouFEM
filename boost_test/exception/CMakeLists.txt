
LIST(APPEND EXCEPTION_TESTS using_boost_exception BOOST_THROW_EXCEPTION_test selectively_accessing_data)

FOREACH(dir ${EXCEPTION_TESTS})
  ADD_SUBDIRECTORY(${dir})
ENDFOREACH()
