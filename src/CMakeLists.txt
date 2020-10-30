cmake_minimum_required(VERSION 3.10)

project(Prj1)

set(SOURCES main.c ini_parser.c ini_parser.h)

add_executable(test ${SOURCES})