-- target("test_socket")
--     add_cxflags("-g -ggdb -DDEBUG")
--     set_kind("binary")
--     add_files("../TcpSocket.cpp", "test_socket.cpp")

target("echo_server")
    set_targetdir("./bin")
    add_cxflags("-g -ggdb -DDEBUG -fsanitize=address")
    add_ldflags("-fsanitize=address")
    add_includedirs("../")
    set_kind("binary")
    add_files("echo_server.cpp", 
    "../TcpSocket.cpp", 
    "../Channel.cpp", 
    "../EventLoop.cpp", 
    "../TcpSocket.cpp",
    "../EpollEvent.cpp",
    "../Acceptor.cpp",
    "../Connector.cpp",
    "../TcpServer.cpp",
    "../RingBuffer.cpp")