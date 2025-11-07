#ifndef SOCKETBINDFAILEDEXCEPTION_HPP
# define SOCKETBINDFAILEDEXCEPTION_HPP

# include <exception>

class SocketBindFailedException : public std::exception
{
    public:
        virtual const char *what(void) const throw();
};

#endif
