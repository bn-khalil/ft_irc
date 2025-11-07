#ifndef SOCKETLISTENFAILEDEXCEPTION_HPP
# define SOCKETLISTENFAILEDEXCEPTION_HPP

# include <exception>

class SocketListenFailedException : public std::exception
{
    public:
        virtual const char *what(void) const throw();
};

#endif
