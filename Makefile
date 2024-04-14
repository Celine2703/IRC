# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ranki <ranki@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/14 17:51:10 by ranki             #+#    #+#              #
#    Updated: 2024/04/14 17:54:56 by ranki            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR = ./src/

SRCS = main.cpp\
\
$(SRC_DIR)Client.cpp\
\
./src/Server.cpp\
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