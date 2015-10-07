cmake_minimum_required(VERSION 2.8)

set (HPP_FILES
    NinfoDef.hpp NinfoBase.hpp
    NinfoBond.hpp NinfoAngl.hpp NinfoDihd.hpp
    NinfoContact.hpp NinfoAicg13.hpp NinfoAicgdih.hpp
    NinfoReader.hpp NinfoWriter.hpp NinfoSplitter.hpp
)

# set (CPP_FILES
#     NinfoLineBase.cpp NinfoBond.cpp NinfoAngl.cpp NinfoDihd.cpp
#     NinfoContact.cpp NinfoAicg13.cpp NinfoAicgdih.cpp NinfoReader.cpp
# )

add_definitions("-std=c++11 -Wall")

add_executable(tester
    NinfoTester.cpp ${HPP_FILES})

 add_executable(nsplit
    NinfoSplitter.cpp ${HPP_FILES})

# add_executable(join
#     NinfoJoin.cpp ${HPP_FILES})
