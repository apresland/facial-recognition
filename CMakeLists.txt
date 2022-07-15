cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" )
set( OpenCV_DIR  "C:/vcpkg/installed/x64-windows/share/opencv" )
set( OpenCV_BIN  "C:/vcpkg/installed/x64-windows" )

# project declaration
project(face_detection LANGUAGES CXX)
add_executable(${PROJECT_NAME} 
    src/main.cpp 
    src/face/preprocessor.cpp 
    src/face/frame_selection.cpp 
    src/face/detector.cpp 
    src/face/tracker.cpp 
    src/face/track_observer.cpp 
    src/face/multitracker.cpp  
    src/face/merging.cpp 
    src/face/annotator.cpp 
    src/face/node.cpp)

find_package(OpenCV CONFIG REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
#link_directories( ${OpenCV_LIB_DIR} )

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
target_include_directories(${PROJECT_NAME} PRIVATE include)

add_custom_command(TARGET face_detection POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_cored.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_dnnd.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_imgprocd.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_highguid.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_imgcodecsd.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_videoiod.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/zlibd1.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/libprotobufd.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/jpeg62.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/tiffd.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/libpng16d.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/webpdecoder.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/webp.dll ${CMAKE_BINARY_DIR}/Debug
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/liblzma.dll ${CMAKE_BINARY_DIR}/Debug
)

add_custom_command(TARGET face_detection POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_cored.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_dnnd.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_imgprocd.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_highguid.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_imgcodecsd.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/opencv_videoiod.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/zlibd1.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/libprotobufd.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/jpeg62.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/tiffd.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/libpng16d.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/webpdecoder.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/webp.dll ${CMAKE_BINARY_DIR}/Release
    COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_BIN}/debug/bin/liblzma.dll ${CMAKE_BINARY_DIR}/Release
)

set(FACE_DETECTION_CONFIGURATION 
    "${PROJECT_SOURCE_DIR}/assets/deploy.prototxt")

set(FACE_DETECTION_WEIGHTS 
    "${PROJECT_SOURCE_DIR}/assets/res10_300x300_ssd_iter_140000_fp16.caffemodel")

target_compile_definitions(${PROJECT_NAME} PRIVATE FACE_DETECTION_CONFIGURATION="${FACE_DETECTION_CONFIGURATION}")
target_compile_definitions(${PROJECT_NAME} PRIVATE FACE_DETECTION_WEIGHTS="${FACE_DETECTION_WEIGHTS}")






