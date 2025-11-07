#include "SocketBindFailedException.hpp"

const char *SocketBindFailedException::what(void) const throw()
{
    return "SocketBindFailedException";
}


