cmake_minimum_required(VERSION 3.13)
project(ADAS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_RPATH "/usr/local/lib")
set(CMAKE_INSTALL_RPATH "/usr/local/lib")
set(CMAKE_C_FLAGS "Wno-deprecated" "Wdeprecated-declarations")

add_executable(${PROJECT_NAME} 
	src/main.cpp

	src/lib/barcode/barcode.h
	src/lib/barcode/barcode.cpp

	src/lib/keyboard/keyboard.h
	src/lib/keyboard/keyboard.cpp

	src/lib/lcdi2c/lcdi2c.h
	src/lib/lcdi2c/lcdi2c.cpp

	src/lib/log/log.h
	src/lib/log/log.cpp

	src/lib/prog/prog.h
	src/lib/prog/prog.cpp
	)

set(LIB_DIRECTORY "src/lib/barcode" "src/lib/keyboard" "src/lib/lcd" "src/lib/log")

target_include_directories(${PROJECT_NAME} PRIVATE ${LIB_DIRECTORY})

find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)
include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE mongo::bsoncxx_shared)

target_link_libraries(${PROJECT_NAME} PRIVATE mongo::mongocxx_shared)

target_link_libraries(${PROJECT_NAME} PRIVATE wiringPi)