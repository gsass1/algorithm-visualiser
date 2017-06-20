cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories("${PROJECT_SOURCE_DIR}/NSEngine/include")
include_directories("${PROJECT_SOURCE_DIR}/NSUtilLib/include")

if(WIN32)
	include_directories(C:/Include)
	add_definitions("-D_CRT_SECURE_NO_WARNINGS -DALGORITHMVISUALISER_EXPORTS")
else()
	add_definitions("-std=c++11 -D_LINUX")
endif()

set(SRC
	BubbleSort.cpp
	BubbleSort.h
	Button.cpp
	Button.h
	Data.cpp
	Data.h
	Game.cpp
	Game.h
	IAlgorithm.h
	InsertionSort.cpp
	InsertionSort.h
	ListButton.cpp
	ListButton.h
	QuickSort.cpp
	QuickSort.h
	RadixSort.cpp
	RadixSort.h
	Slider.cpp
	Slider.h
)

add_library(Game ${SRC})

target_link_libraries(Game NSUtilLib)

if(NOT WIN32)
	set_target_properties(Game PROPERTIES PREFIX "")
endif()
