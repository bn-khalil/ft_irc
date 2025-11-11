#ifndef FCNTLFAILEDEXCEPTION_HPP
# define FCNTLFAILEDEXCEPTION_HPP

# include <exception>

class FcntlFailedException : public std::exception
{
    public:
        virtual const char *what(void) const throw();
};

#endif
