cmake_minimum_required(VERSION 3.15)

project(picScaler LANGUAGES CXX)

include(cmake/3rd.cmake)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#file(GLOB_RECURSE qrcs "qrcs/*.qrc")
file(GLOB_RECURSE srcs CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE uis "src/*.ui")
file(GLOB_RECURSE headers "include/*.h")

find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

add_executable(picScaler ${srcs} ${headers} ${uis}) # ${qrcs})
target_link_libraries(picScaler PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
target_link_libraries(picScaler PUBLIC spdlog::spdlog)

target_include_directories(picScaler PUBLIC ${PROJECT_SOURCE_DIR}/3rd/stb_image/include)
target_include_directories(picScaler PUBLIC ${PROJECT_SOURCE_DIR}/3rd/boost_1_87)
target_include_directories(picScaler PUBLIC ${PROJECT_SOURCE_DIR}/3rd/spdlog_1_15/include)
target_include_directories(picScaler PUBLIC ${PROJECT_SOURCE_DIR}/include)