include_directories(../tbsys)

add_executable(testthread testthread.cpp testBase.cpp)
target_link_libraries(testthread tbsys)

add_executable(testconfig testconfig.cpp)
target_link_libraries(testconfig tbsys)

add_executable(testfilequeue testfilequeue.cpp)
target_link_libraries(testfilequeue tbsys)

add_executable(testlog testlog.cpp)
target_link_libraries(testlog tbsys)

add_executable(testnetutil testnetutil.cpp)
target_link_libraries(testnetutil tbsys)

add_executable(testThreadPool testThreadPool.cpp)
target_link_libraries(testThreadPool tbsys)

add_executable(testTimer testTimer.cpp)
target_link_libraries(testTimer tbsys)

add_executable(testtimeutil testtimeutil.cpp)
target_link_libraries(testtimeutil tbsys)

add_executable(testwarningbuffer testwarningbuffer.cpp)
target_link_libraries(testwarningbuffer tbsys)

add_executable(testService testService.cpp)
target_link_libraries(testService tbsys)


add_executable(teststringutil teststringutil.cpp)
target_link_libraries(teststringutil tbsys)

add_executable(testqueuethread testqueuethread.cpp)
target_link_libraries(testqueuethread tbsys)