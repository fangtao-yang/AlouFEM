
SET(DOC_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)


CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/definitions.tex.in ${CMAKE_CURRENT_BINARY_DIR}/definitions.tex)
FILE(GLOB latex_files ${CMAKE_CURRENT_SOURCE_DIR}/*.tex ${CMAKE_CURRENT_SOURCE_DIR}/*.bib)
FILE(COPY ${latex_files} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

FILE(GLOB section_tex ${CMAKE_CURRENT_SOURCE_DIR}/sections/*.tex)
FILE(GLOB section_tex_in ${CMAKE_CURRENT_SOURCE_DIR}/sections/*.tex.in)

FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/sections)
FILE(COPY ${section_tex} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/sections)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/sections/LinEqSolve.tex.in ${CMAKE_CURRENT_BINARY_DIR}/sections/LinEqSolve.tex)


FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/pictures DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/extract_from_src.py DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/AlouFEM/src)

ADD_CUSTOM_TARGET(
  extract
  COMMENT "extract code and comment block from source file"
)

ADD_CUSTOM_COMMAND(
  TARGET extract
  COMMAND ${Python3_EXECUTABLE} extract_from_src.py ${CMAKE_SOURCE_DIR}/AlouFEM/src/skyline.cpp ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)


#python extract_from_src.py D:\LiToSim_Allons_Y\AlouFEM\documentation\AlouFEM\AlouFEM\src\skyline.cpp D:\LiToSim_Allons_Y\AlouFEM\documentation\AlouFEM D:\LiToSim_Allons_Y\AlouFEM\documentation\build\documentation\user_doc
#MESSAGE("latex_files == ${latex_files}")

ADD_CUSTOM_TARGET(
  user_doc
  #WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "build AlouFEM user documentation"
)

ADD_DEPENDENCIES(user_doc extract)


ADD_CUSTOM_COMMAND(
  TARGET user_doc
  COMMAND ${PDFLATEX_COMPILER} -shell-escape -output-directory=${DOC_OUTPUT_DIR} main.tex
  COMMAND ${PDFLATEX_COMPILER} -shell-escape -output-directory=${DOC_OUTPUT_DIR} main.tex
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

