#ifndef INVALIDPASSWORDEXCEPTION_HPP
# define INVALIDPASSWORDEXCEPTION_HPP

# include <exception>

class InvalidPasswordException : public std::exception
{
    public:
        virtual const char *what(void) const throw();
};

#endif
