cmake_minimum_required(VERSION 3.30)
project(RedmineHelper VERSION 1.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_SUPPRESS_REGENERATION TRUE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/build/backends)

set (SOURCES src/main.cpp)
set (HEADER 
	src/json_helper.h
	src/resp_helper.h
	src/app_helper.h
)

source_group("Source files/Source" FILES ${SOURCES}) 
source_group("Header files/Header" FILES ${HEADER})

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADER})

find_package(nlohmann_json REQUIRED)
find_package(gtest REQUIRED)
find_package(cpr REQUIRED)
find_package(tgbot REQUIRED)
find_package(imgui REQUIRED)
find_package(SDL2 REQUIRED)
find_package(DirectX-Headers REQUIRED)

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




set (SOURCESG 
	src/main_gui.cpp
	src/ui/ui.cpp
	src/ui/uiwin.cpp
	src/ui/save.cpp
	build/backends/imgui_impl_sdl.cpp
	build/backends/imgui_impl_dx11.cpp
	src/ui/windata.cpp
	src/ui/win.cpp
)
set (HEADERG 
	src/json_helper.h
	src/resp_helper.h
	src/app_helper.h
	src/ui/windata.h
	src/ui/win.h
	src/ui/uiwin.h
	src/ui/save.h
	build/backends/imgui_impl_sdl.h
	build/backends/imgui_impl_dx11.h
)

source_group("Source files/private" FILES ${SOURCESG}) 
source_group("Source files/public" FILES ${HEADERG})

add_executable(${PROJECT_NAME}_imGUI WIN32 ${SOURCESG} ${HEADERG})
target_link_libraries(${PROJECT_NAME}_imGUI nlohmann_json::nlohmann_json cpr::cpr tgbot::tgbot imgui::imgui SDL2::SDL2main Microsoft::DirectX-Headers ${CONAN_LIBS} d3d11) 

