#include "SocketListenFailedException.hpp"

const char *SocketListenFailedException::what(void) const throw()
{
    return "SocketListenFailedException";
}


