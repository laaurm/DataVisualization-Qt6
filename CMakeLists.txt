cmake_minimum_required(VERSION 3.21)

project(title
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CSS_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED
COMPONENTS
    Widgets
    WebEngineWidgets
    WebChannel
) 

qt_add_executable(DataVisualization main.cpp DicomBridge.h DicomBridge.cpp)

target_link_libraries(DataVisualization
PRIVATE
    Qt6::Widgets 
    Qt6::WebEngineWidgets
    Qt6::WebChannel
) 

# Copier l'interface web à côté de l'exécutable
add_custom_command(TARGET DataVisualization POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:DataVisualization>/app" COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/app/index.html" "$<TARGET_FILE_DIR:DataVisualization>/app/index.html" )
file(COPY "${CMAKE_SOURCE_DIR}/app/"
     DESTINATION "${CMAKE_BINARY_DIR}/app/")