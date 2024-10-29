#pragma once

#include <fcntl.h>
#include <errno.h>
#include <cstdio>

class CNonCoybale 
{
protected:
    CNonCoybale() = default;
    ~CNonCoybale() = default;

    CNonCoybale(const CNonCoybale&) = delete;
    CNonCoybale& operator=(const CNonCoybale&) = delete;
};

inline bool is_valid_fd(int fd) 
{
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}