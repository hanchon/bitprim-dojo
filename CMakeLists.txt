cmake_minimum_required(VERSION 3.6)
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
else ()
    message(FATAL_ERROR "Invalid Cryptocurrency: ${CURRENCY}")
endif ()

message(STATUS "Bitprim: Cryptocurrency: ${CURRENCY}")

# Conan infrastructure.
#==============================================================================
if (EXISTS ${CMAKE_SOURCE_DIR}/bitprimbuildinfo.cmake)
    include(${CMAKE_SOURCE_DIR}/bitprimbuildinfo.cmake)
else ()
    message(STATUS "bitprimbuildinfo doent exists")
endif ()

# Common cpp/ipp files for the example and cli.
#==============================================================================
set (SRC_FILES
        src/wallet/wallet_functions.cpp
        src/simple_web_client/requester.cpp
        src/wallet/transaction_functions.cpp
        include/utils/utility.ipp
        src/utils/output.cpp
        src/utils/script.cpp
        src/utils/encoding.cpp
        src/utils/input.cpp
        src/utils/transaction.cpp
        src/utils/point.cpp
        src/utils/ec_private.cpp
        src/utils/endorsement.cpp)


# BIPTIRM EXAMPLE EXE/INCLUDES/LIBS.
#==============================================================================

# Executable files.
#==============================================================================
add_executable(bitprim-dojo dojo/main.cpp
        ${SRC_FILES})

# Includes.
#==============================================================================
target_include_directories(bitprim-dojo PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# Libraries.
#==============================================================================
target_link_libraries(bitprim-dojo PUBLIC ${CONAN_LIBS})


# BIPTIRM CLI EXE/INCLUDES/LIBS.
#==============================================================================

# Executable files.
#==============================================================================
add_executable(bitprim-cli cli/main.cpp
        ${SRC_FILES}
        cli/process_requests.hpp)

# Includes.
#==============================================================================
target_include_directories(bitprim-cli PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

# Libraries.
#==============================================================================
target_link_libraries(bitprim-cli PUBLIC ${CONAN_LIBS})