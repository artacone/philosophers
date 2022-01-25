NAME		=	philo

SRC_DIR		=	./src/

OBJ_DIR		=	./obj/

HEADER_DIR	=	./include/

SRC			=	$(wildcard $(SRC_DIR)*.c)

HEADER		=	$(wildcard $(HEADER_DIR)*.h)

OBJ			=	$(addprefix $(OBJ_DIR), $(notdir $(SRC:.c=.o)))

DEP			=	$(OBJ:%.o=%.d)

CC			=	gcc

CFLAGS		=	-Wall -Wextra -Werror -MMD -pthread -O2

LDFLAGS		=	-pthread -O2
CFLAGS += -g -fsanitize=address
LDFLAGS += -g -fsanitize=address

RM			=	rm -f


all: $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c $(HEADER) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

-include $(DEP)

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

re: fclean $(OBJ_DIR) all

.PHONY: all clean fclean re