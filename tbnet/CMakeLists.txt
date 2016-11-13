cmake_minimum_required(VERSION 3.5)
include_directories(../tbsys/)

set(source
        databuffer.h
        ipacketfactory.h
        ipackethandler.h
        ipacketstreamer.h
        iserveradapter.h
        udpacceptor.h
        tbnet.h
        channel.cpp
        channelpool.cpp
        connection.cpp
        connectionmanager.cpp
        controlpacket.cpp
        defaultpacketstreamer.cpp
        epollsocketevent.cpp
        httppacketstreamer.cpp
        httprequestpacket.cpp
        httpresponsepacket.cpp
        iocomponent.cpp
        iocomponent.cpp
        packet.cpp
        packetqueue.cpp
        packetqueuethread.cpp
        serversocket.cpp
        socket.cpp
        socketevent.cpp
        stats.cpp
        tcpacceptor.cpp
        tcpcomponent.cpp
        tcpconnection.cpp
        transport.cpp
        udpcomponent.cpp
        udpconnection.cpp
        )

add_library(tbnet ${source})
target_link_libraries(tbnet tbsys)
