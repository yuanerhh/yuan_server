target("test_socket")
    add_cxflags("-g -ggdb")
    set_kind("binary")
    add_files("../TcpSocket.cpp", "test_socket.cpp")