NAME = cub3d

COMPILER = cc

FLAGS = -Wall -Werror -Wextra -I Libft/includes -I ./include

# Librerias del sistema que le hace falta a MLX42 
LIB_SYS = -ldl -lglfw -pthread -lm

LIBFT_PATH = ./Libft
LIBFT = $(LIBFT_PATH)/libft.a

# Librería y Makefile MLX42
MLX42_PATH = ./MLX42/build
MLX42 = $(MLX42_PATH)/libmlx42.a
MLX42MAKEFILE = $(MLX42_PATH)/Makefile

SRC_DIR = src
SRC_PA_PATH = $(SRC_DIR)/Persona_A
SRC_PB_PATH = $(SRC_DIR)/Persona_B

# En SRC_PA es donde pones todos los archivos que codees, F.E. "$(SRC_PA_PATH)/main.c"
SRC_PA = $(SRC_PA_PATH)/main.c

SRC_PB = $(SRC_PB_PATH)/main.c

FILES = $(SRC_PA) $(SRC_PB)

# Esto está configurado para que al crear los objetos ".o" se creen en una carpeta propia y no se llena todo de basura
OBJ_DIR = obj
OBJECTS = $(FILES:%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJECTS) $(MLX42) $(LIBFT)
	$(COMPILER) $(FLAGS) $(OBJECTS) $(MLX42) $(LIBFT) $(LIB_SYS) -o $(NAME)

# Aquí es donde se crea la carpeta para los ".o" si la carpeta no existe de antes
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(COMPILER) $(FLAGS) -c $< -o $@

$(LIBFT):
	make -s -C $(LIBFT_PATH)

$(MLX42): mlx42
	make -s -C $(MLX42_PATH)

# Regla Valgrind para checkear leaks
valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

# 1.- Aquí es donde se abre un condicional en que dice "si no existe el directorio MLX42, hazme un gitclone de este enlace" y el fi cierra la condición.
# 2.- El siguiente es lo mismo pero con el Makefile de MLX42, ya que si no está entonces no está configurado y se mete en la carpeta para hacerlo.
mlx42:
	@if [ ! -d "./MLX42" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git; \
	fi
	@if [ ! -f "$(MLX42MAKEFILE)" ]; then \
		cd MLX42 && cmake -B build && cmake --build build -j4; \
	fi

clean: 
	@rm -rf $(OBJ_DIR)
	-@make clean -s -C $(MLX42_PATH)
	-@make clean -s -C $(LIBFT_PATH)

fclean: clean
	@rm -rf $(NAME) $(MLX42_PATH)
	-@make fclean -s -C $(LIBFT_PATH)

re: fclean all

.PHONY: all valgrind mlx42 clean fclean re