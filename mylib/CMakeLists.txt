cmake_minimum_required(VERSION 3.9)
project(mylib VERSION 1.0.0 DESCRIPTION "mylib c++ example")
include(GNUInstallDirs)

find_package(cpr REQUIRED)
find_package(Boost REQUIRED)

add_library(mylib SHARED src/mylib.cpp)
target_link_libraries(mylib PRIVATE cpr::cpr)
target_link_libraries(mylib LINK_PUBLIC ${Boost_LIBRARIES})

set_target_properties(mylib PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER include/mylib.h)
configure_file(mylib.pc.in mylib.pc @ONLY)
target_include_directories(mylib PRIVATE .)
install(TARGETS mylib
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/mylib.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)