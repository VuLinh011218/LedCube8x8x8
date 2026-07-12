# File: CMakeLists.txt (nằm ngoài cùng dự án)
cmake_minimum_required(VERSION 3.5)

# Báo cho hệ thống biết các thư viện nằm trong folder "app"
set(EXTRA_COMPONENT_DIRS app)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(LED_NODE_RED)

