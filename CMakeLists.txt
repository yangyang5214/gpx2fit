# 最低需要的 CMake 版本
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(gpx2fit C)

# 设置 C 标准为 C99
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# 包含头文件目录
include_directories(${PROJECT_SOURCE_DIR}/fit)
include_directories(${PROJECT_SOURCE_DIR}/gpxp)

# 查找 libxml2 库
find_package(LibXml2 REQUIRED)

# 设置 fit 目录中的源文件
set(FIT_SOURCES
        ${PROJECT_SOURCE_DIR}/fit/fit.c
        ${PROJECT_SOURCE_DIR}/fit/fit_convert.c
        ${PROJECT_SOURCE_DIR}/fit/fit_crc.c
        ${PROJECT_SOURCE_DIR}/fit/fit_product.c
        ${PROJECT_SOURCE_DIR}/fit/fit_ram.c
)

# 设置 gpxp 目录中的源文件
set(GPXP_SOURCES
        ${PROJECT_SOURCE_DIR}/gpxp/gpxp.c
)

# 指定源文件
set(SOURCES
        ${PROJECT_SOURCE_DIR}/src/main.c
        ${FIT_SOURCES}
        ${GPXP_SOURCES}
)

# 生成可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 链接 libxml2 库
target_link_libraries(${PROJECT_NAME} ${LIBXML2_LIBRARIES})

# 包含 libxml2 头文件目录
include_directories(${LIBXML2_INCLUDE_DIR})
