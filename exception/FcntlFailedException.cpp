#include "FcntlFailedException.hpp"

const char *FcntlFailedException::what(void) const throw()
{
    return "FcntlFailedException";
}


