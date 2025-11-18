#include "InvalidPasswordException.hpp"

const char *InvalidPasswordException::what(void) const throw()
{
    return "InvalidPasswordException";
}


