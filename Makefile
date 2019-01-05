##
## EPITECH PROJECT, 2018
## NanoTekSpice
## File description:
## Makefile
##

NAME	=	nanotekspice

RM	=	rm -rf

CC	=	g++

CPPFLAGS=	-g3 -W -Wall -Wextra -I./inc

SRC	=	src/main.cpp 		\
		src/component.cpp 	\
		src/funcs.cpp		\
		src/get_parser_cmp.cpp	\
		src/Parse_nts.cpp 	\
		src/parser_cmp.cpp 	\
		src/Return_nts.cpp

OBJ	=	$(SRC:.cpp=.o)

all:	$(NAME)

$(NAME):	$(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

.PHONY:	re fclean clean all
