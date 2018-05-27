cmake_minimum_required(VERSION 3.7)
project(bitprim-dojo
        VERSION 0.1
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)

# Currency Options.
#==============================================================================
set(CURRENCY "BCH" CACHE STRING "Specify the Cryptocurrency (BCH|BTC|LTC).")

if (${CURRENCY} STREQUAL "BCH")
    add_definitions(-DBITPRIM_CURRENCY_BCH)
elseif (${CURRENCY} STREQUAL "BTC")
    add_definitions(-DBITPRIM_CURRENCY_BTC)
elseif (${CURRENCY} STREQUAL "LTC")
    add_definitions(-DBITPRIM_CURRENCY_LTC)
else()
    message(FATAL_ERROR "Invalid Cryptocurrency: ${CURRENCY}")
endif()

message(STATUS "Bitprim: Cryptocurrency: ${CURRENCY}")

# Conan infrastructure.
#==============================================================================
if (EXISTS ${CMAKE_SOURCE_DIR}/bitprimbuildinfo.cmake)
    include(${CMAKE_SOURCE_DIR}/bitprimbuildinfo.cmake)
else()
    message( STATUS "bitprimbuildinfo doent exists")
endif()

# Executable files.
#==============================================================================
add_executable(bitprim-dojo console/main.cpp)

# Libraries.
#==============================================================================
target_link_libraries(bitprim-dojo PUBLIC ${CONAN_LIBS})