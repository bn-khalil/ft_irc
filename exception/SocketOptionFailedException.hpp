#ifndef SOCKETOPTIONFAILEDEXCEPTION_HPP
# define SOCKETOPTIONFAILEDEXCEPTION_HPP

# include <exception>

class SocketOptionFailedException : public std::exception
{
    public:
        virtual const char *what(void) const throw();
};

#endif
