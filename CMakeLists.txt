cmake_minimum_required(VERSION 3.10.2)
project(DeepstreamApp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPLATFORM_TEGRA")
set(SRC_PATH /opt/nvidia/deepstream)
set(SRC_FOLDER ${SRC_PATH}/deepstream/deepstream-5.1/sources/apps/sample_apps/deepstream-test5)
set(SRC_INC ${SRC_PATH}/sources/apps/apps-common/includes)
set(SRC_INC2 ${SRC_PATH}/sources/includes)
set(SRC_SRC ${SRC_PATH}/sources/apps/apps-common/src)
include_directories(${PROJECT_SOURCE_DIR}
${SRC_FOLDER}
${SRC_INC}
${SRC_INC2}
${SRC_SRC}
/usr/include
/usr/include/gstreamer-1.0
/usr/include/glib-2.0
/usr/lib/aarch64-linux-gnu/glib-2.0/include
/usr/include/json-glib-1.0)
link_directories(${SRC_PATH}/lib
/usr/lib/aarch64-linux-gnu
/usr/lib/aarch64-linux-gnu/gstreamer-1.0)file(GLOB SRCS1 ${SRC_FOLDER}/*.c ${SRC_INC}/*.h ${SRC_INC2}/*.h ${SRC_SRC}/*.c)
list(REMOVE_ITEM SRCS1 "${SRC_INC}/deepstream_osd.h")
list(REMOVE_ITEM SRCS1 "${SRC_SRC}/deepstream_osd_bin.c")
file(GLOB SRCS2 ${PROJECT_SOURCE_DIR}/*.c ${PROJECT_SOURCE_DIR}/*.h)
#message("sources1 ${SRCS1}")
#message("sources2 ${SRCS2}")
add_executable(deepstreamApp ${SRCS1} ${SRCS2})
target_link_libraries(deepstreamApp -lnvdsgst_meta -lnvds_meta -lnvdsgst_helper -lnvds_utils -lnvds_msgbroker -lgstrtp-1.0 -lX11 -lm -lgstreamer-1.0 -lgstrtspserver-1.0 -lglib-2.0 -lgobject-2.0 -lgstvideo-1.0 -lnvdsgst_smartrecord -ljson-glib-1.0)