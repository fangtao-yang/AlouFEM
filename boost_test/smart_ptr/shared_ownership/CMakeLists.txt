
SET(TEST_EXE shared_ownership)
ADD_EXECUTABLE(${TEST_EXE} main.cpp)
if(Boost_FOUND)
  #MESSAGE("Boost_LIBRARIES used: ${Boost_LIBRARIES}")
  target_link_libraries(${TEST_EXE} ${Boost_LIBRARIES})
endif()