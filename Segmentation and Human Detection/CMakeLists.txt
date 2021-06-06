cmake_minimum_required(VERSION 2.8)
project(ImageAnalysis )
find_package( OpenCV REQUIRED )
add_custom_target(build-time-make-directory ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory bin)
file(GLOB SOURCES
    src/*.h
    src/*.cpp
)
add_executable(ImageAnalysis ${SOURCES})
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(ImageAnalysis PUBLIC OpenMP::OpenMP_CXX)
endif()
target_link_libraries(ImageAnalysis ${OpenCV_LIBS} )