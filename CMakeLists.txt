cmake_minimum_required(VERSION 2.6)
project(AlgorithmVisualiser)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(${PROJECT_SOURCE_DIR}/NSUtilLib/include)
include_directories(${PROJECT_SOURCE_DIR}/NSEngine/include)
include_directories("C:/Users/Gian Sass/Documents/NSEngine/NSUtilLib/include")
include_directories("C:/Users/Gian Sass/Documents/NSEngine/NSEngine/include")

if(WIN32)
include_directories(C:/Include)
endif()

if(WIN32)

add_definitions("-D_CRT_SECURE_NO_WARNINGS -DALGORITHMVISUALISER_EXPORTS")

endif()

if(NOT WIN32)

add_definitions("-std=c++11 -fPIC -D_LINUX")

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

if(STANDALONE)

add_library(Game ${SRC})

else()

add_library(Game SHARED ${SRC})

endif()

set(LIBRARIES
	tinyxml
)

if(NOT STANDALONE)

if(WIN32)

list(APPEND LIBRARIES "${CMAKE_SOURCE_DIR}/nsengine/lib/NSUtilLib.lib")

else()

list(APPEND LIBRARIES "${CMAKE_SOURCE_DIR}/nsengine/lib/libNSUtilLib.a")

endif()

endif()

if(STANDALONE)

target_link_libraries(Game NSUtilLib)

else()

target_link_libraries(Game ${LIBRARIES})

endif()

if(NOT WIN32)

set_target_properties(Game PROPERTIES PREFIX "")

endif()
