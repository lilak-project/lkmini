cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(LKMINI)

add_compile_definitions(LKMINI_COMPILED)
set(LKMINI_SOURCE_DIRECTORY_LIST ${CMAKE_SOURCE_DIR}/source CACHE INTERNAL "")
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
find_package(ROOT REQUIRED COMPONENTS RIO Net Geom Gui Graf Graf3d Ged Matrix MathCore Physics)
include(${ROOT_USE_FILE})

set(ROOTLOGON_CONTENT [[
{
    if (gSystem->Load("${CMAKE_BINARY_DIR}/libLKMINI")<=1)
        cout << "Loading LKMINI" << endl;
    else
        cout << "Cannot load LKMINI" << endl;
}
]])

#file(WRITE "${CMAKE_SOURCE_DIR}/rootlogon.C" "${ROOTLOGON_CONTENT}")
file(WRITE ${CMAKE_SOURCE_DIR}/rootlogon.C
"{\n"
"    if (gSystem->Load(\"${CMAKE_BINARY_DIR}/libLKMINI\")<=1)\n"
"        cout << \"Loading LKMINI\" << endl;\n"
"    else\n"
"        cout << \"Cannot load LKMINI\" << endl;\n"
"}"
)

file(GLOB EXAMPLE_DIRS RELATIVE "${CMAKE_SOURCE_DIR}/examples" "${CMAKE_SOURCE_DIR}/examples/*")
foreach(d ${EXAMPLE_DIRS})
  if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/examples/${d}")
    #file(WRITE "${CMAKE_SOURCE_DIR}/examples/${d}/rootlogon.C" "${ROOTLOGON_CONTENT}")
    file(WRITE "${CMAKE_SOURCE_DIR}/examples/${d}/rootlogon.C"
"{\n"
"    if (gSystem->Load(\"${CMAKE_BINARY_DIR}/libLKMINI\")<=1)\n"
"        cout << \"Loading LKMINI\" << endl;\n"
"    else\n"
"        cout << \"Cannot load LKMINI\" << endl;\n"
"}"
)
  endif()
endforeach()

file(WRITE ${CMAKE_BINARY_DIR}/LinkDef.h.in
"#ifdef __CINT__\n"
"\n"
"#pragma link off all globals;\n"
"#pragma link off all classes;\n"
"#pragma link off all functions;\n"
"@PRAGMA_LINE_CPP_CLASS_LIST@\n"
"#endif\n"
)

set(LKMINI_ROOT_LINKED_CLASS_LIST)

foreach(_directory ${LKMINI_SOURCE_DIRECTORY_LIST})
    message(STATUS "source directory : " ${_directory})
    include_directories(${_directory})
    string(REPLACE / _ _name ${_directory})
    list(APPEND LKMINI_SOURCE_NAME_LIST ${_name})
    set(LKMINI_${_name}_G_NAME G__${_name}_Dict)
    file(GLOB LKMINI_${_name}_SOURCE_FILES ${_directory}/*.cpp)
    file(GLOB LKMINI_${_name}_HEADER_FILES ${_directory}/*.h)
    set(PRAGMA_LINE_CPP_CLASS_LIST)
    foreach(_full_name ${LKMINI_${_name}_HEADER_FILES})
        get_filename_component(file_name ${_full_name} NAME)
        if(file_name STREQUAL "LKCompiled.h")
            continue()
        endif()
        message(STATUS "adding ${file_name}")
        string(REPLACE .h "" class_name ${file_name})
        set(pragma_line "\n#pragma link C++ class ${class_name}+")
        list(APPEND PRAGMA_LINE_CPP_CLASS_LIST ${pragma_line})
        list(APPEND LKMINI_ROOT_LINKED_CLASS_LIST ${class_name})
    endforeach()
    list(APPEND PRAGMA_LINE_CPP_CLASS_LIST "\n")
    configure_file(${CMAKE_BINARY_DIR}/LinkDef.h.in ${CMAKE_BINARY_DIR}/LinkDef.h @ONLY)
    set(LKMINI_${_name}_LINKDEF_FILE ${CMAKE_BINARY_DIR}/LinkDef.h)
endforeach(_directory)

add_definitions(${ROOT_CXX_FLAGS})
foreach(_name ${LKMINI_SOURCE_NAME_LIST})
    ROOT_GENERATE_DICTIONARY(
        ${LKMINI_${_name}_G_NAME}
        ${LKMINI_${_name}_HEADER_FILES}
        LINKDEF ${LKMINI_${_name}_LINKDEF_FILE}
    )
endforeach(_name)

foreach(_name ${LKMINI_SOURCE_NAME_LIST})
    foreach(_file ${LKMINI_${_name}_SOURCE_FILES})
        list(APPEND LKMINI_LIB_LIST ${_file})
        list(APPEND LKMINI_SOURCE_FILES ${_file})
    endforeach(_file)
    foreach(_file ${LKMINI_${_name}_HEADER_FILES})
        list(APPEND LKMINI_HEADER_FILES ${_file})
    endforeach(_file)
    list(APPEND LKMINI_LIB_LIST ${LKMINI_${_name}_G_NAME}.cxx)
endforeach(_name)

add_library(LKMINI SHARED ${LKMINI_LIB_LIST})
target_link_libraries(LKMINI PUBLIC ${ROOT_LIBRARIES})
target_include_directories(LKMINI PUBLIC ${ROOT_INCLUDE_DIRS} ${LKMINI_SOURCE_DIRECTORY_LIST})
