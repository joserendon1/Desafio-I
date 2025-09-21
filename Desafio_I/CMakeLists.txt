cmake_minimum_required(VERSION 3.16)

project(Desafio_I LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Desafio_I main.cpp)

include(GNUInstallDirs)
install(TARGETS Desafio_I
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
