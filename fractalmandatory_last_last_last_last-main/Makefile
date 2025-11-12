NAME = fractol_bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror -O3 -march=native
INCLUDES = -I. -I./minilibx-linux

# Libraries
MLX_DIR = ./minilibx-linux
MLX_LIB = $(MLX_DIR)/libmlx_Linux.a
LIBS = -L$(MLX_DIR) -lmlx_Linux -lXext -lX11 -lm -lpthread

# Source files
SRCS = main.c fractals.c hooks.c utils.c

# Object files
OBJS = $(SRCS:.c=.o)

# Colors for output
RED = \033[0;31m
GREEN = \033[0;32m
YELLOW = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

all: $(MLX_LIB) $(NAME)

$(MLX_LIB):
	@echo "$(YELLOW)Compiling MiniLibX...$(RESET)"
	@make -C $(MLX_DIR) > /dev/null 2>&1
	@echo "$(GREEN)MiniLibX compiled successfully!$(RESET)"

$(NAME): $(OBJS)
	@echo "$(YELLOW)Linking $(NAME)...$(RESET)"
	@$(CC) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"
	@echo "$(BLUE)Usage: ./$(NAME) [fractal_type] [julia_real] [julia_imag]$(RESET)"
	@echo "$(BLUE)Fractal types: mandelbrot, julia, burning_ship, tricorn, newton$(RESET)"

%.o: %.c
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -f $(OBJS)
	@make -C $(MLX_DIR) clean > /dev/null 2>&1

fclean: clean
	@echo "$(RED)Cleaning $(NAME)...$(RESET)"
	@rm -f $(NAME)

re: fclean all

# Test targets
test_mandelbrot:
	@./$(NAME) mandelbrot

test_julia:
	@./$(NAME) julia -0.7269 0.1889

test_burning_ship:
	@./$(NAME) burning_ship

test_tricorn:
	@./$(NAME) tricorn

test_newton:
	@./$(NAME) newton

# Help target
help:
	@echo "$(GREEN)Fractol Bonus - Advanced Fractal Explorer$(RESET)"
	@echo ""
	@echo "$(YELLOW)Compilation:$(RESET)"
	@echo "  make          - Compile the project"
	@echo "  make clean    - Remove object files"
	@echo "  make fclean   - Remove object files and executable"
	@echo "  make re       - Recompile everything"
	@echo ""
	@echo "$(YELLOW)Usage:$(RESET)"
	@echo "  ./$(NAME) mandelbrot"
	@echo "  ./$(NAME) julia [real] [imaginary]"
	@echo "  ./$(NAME) burning_ship"
	@echo "  ./$(NAME) tricorn"
	@echo "  ./$(NAME) newton"
	@echo ""
	@echo "$(YELLOW)Controls:$(RESET)"
	@echo "  WASD          - Pan around"
	@echo "  Mouse wheel   - Zoom in/out"
	@echo "  Left click    - Center on point"
	@echo "  Space         - Change fractal type"
	@echo "  C             - Change color palette"
	@echo "  Arrow keys    - Adjust Julia parameters"
	@echo "  1-9           - Jump to preset locations"
	@echo "  +/-           - Zoom in/out"
	@echo "  R             - Reset view"
	@echo "  H             - Toggle info display"
	@echo "  S             - Save image as PPM"
	@echo "  T             - Toggle animation"
	@echo "  ESC           - Exit"
	@echo ""
	@echo "$(YELLOW)Test targets:$(RESET)"
	@echo "  make test_mandelbrot   - Test Mandelbrot set"
	@echo "  make test_julia        - Test Julia set"
	@echo "  make test_burning_ship - Test Burning Ship"
	@echo "  make test_tricorn      - Test Tricorn"
	@echo "  make test_newton       - Test Newton fractal"

.PHONY: all clean fclean re test_mandelbrot test_julia test_burning_ship test_tricorn test_newton help
