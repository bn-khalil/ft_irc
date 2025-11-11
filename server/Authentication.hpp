#ifndef AUTHENTICATION_HPP
# define AUTHENTICATION_HPP
# include <iostream>

class Authentication
{
    public:
        Authentication(void);
        Authentication(const Authentication& other);
        Authentication &operator=(const Authentication &other);
        ~Authentication();
};

#endif

