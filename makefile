NAME = miniRM

CC = gcc

FLAGS = -Wall -Werror -Wextra -g

INC =

SR = ./src/
EX = $(SR)exit/
IN = $(SR)input/
IT = $(IN)init/
UT = $(IN)util/
GE = $(SR)geometry/

M  = ./MLX42/
L  = $(SR)lft/

SRC =	$(SR)main.c \
		$(SR)rayinit.c \
		$(SR)trace.c \
		$(EX)rt_exit.c \
		$(IN)rt_input.c \
		$(IT)ft_init.c \
		$(IT)init_objects.c \
		$(IT)init_scene.c \
		$(UT)rt_count.c \
		$(UT)rt_whtspc.c \
		$(GE)geo_maths.c \
		$(GE)dist.c \
#		$(GE)ray_plane.c \
		$(GE)ray_cylinder.c \
		$(GE)ray_sphere.c \

OBJ		= ${SRC:.c=.o}

all: $(NAME) $(INC)

$(NAME): $(INC) $(OBJ)
	@make bonus -C $(L)
	@make -C $(M)
	$(CC) $(FLAGS) $(OBJ) -L /Users/cdahlhof/goinfre/.brew/Cellar/glfw/3.3.7/lib -lglfw -lm $(M)libmlx42.a $(L)libft.a -o $@

e: all
	./$(NAME) scenes/basic_cylinder.rt

clean:
	rm -f $(OBJ)
	@make clean -C $(M)
	@make clean -C $(L)

$(%.o): $(%.c)
	$(CC) -o $@ -c $^

fclean: clean
	rm -f $(NAME)
	@make fclean -C $(L)

re: fclean all

.PHONY: all clean fclean re
