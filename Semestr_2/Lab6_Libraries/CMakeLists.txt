cmake_minimum_required(VERSION 3.10) # Мин версия, что нам нужна
project(StudentSort) # Обзываем проект
set(CMAKE_C_STANDARD 11) # Берем эту версию, чтобы точно все работало

option(BUILD_STATIC "Build static library" ON) # По умолчанию статическая
option(BUILD_SHARED "Build shared library" OFF) # Динамическая выключена

#Статическая память — как полка в шкафу: её размер не меняется, но всё под рукой.
#Динамическая память — как склад: можно брать и возвращать место по мере надобности, но нужно следить за порядком.

# Создаем библиотеку в зависимости от флага
if(BUILD_STATIC)
    add_library(student STATIC student_utils.c sort_algs.c)
    set(LIB_TYPE "STATIC")
elseif(BUILD_SHARED)
    add_library(student SHARED student_utils.c sort_algs.c)
    set(LIB_TYPE "SHARED")
endif()

add_executable(student_program main.c) # Основная программа
target_include_directories(student_program PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}) # Подключение текущей директории с заголовками
target_link_libraries(student_program student) # Линковка библиотеки `student` к программе
message(STATUS "Building ${LIB_TYPE} library") # Сообщение о типе собираемой библиотеки