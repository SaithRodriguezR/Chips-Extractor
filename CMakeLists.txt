cmake_minimum_required(VERSION 2.8.2)
project(Chips-Extractor)

add_definitions(-D UNIX)
add_definitions(-D LINUX)
add_definitions(-D _GNU_SOURCE)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_INCLUDE_CURRENT_DIR YES)

set(libs)

# Qt
set(QT_USE_QTGUI true)
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})
list(APPEND libs ${QT_LIBRARIES})

# Protobuf
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
list(APPEND libs ${PROTOBUF_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR})

protobuf_generate_cpp(PROTO_CPP PROTO_H
    src/common/proto/messages_robocup_ssl_detection.proto
    src/common/proto/messages_robocup_ssl_geometry.proto
    src/common/proto/messages_robocup_ssl_wrapper.proto
    src/common/proto/referee.proto
    src/common/proto/messages_stoxs_datamodel.proto
    src/common/proto/messages_stoxs_logfile.proto
)

set(SOURCES
    src/main.cpp
    src/chipslogsextractor.cpp
)

set(HEADERS     
    src/chipslogsextractor.h
)

qt4_wrap_cpp(HEADERS ${HEADERS})

set(srcs
    ${PROTO_CPP}
    ${PROTO_H}
    ${HEADERS}
    ${SOURCES}
)

add_executable(extractor ${srcs})
target_link_libraries(extractor ${libs})
