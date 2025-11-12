#include "fract.h"
#include "minilibx-linux/mlx.h"
#include <unistd.h>

void	print_usage(void)
{
	write(1, "Usage: ./fractol [fractal_type] [parameters]\n\n", 47);
	write(1, "Available fractals:\n", 20);
	write(1, "  mandelbrot          - Display Mandelbrot set\n", 47);
	write(1, "  julia [c_re] [c_im] - Display Julia set with parameters\n", 58);
	write(1, "                        ", 24);
	write(1, "Example: ./fractol julia -0.7 0.27\n\n", 37);
	write(1, "Controls:\n", 10);
	write(1, "  Mouse wheel - Zoom in/out\n", 28);
	write(1, "  ESC         - Exit program\n", 29);
}

static void	init_fractal(t_fractal *fract)
{
	fract->min_re = -2.5;
	fract->max_re = 2.5;
	fract->min_im = -2.0;
	fract->max_im = fract->min_im + (fract->max_re
			- fract->min_re) * HEIGHT / WIDTH;
	fract->c_re = -0.7;
	fract->c_im = 0.27;
	fract->max_iter = MAX_ITER;
}

static int	parse_args(int argc, char **argv, t_fractal *fract)
{
	if (ft_strcmp(argv[1], "mandelbrot") == 0)
		fract->type = MANDELBROT;
	else if (ft_strcmp(argv[1], "julia") == 0)
	{
		fract->type = JULIA;
		if (argc >= 4)
		{
			fract->c_re = ft_atof(argv[2]);
			fract->c_im = ft_atof(argv[3]);
		}
		else
		{
			printf("Error: Julia set requires two parameters\n");
			print_usage();
			return (1);
		}
	}
	else
	{
		printf("Error: Invalid fractal type '%s'\n\n", argv[1]);
		print_usage();
		return (1);
	}
	return (0);
}

static int	init_mlx(t_fractal *fract)
{
	fract->mlx = mlx_init();
	if (!fract->mlx)
	{
		printf("Error: Failed to initialize MLX\n");
		return (1);
	}
	fract->win = mlx_new_window(fract->mlx, WIDTH, HEIGHT,
			"Fractol - 42 Project");
	fract->img = mlx_new_image(fract->mlx, WIDTH, HEIGHT);
	fract->addr = mlx_get_data_addr(fract->img, &fract->bpp,
			&fract->line_len, &fract->endian);
	mlx_hook(fract->win, 17, 1L << 17, close_hook, fract);
	mlx_key_hook(fract->win, key_hook, fract);
	mlx_mouse_hook(fract->win, mouse_hook, fract);
	return (0);
}

int	main(int argc, char **argv)
{
	t_fractal	fract;

	if (argc < 2)
	{
		print_usage();
		return (1);
	}
	init_fractal(&fract);
	if (parse_args(argc, argv, &fract))
		return (1);
	if (init_mlx(&fract))
		return (1);
	render_fractal(&fract);
	mlx_loop(fract.mlx);
	return (0);
}
