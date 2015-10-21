cmake_minimum_required(VERSION 2.8)

# set (CPP_FILES
#     NinfoLineBase.cpp NinfoBond.cpp NinfoAngl.cpp NinfoDihd.cpp
#     NinfoContact.cpp NinfoAicg13.cpp NinfoAicgdih.cpp NinfoReader.cpp
# )

add_definitions("-std=c++11 -O3 -Wall")

add_executable(tester
    NinfoTester.cpp)

 add_executable(nsplit
    NinfoSplitter.cpp)

# add_executable(join
#     NinfoJoin.cpp ${HPP_FILES})
