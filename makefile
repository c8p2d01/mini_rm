#OUTPUT NAME
NAME := cub3D

# Folders:
BUILD	= ./build
SOURCE	= ./src

# Other Variables:
COMPILER:=	cc
COMPFLAGS:=	-g #-fsanitize=address #-Wall -Werror -Wextra -Wno-unused-variable 
DEFINES = -D DEBUG=1

# Source Files:
SRCFILES:=\
			main.c \
			rayinit.c \
			trace.c \
			\
			input/rt_input.c \
			\
			input/init/ft_init.c \
			input/init/init_objects.c \
			input/init/init_scene.c \
			\
			input/util/rt_count.c \
			input/util/rt_whtspc.c \
			\
			exit/rt_exit.c \
			\
			geometry/dist.c \
#			geometry/ray_cylinder.c \
#			geometry/ray_plane.c \
#			geometry/ray_sphere.c \
#			geometry/geo_maths.c \
#			\



# ------------------------------------------
# Do not change anything beyond this point!
# ------------------------------------------

# Process Variables
CC:=		$(COMPILER)
CFLAGS:=	$(COMPFLAGS)
SRCS:=		$(addprefix $(SOURCE)/,$(SRCFILES))
OBJS:=		$(SRCS:$(SOURCE)/%.c=$(BUILD)/%.o)
NAME:=		./$(NAME)
OS:=		$(shell uname -s)

.PHONY: all clean fclean re e red clear green

LFT = ./ft_libft
LIBMLX = ./MLX42
LIBRARYS	:= $(LFT)/libft.a
ifeq ($(shell uname),Darwin)
	LIBRARYS += $(LIBMLX)/libmlx42.a -framework OpenGL -framework IOKit -lglfw
else ifeq ($(shell uname),Linux)
	LIBRARYS += $(LIBMLX)/libmlx42.a -pthread -lm -lglfw
endif
RED = "\033[38;2;255;51;51m"
GRN = "\033[38;2;170;255;170m"
CLEAR = "\033[0m"

ifeq ($(SUBM_STATE),)
SUBM_FLAG	= submodule
else 
SUBM_FLAG	=
endif

all: $(SUBM_FLAG) lib
	make -j $(nproc) $(NAME)

submodule: 
	@git submodule init
	@git submodule update --remote --init --recursive
	@cmake -S $(LIBMLX) -B $(LIBMLX)

lib:
	make bonus -C $(LFT)
	make -C $(LIBMLX)

# Compile .cpp files to .o Files
$(OBJS): $(BUILD)%.o : $(SOURCE)%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

# Main Build Rule
$(NAME): $(OBJS)
	@echo "--> Compiling Executable"
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) $(LIBRARYS)

clean:
	@make -s red
	rm -rdf $(BUILD)
	make clean -C $(LFT)
	make clean -C $(LIBMLX)
	@make -s clear

fclean:
	@make -s red
	rm -rdf $(NAME)
	rm -rdf $(BUILD)
	make fclean -C $(LFT)
	make clean -C $(LIBMLX)
	@make -s clear

re: fclean all

ree: re
	./$(NAME) maps/subject.cub

e:
	make
	./$(NAME) maps/subject.cub

red:
	echo $(RED)
green:
	echo $(GRN)
clear:
	echo $(CLEAR)

.PHONY: all clean fclean re e red clear green
.SILENT: red clear green
