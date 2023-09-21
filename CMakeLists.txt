cmake_minimum_required(VERSION 3.19)
project(STComm)

set(CMAKE_CXX_STANDARD 20)

add_definitions(-w)

if (MSVC)
    set(OS "win")
    set(BOOST_INCLUDEDIR libOTe/cryptoTools/thirdparty/${OS}/include/boost-1_76)
    find_package(Boost)
else ()
    set(OS "unix")
    set(BOOST_INCLUDEDIR libOTe/cryptoTools/thirdparty/${OS}/include/)
    set(BOOST_COMPOENTS filesystem system thread atomic regex)
    add_compile_options(-maes -msse2 -msse3 -mssse3 -msse4.1 -mpclmul)
    find_package(Boost COMPONENTS REQUIRED ${BOOST_COMPOENTS})
endif ()


set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(GMP REQUIRED)
# find_package(libOTe REQUIRED)

set(MY_LIBS libOTe cryptoTools relic gmp gmpxx)

include_directories(libOTe/out/install/${PRESET_NAME}/include)
include_directories(libOTe/cryptoTools/thirdparty/${OS}/include/)
include_directories(${BOOST_INCLUDEDIR})

# link_directories(libOTe/cryptoTools/thirdparty/${OS}/lib)
link_directories(/home/original/STComm/libOTe/cryptoTools/thirdparty/unix/lib)
link_directories(libOTe/out/install/${PRESET_NAME}/lib)

# include_directories(libOTe/cryptoTools)

include_directories(osn)
include_directories(common)

add_library(osn SHARED
        osn/benes.cpp
        osn/benes.h
        osn/OSNReceiver.cpp
        osn/OSNReceiver.h
        osn/OSNSender.cpp
        osn/OSNSender.h
        osn/op.cpp
        osn/op.h
        )

target_link_libraries(osn ${MY_LIBS} ${Boost_LIBRARIES})
add_executable(test_osn
        test/test_osn.cpp)
target_link_libraries(test_osn osn ${MY_LIBS} ${Boost_LIBRARIES})