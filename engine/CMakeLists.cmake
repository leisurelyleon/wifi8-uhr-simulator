cmake_minimum_required(VERSION 3.14)
project(WiFi8_UHR_Engine VERSION 1.0.0 LANGUAGES CXX)

# Enforce Modern C++ Standards (Required for std::optional, std::mutex, etc.)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Threading library (critical for our UHR cluster and ML engine concurrency)
find_package(Threads REQUIRED)

# Specify where our blueprint headers live
include_directories(${CMAKE_SOURCE_DIR}/include)

# Gather all the source files your just meticulously transcribed
set(SOURCES
    src/ap_coordination/MapcController.cpp
    src/ap_coordination/SpatialReuse.cpp
    src/ml_predictor/InterferenceEngine.cpp
    src/ml_predictor/JitterBuffer.cpp
    src/phy_mac/SubThzBridge.cpp
    src/phy_mac/FrameAggregator.cpp
    src/main.cpp
)

# Define the final executable name
add_executable(wifi8_engine ${SOURCES})

# Link the threading libraries to our executable
target_link_libraries(wifi8_engine PRIVATE Threads::Threads)

# Aggressive compiler optimizations for high-speed simulation
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Configuring GCC/Clang optimization flags...")
    target_compile_options(wifi8_engine PRIVATE -03 -Wall -Wextra -pthread)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "Configuring MSVC optimization flags...")
    target_compile_options(wifi8_engine PRIVATE /02 /W4)
endif()
