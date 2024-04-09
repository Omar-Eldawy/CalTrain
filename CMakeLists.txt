cmake_minimum_required(VERSION 3.27)
project(CalTrain C)

set(CMAKE_C_STANDARD 11)

add_executable(CalTrain
        caltrain-runner.c
        caltrain.c)
