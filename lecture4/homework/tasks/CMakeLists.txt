cmake_minimum_required(VERSION 3.16)
# Where OpenVINO is installed (the folder that has OpenVINOConfig.cmake).
# Easiest: pass it when you run cmake, for example
#   cmake .. -DOpenVINO_DIR=$(python3 -c "import openvino, os; print(os.path.join(os.path.dirname(openvino.__file__), 'cmake'))")
# Or change the default path below.
set(OpenVINO_DIR "/usr/lib/cmake/openvino2024.6.0" CACHE PATH "Folder that has OpenVINOConfig.cmake")
find_package(OpenVINO REQUIRED)

add_library(auto_buff OBJECT 
    buff_type.cpp
    buff_detector.cpp
    yolo11_buff.cpp
    buff_solver.cpp
)

target_link_libraries(auto_buff openvino::runtime ${CERES_LIBRARIES})
