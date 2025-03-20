# stb_image 根目录
set(STB_IMAGE_ROOT "${CMAKE_SOURCE_DIR}/3rd/stb_image")
# boost 根目录
set(BOOST_ROOT "${CMAKE_SOURCE_DIR}/3rd/boost_1_87")
# 日志系统根目录
set(SPDLOG_ROOT "${CMAKE_SOURCE_DIR}/3rd/spdlog_1_15")

add_subdirectory(${STB_IMAGE_ROOT})

add_subdirectory(${SPDLOG_ROOT})
set_target_properties(spdlog
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${EDIT_ROOT}/bin
)