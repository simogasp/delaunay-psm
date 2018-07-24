cmake_minimum_required(VERSION 3.1.3)

project(delaunay-psm LANGUAGES C CXX VERSION 1.6.6)

set(CMAKE_CXX_STANDARD 11)

option(BUILD_SAMPLE "Build the example file" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

find_package(Threads REQUIRED)

add_library(delaunay delaunay-psm/Delaunay_psm.cpp)
target_link_libraries(delaunay PRIVATE Threads::Threads)

set_target_properties(delaunay PROPERTIES DEBUG_POSTFIX "d")
set_target_properties(delaunay PROPERTIES
        VERSION ${PROJECT_VERSION}
        SOVERSION ${PROJECT_VERSION})


####
# Installation

# Layout. This works for all platforms:
#   * <prefix>/lib/cmake/<PROJECT-NAME>
#   * <prefix>/lib/
#   * <prefix>/include/
set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(include_install_dir "include")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Note: PROJECT_VERSION is used as a VERSION
write_basic_package_version_file("${version_config}" COMPATIBILITY SameMajorVersion)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * targets_export_name
#   * PROJECT_NAME
configure_package_config_file("${CMAKE_SOURCE_DIR}/cmake/Config.cmake.in"
        "${project_config}"
        INSTALL_DESTINATION "${config_install_dir}")

# Targets:
install(TARGETS delaunay
        EXPORT "${targets_export_name}"
        LIBRARY DESTINATION "lib"
        ARCHIVE DESTINATION "lib"
        RUNTIME DESTINATION "bin"
        INCLUDES DESTINATION "${include_install_dir}")

# Headers:
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/delaunay-psm"
        DESTINATION "${include_install_dir}"
        FILES_MATCHING PATTERN "*.h")

# Config
#   * <prefix>/lib/cmake/<PROJECT-NAME>/<PROJECT-NAME>Config.cmake
#   * <prefix>/lib/cmake/<PROJECT-NAME>/<PROJECT-NAME>ConfigVersion.cmake
install(FILES "${project_config}" "${version_config}"
        DESTINATION "${config_install_dir}")

# Config
#   * <prefix>/lib/cmake/<PROJECT-NAME>/<PROJECT-NAME>Targets.cmake
install(EXPORT "${targets_export_name}"
        NAMESPACE "${namespace}"
        DESTINATION "${config_install_dir}")


if(BUILD_SAMPLE)
    add_subdirectory(example)
endif()
