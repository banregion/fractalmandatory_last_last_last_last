#ifndef FRACT_H
#define FRACT_H

#define WIDTH 1024
#define HEIGHT 768
#define MAX_ITER 100
#define ESC_KEY 65307

typedef enum e_fractal_type
{
	MANDELBROT,
	JULIA
}	t_fractal_type;

typedef struct s_fractal
{
	void			*mlx;
	void			*win;
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	double			min_re;
	double			max_re;
	double			min_im;
	double			max_im;
	double			c_re;
	double			c_im;
	t_fractal_type	type;
	int				max_iter;
}	t_fractal;

int		mandelbrot(double cr, double ci, int max_iter);
int		julia(double zr, double zi, double cr, double ci, int max_iter);
int		get_color(int iter, int max_iter);
void	render_fractal(t_fractal *fract);
int		key_hook(int keycode, t_fractal *fract);
int		mouse_hook(int button, int x, int y, t_fractal *fract);
int		close_hook(t_fractal *fract);
void	print_usage(void);
int		ft_strcmp(const char *s1, const char *s2);
double	ft_atof(const char *str);
double	is_it_double(const char **s);
double	is_it_int(const char **s);

#endif