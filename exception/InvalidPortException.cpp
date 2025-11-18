#include "InvalidPortException.hpp"

const char *InvalidPortException::what(void) const throw()
{
    return "InvalidPortException";
}


