#pragma once

#include <fcntl.h>
#include <errno.h>
#include <cstdio>

bool is_valid_fd(int fd) 
{
    return fcntl(fd, F_GETFD) != -1 || errno != EBADF;
}