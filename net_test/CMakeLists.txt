include_directories(../tbnet/)
include_directories(../tbsys/)

add_executable(dotest dotest.cpp)
target_link_libraries(dotest tbnet cppunit)

add_executable(echoclient echoclient.cpp)
target_link_libraries(echoclient tbnet)

add_executable(echoserver echoserver.cpp)
target_link_libraries(echoserver tbnet)

add_executable(httpserver httpserver.cpp)
target_link_libraries(httpserver tbnet)

