cmake_minimum_required(VERSION 3.12)
project(MOD5_Project C)

set(CMAKE_C_STANDARD 11)

add_executable(MOD5_Project
        proofofconcept/fpgasimul.c
        proofofconcept/fpgasimulPIGPIO.c
        proofofconcept/movingdraw.c
        proofofconcept/readallgpio.c)
