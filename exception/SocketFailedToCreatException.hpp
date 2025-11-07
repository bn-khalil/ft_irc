#ifndef SOCKETFAILEDTOCREATEXCEPTION_HPP
# define SOCKETFAILEDTOCREATEXCEPTION_HPP

# include <exception>

class SocketFailedToCreatException : public std::exception
{
    public:
        virtual const char *what(void) const throw();
};

#endif
