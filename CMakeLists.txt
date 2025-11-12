cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(BEAMPID)

add_compile_definitions(BEAMPID_COMPILED)
set(BEAMPID_SOURCE_DIRECTORY_LIST ${CMAKE_SOURCE_DIR}/source CACHE INTERNAL "")
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net Geom Gui Graf Graf3d Ged Matrix MathCore Physics)
include(${ROOT_USE_FILE})

file(WRITE ${CMAKE_SOURCE_DIR}/rootlogon.C
"{\n"
"    if (gSystem->Load(\"${CMAKE_BINARY_DIR}/libBEAMPID\")<=1)\n"
"        cout << \"Loading BeamPID\" << endl;\n"
"    else\n"
"        cout << \"Cannot load BeamPID\" << endl;\n"
"}"
)

file(WRITE ${CMAKE_BINARY_DIR}/LinkDef.h.in
"#ifdef __CINT__\n"
"\n"
"#pragma link off all globals;\n"
"#pragma link off all classes;\n"
"#pragma link off all functions;\n"
"@PRAGMA_LINE_CPP_CLASS_LIST@\n"
"#endif\n"
)

set(BEAMPID_ROOT_LINKED_CLASS_LIST)

foreach(_directory ${BEAMPID_SOURCE_DIRECTORY_LIST})
    message(STATUS "source directory : " ${_directory})
    include_directories(${_directory})
    string(REPLACE / _ _name ${_directory})
    list(APPEND BEAMPID_SOURCE_NAME_LIST ${_name})
    set(BEAMPID_${_name}_G_NAME G__${_name}_Dict)
    file(GLOB BEAMPID_${_name}_SOURCE_FILES ${_directory}/*.cpp)
    file(GLOB BEAMPID_${_name}_HEADER_FILES ${_directory}/*.h)
    set(PRAGMA_LINE_CPP_CLASS_LIST)
    foreach(_full_name ${BEAMPID_${_name}_HEADER_FILES})
        get_filename_component(file_name ${_full_name} NAME)
        if(file_name STREQUAL "LKCompiled.h")
            continue()
        endif()
        message(STATUS "adding ${file_name}")
        string(REPLACE .h "" class_name ${file_name})
        set(pragma_line "\n#pragma link C++ class ${class_name}+")
        list(APPEND PRAGMA_LINE_CPP_CLASS_LIST ${pragma_line})
        list(APPEND BEAMPID_ROOT_LINKED_CLASS_LIST ${class_name})
    endforeach()
    list(APPEND PRAGMA_LINE_CPP_CLASS_LIST "\n")
    configure_file(${CMAKE_BINARY_DIR}/LinkDef.h.in ${CMAKE_BINARY_DIR}/LinkDef.h @ONLY)
    set(BEAMPID_${_name}_LINKDEF_FILE ${CMAKE_BINARY_DIR}/LinkDef.h)
endforeach(_directory)

add_definitions(${ROOT_CXX_FLAGS})
foreach(_name ${BEAMPID_SOURCE_NAME_LIST})
    ROOT_GENERATE_DICTIONARY(
        ${BEAMPID_${_name}_G_NAME}
        ${BEAMPID_${_name}_HEADER_FILES}
        LINKDEF ${BEAMPID_${_name}_LINKDEF_FILE}
    )
endforeach(_name)

foreach(_name ${BEAMPID_SOURCE_NAME_LIST})
    foreach(_file ${BEAMPID_${_name}_SOURCE_FILES})
        list(APPEND BEAMPID_LIB_LIST ${_file})
        list(APPEND BEAMPID_SOURCE_FILES ${_file})
    endforeach(_file)
    foreach(_file ${BEAMPID_${_name}_HEADER_FILES})
        list(APPEND BEAMPID_HEADER_FILES ${_file})
    endforeach(_file)
    list(APPEND BEAMPID_LIB_LIST ${BEAMPID_${_name}_G_NAME}.cxx)
endforeach(_name)

add_library(BEAMPID SHARED ${BEAMPID_LIB_LIST})
target_link_libraries(BEAMPID PUBLIC ${ROOT_LIBRARIES})
target_include_directories(BEAMPID PUBLIC ${ROOT_INCLUDE_DIRS} ${BEAMPID_SOURCE_DIRECTORY_LIST})
