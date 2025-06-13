cmake_minimum_required(VERSION 3.27)
project(Aquitectura_de_Computadoras_1__Clark_Rivarola_Rubies_Carrizo C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Aquitectura_de_Computadoras_1__Clark_Rivarola_Rubies_Carrizo
        main.c
        password.c
        password.h
        secuencias.c
        secuencias.h
        util.c
        util.h)
