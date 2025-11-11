#include "SocketOptionFailedException.hpp"

const char *SocketOptionFailedException::what(void) const throw()
{
    return "SocketOptionFailedException";
}


