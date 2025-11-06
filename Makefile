NAME = ircserv

FLAG = -Wall -Werror -Wextra -std=c++98

HEADER = server/Authentication.hpp server/Client.hpp server/Server.hpp

CPP = c++

SERVER_SRC = server/Authentication.cpp server/Client.cpp server/Server.cpp main.cpp
CMD_SRC =

SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CMD_OBJ = $(CMD_SRC:.cpp=.o)

OBJ = $(SERVER_OBJ) $(CMD_OBJ)

all: $(NAME)

$(NAME): $(OBJ)
	$(CPP) $(FLAG) $(OBJ) -o $(NAME)

%.o: %.cpp $(HEADER)
	$(CPP) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
