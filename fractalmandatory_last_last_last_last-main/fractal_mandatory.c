#include "fract.h"
#include "minilibx-linux/mlx.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int	mandelbrot(double cr, double ci, int max_iter)
{
	double	zr;
	double	zi;
	double	tmp;
	int		iter;

	zr = 0.0;
	zi = 0.0;
	iter = 0;
	while (zr * zr + zi * zi <= 4.0 && iter < max_iter)
	{
		tmp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = tmp;
		iter++;
	}
	return (iter);
}

int	julia(double zr, double zi, double cr, double ci, int max_iter)
{
	double	tmp;
	int		iter;

	iter = 0;
	while (zr * zr + zi * zi <= 4.0 && iter < max_iter)
	{
		tmp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = tmp;
		iter++;
	}
	return (iter);
}

int	get_color(int iter, int max_iter)
{
	double	t;
	int		r;
	int		g;
	int		b;

	if (iter == max_iter)
		return (0x000000);
	t = (double)iter / max_iter;
	r = (int)(9 * (1 - t) * t * t * t * 255);
	g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
	b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
	return ((r << 16) | (g << 8) | b);
}

static void	calculate_pixel(t_fractal *fract, int x, int y, double factors[2])
{
	double	c_re;
	double	c_im;
	int		iter;
	int		color;
	int		pixel;

	c_im = fract->max_im - y * factors[1];
	c_re = fract->min_re + x * factors[0];
	if (fract->type == MANDELBROT)
		iter = mandelbrot(c_re, c_im, fract->max_iter);
	else
		iter = julia(c_re, c_im, fract->c_re, fract->c_im, fract->max_iter);
	color = get_color(iter, fract->max_iter);
	pixel = y * fract->line_len + x * (fract->bpp / 8);
	*(int *)(fract->addr + pixel) = color;
}

void	render_fractal(t_fractal *fract)
{
	double	factors[2];
	int		x;
	int		y;

	factors[0] = (fract->max_re - fract->min_re) / (WIDTH - 1);
	factors[1] = (fract->max_im - fract->min_im) / (HEIGHT - 1);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			calculate_pixel(fract, x, y, factors);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(fract->mlx, fract->win, fract->img, 0, 0);
}

int	key_hook(int keycode, t_fractal *fract)
{
	if (keycode == ESC_KEY)
	{
		mlx_destroy_image(fract->mlx, fract->img);
		mlx_destroy_window(fract->mlx, fract->win);
		mlx_destroy_display(fract->mlx);
		free(fract->mlx);
		exit(0);
	}
	return (0);
}

static void	calculate_zoom(t_fractal *fract, double mouse[2], double zoom)
{
	double	width;
	double	height;
	double	new_width;
	double	new_height;
	double	ratio_x;
	double	ratio_y;

	width = fract->max_re - fract->min_re;
	height = fract->max_im - fract->min_im;
	new_width = width * zoom;
	new_height = height * zoom;
	ratio_x = (mouse[0] - fract->min_re) / width;
	ratio_y = (mouse[1] - fract->min_im) / height;
	fract->min_re = mouse[0] - new_width * ratio_x;
	fract->max_re = mouse[0] + new_width * (1.0 - ratio_x);
	fract->min_im = mouse[1] - new_height * ratio_y;
	fract->max_im = mouse[1] + new_height * (1.0 - ratio_y);
}

int	mouse_hook(int button, int x, int y, t_fractal *fract)
{
	double	mouse[2];
	double	zoom_factor;

	mouse[0] = fract->min_re + (double)x * (fract->max_re
			- fract->min_re) / WIDTH;
	mouse[1] = fract->min_im + (double)y * (fract->max_im
			- fract->min_im) / HEIGHT;
	if (button == 4)
		zoom_factor = 0.9;
	else if (button == 5)
		zoom_factor = 1.1;
	else
		return (0);
	calculate_zoom(fract, mouse, zoom_factor);
	render_fractal(fract);
	return (0);
}

int	close_hook(t_fractal *fract)
{
	mlx_destroy_image(fract->mlx, fract->img);
	mlx_destroy_window(fract->mlx, fract->win);
	mlx_destroy_display(fract->mlx);
	free(fract->mlx);
	exit(0);
	return (0);
}

void	print_usage(void)
{
	printf("Usage: ./fractol [fractal_type] [parameters]\n\n");
	printf("Available fractals:\n");
	printf("  mandelbrot          - Display Mandelbrot set\n");
	printf("  julia [c_re] [c_im] - Display Julia set with parameters\n");
	printf("                        ");
	printf("Example: ./fractol julia -0.7 0.27\n\n");
	printf("Controls:\n");
	printf("  Mouse wheel - Zoom in/out\n");
	printf("  ESC         - Exit program\n");
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
