#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
#include "channel.hpp"
#include <map>

class Server
{

	public:

		Server();
		Server( Server const & src );
		~Server();

		Server &		operator=( Server const & rhs );
	void join(std::vector<std::string> cmds, Client *c);
	void sendReply(Client *c, std::string message);
	std::vector<std::string> splite_coma(std::string &strr, char d);

	private:
		std::map<std::string,Channel*> channel;
};

std::ostream &			operator<<( std::ostream & o, Server const & i );

#endif /* ********************************************************** SERVER_H */