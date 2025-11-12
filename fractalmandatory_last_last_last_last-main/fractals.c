#include "fract.h"
#include "minilibx-linux/mlx.h"

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
