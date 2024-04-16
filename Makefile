# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ranki <ranki@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/14 17:51:10 by ranki             #+#    #+#              #
#    Updated: 2024/04/16 19:10:07 by ranki            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_DIR = ./src/

CMD_DIR = ./commands/

SRCS = main.cpp\
\
\
\
$(SRC_DIR)Client.cpp\
\
$(SRC_DIR)Server.cpp\
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
\
\
$(CMD_DIR)Nick.cpp\
\
$(CMD_DIR)Pass.cpp\
\
$(CMD_DIR)User.cpp\
\
$(CMD_DIR)Join.cpp\
\
$(CMD_DIR)PrivateMessage.cpp\
\
$(CMD_DIR)Topic.cpp\
\
$(CMD_DIR)Kick.cpp\
\
$(CMD_DIR)Part.cpp\
\
$(CMD_DIR)Quit.cpp\
\
$(CMD_DIR)Ping.cpp\
\

HEAD = ./includes/Channel.hpp ./includes/Replies.hpp ./includes/Client.hpp ./includes/Server.hpp

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