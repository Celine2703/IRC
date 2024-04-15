SRCS = irc.cpp client.cpp server.cpp \
\
./commands/Nick.cpp \
\
./commands/Pass.cpp \
\
./commands/User.cpp \

HEAD = channel.hpp Join.hpp Nick.hpp Part.hpp Pass.hpp Quit.hpp User.hpp

OBJS = $(SRCS:.cpp=.o)

NAME = ircserv

CC = c++ -Wall -Wextra -Werror -Iincludes -std=c++98

all: $(NAME)

$(NAME): $(OBJS) $(HEAD)
	$(CC) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re