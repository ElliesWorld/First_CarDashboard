cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Client)

# Find the Qt6 Widgets library
#find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt6 COMPONENTS Widgets Multimedia REQUIRED)

# Include directories
include_directories(include)

# Wrap the header files for moc
qt6_wrap_cpp(MOC_SOURCES include/canvas.h include/window.h)

# Add the executable
add_executable(Client main.cpp src/canvas.cpp src/window.cpp ${MOC_SOURCES})

# Link Qt6 Widgets library
#target_link_libraries(Client Qt6::Widgets)
target_link_libraries(Client Qt6::Widgets Qt6::Multimedia)

#Ensure the sound file is copied to the output directory
file(COPY ${CMAKE_SOURCE_DIR}/res/squeaky.wav DESTINATION ${CMAKE_BINARY_DIR}/res)
file(COPY ${CMAKE_SOURCE_DIR}/res/car-blinker.wav DESTINATION ${CMAKE_BINARY_DIR}/res)
