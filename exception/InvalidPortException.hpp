#ifndef INVALIDPORTEXCEPTION_HPP
# define INVALIDPORTEXCEPTION_HPP

# include <exception>

class InvalidPortException : public std::exception
{
    public:
        virtual const char *what(void) const throw();
};

#endif
