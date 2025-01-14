cmake_minimum_required(VERSION 3.30)
project(RedmineHelper VERSION 1.0.0)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

set (SOURCES src/main.cpp)
set (HEADER 
	src/json_helper.h
	src/resp_helper.h
	src/app_helper.h
)

source_group("Source files/Source" FILES ${SOURCES}) 
source_group("Header files/Header" FILES ${HEADER})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADER})

if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /EHsc)
	# set(CMAKE_CXX_FLAGS /experimental:module)
endif()



find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)
find_package(cpr REQUIRED)
find_package(tgbot REQUIRED)

target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json cpr::cpr tgbot::tgbot)

set (TEST_SOURCES test/test.cpp)
set (TEST_HEADER 
	src/json_helper.h
	src/resp_helper.h
	src/app_helper.h
)
add_executable(${PROJECT_NAME}Test ${TEST_SOURCES} ${TEST_HEADER})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Test)
target_link_libraries(${PROJECT_NAME}Test nlohmann_json::nlohmann_json gtest::gtest cpr::cpr tgbot::tgbot)
