CC		= gcc
CFLAGS	= -Wall -Wextra -Werror -g
SRCS	= ${wildcard *.c}
OBJS	= ${SRCS:.c=.o}
TARGET	= run.exe
all : ${TARGET}

%.o : %.c
	${CC} ${CFLAGS} -c $< 

${TARGET} : ${OBJS}
	${CC} ${CFLAGS} -o $@ $^

clean :
	rm -f ${OBJS}

fclean : clean
	rm -f ${TARGET}

re : fclean all