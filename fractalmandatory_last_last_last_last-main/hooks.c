#include "fract.h"
#include "minilibx-linux/mlx.h"
#include <stdlib.h>

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

int	close_hook(t_fractal *fract)
{
	mlx_destroy_image(fract->mlx, fract->img);
	mlx_destroy_window(fract->mlx, fract->win);
	mlx_destroy_display(fract->mlx);
	free(fract->mlx);
	exit(0);
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
