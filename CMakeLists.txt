project(TestV)
cmake_minimum_required(VERSION 3.5)

add_definitions(-DUNICODE -D_CONSOLE)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_executable(TestV ${PROJECT_SOURCE_DIR}/src/Testy.cpp)
set_target_properties(TestV PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\" /SUBSYSTEM:CONSOLE")
