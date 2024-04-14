SRC_DIR = ./src/

SRCS = main.cpp\
\
$(SRC_DIR)client.cpp\
\
./src/server.cpp\
\
$(SRC_DIR)Channel.cpp\
\
$(SRC_DIR)ManageChannel.cpp\
\
$(SRC_DIR)ManageClient.cpp\
\
$(SRC_DIR)ManageCommand.cpp\
\
$(SRC_DIR)CommunicationClient.cpp\
\
./commands/Nick.cpp\
\
./commands/Pass.cpp\
\
./commands/User.cpp\
\
./commands/Join.cpp\
\
./commands/PrivateMessage.cpp\
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