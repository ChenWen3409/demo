# ===================================================================
# PROJECT SETUP
# ===================================================================
project(RDKDemo VERSION 0.1.0)

# ===================================================================
# PROJECT DEPENDENCIES
# ===================================================================
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread system)
find_package(spdlog REQUIRED)

# ===================================================================
# PROJECT EXECUTABLES
# ===================================================================
# add executables
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MAIN_SOURCE)
add_executable(${PROJECT_NAME} ${MAIN_SOURCE})

# add path to all header files
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/flexiv_rdk/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/flexiv_rdk/thirdparty/eigen3
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# find and link dependencies
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../thirdparty/flexiv_rdk/lib/libFlexivRdk.x86_64-linux-gnu.a
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libjsoncpp.a
    Threads::Threads
    spdlog::spdlog
    ${Boost_LIBRARIES}
)

# ===================================================================
# TEST EXECUTABLES
# ===================================================================
add_executable(test_client ${CMAKE_CURRENT_SOURCE_DIR}/test/test_client.cpp)
target_include_directories(test_client
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

# find and link dependencies
target_link_libraries(test_client
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/libjsoncpp.a
    ${Boost_LIBRARIES}
)