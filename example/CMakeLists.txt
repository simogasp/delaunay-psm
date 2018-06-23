cmake_minimum_required(VERSION 3.1.3)

project(delaunaySample)

if(TARGET delaunay)
    add_library(delaunay-psm::delaunay ALIAS delaunay)
else()
    find_package(delaunay-psm CONFIG REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 11)

add_executable(delaunaySample main.cpp)

target_link_libraries(delaunaySample PUBLIC delaunay-psm::delaunay)

set_target_properties(delaunaySample PROPERTIES DEBUG_POSTFIX "d")

install(TARGETS delaunaySample DESTINATION bin)
