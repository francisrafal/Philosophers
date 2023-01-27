# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: frafal <frafal@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/19 11:03:02 by frafal            #+#    #+#              #
#    Updated: 2023/01/27 10:33:44 by frafal           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			:= philo
CC				:= cc
RM		    	:= rm -f
FLAGS			:= -Wall -Wextra -Werror -pthread 
DEBUG			:= -g -fsanitize=thread

HEADDIR			:= ./
HEADLIST		:= philo.h
HEADERS			:= $(addprefix ${HEADDIR}, ${HEADLIST})

SRCSDIR			:= ./
SRCSLIST		:= philo.c \
					libft.c \
					queue.c
SRCS			:= $(addprefix ${SRCSDIR}, ${SRCSLIST})

OBJSDIR			:= ./obj/
OBJSLIST		:= ${SRCSLIST:.c=.o}
OBJS			:= $(addprefix ${OBJSDIR}, ${OBJSLIST})

LIBS			:= 
INCS			:= -I${HEADDIR} 

CYAN			:= \033[0;36m
RESET			:= \033[0m

${NAME}:		${OBJSDIR} ${OBJS}
				@echo ""
				@echo "${CYAN}Compiling ${NAME} ...${RESET}"
				${CC} ${FLAGS} ${DEBUG} ${OBJS} -o ${NAME} ${LIBS} ${INCS}
				@echo ""
				@echo "${CYAN}${NAME} Created${RESET}"

${OBJSDIR}%.o:	${SRCSDIR}%.c ${HEADERS} compiling
				${CC} ${FLAGS} ${DEBUG} ${INCS} -c $< -o $@

${OBJSDIR}:
				mkdir -p ${OBJSDIR}

.PHONY:			all clean fclean re test 

.INTERMEDIATE:	compiling

compiling:
				@echo ""
				@echo "${CYAN}Compiling Objects ...${RESET}"

all:			${NAME}

clean:
				@echo ""
				@echo "${CYAN}Deleting $(NAME) Objects ...${RESET}"
				${RM} -r ${OBJSDIR}

fclean:			clean
				@echo ""
				@echo "${CYAN}Deleting $(NAME) Executable ...${RESET}"
				${RM} ${NAME}

re:				fclean all

test:			all