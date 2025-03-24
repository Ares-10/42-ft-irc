NAME = ircserv

SRCS =  srcs/main.cpp				\
		srcs/Server.cpp				\
		srcs/Client.cpp				\
		srcs/Channel.cpp			\
		srcs/Parser.cpp				\
		srcs/Error.cpp				\
		srcs/Command.cpp			\
		srcs/command/Join.cpp		\
		srcs/command/Kick.cpp		\
		srcs/command/Mode.cpp		\
		srcs/command/Nick.cpp		\
		srcs/command/Notice.cpp		\
		srcs/command/Part.cpp		\
		srcs/command/Ping.cpp		\
		srcs/command/Pong.cpp		\
		srcs/command/PrivMsg.cpp	\
		srcs/command/Quit.cpp		\
		srcs/command/User.cpp		\
		srcs/command/Pass.cpp		\
		srcs/command/Names.cpp		\

OBJS = $(SRCS:.cpp=.o)

INC_DIR = ./includes

CPPFLAGS = 	-fsanitize=address #-Wall -Wextra -Werror -std=c++98	\



all : $(NAME)

$(NAME) : $(OBJS)
	c++ $(CPPFLAGS) $^ -o $(NAME)

%.o: %.cpp
	c++ $(CPPFLAGS) -c $< -o $@ -I$(INC_DIR)

clean :
	rm -rf $(OBJS)

fclean : clean
	rm -rf $(NAME)

re : fclean all

.PHONY : all clean fclean re