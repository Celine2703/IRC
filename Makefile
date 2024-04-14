SRCS = irc.cpp client.cpp server.cpp Channel.cpp ManageChannel.cpp ManageClient.cpp\
\
./commands/Nick.cpp \
\
./commands/Pass.cpp \
\
./commands/User.cpp \
\
./commands/Join.cpp \
\
./commands/PrivateMessage.cpp \
\


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