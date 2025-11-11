#include "fract.h"
#include "minilibx-linux/mlx.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int mandelbrot(double cr, double ci, int max_iter)
{
    double zr = 0.0, zi = 0.0;
    int iter = 0;
    
    while (zr * zr + zi * zi <= 4.0 && iter < max_iter)
    {
        double tmp = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = tmp;
        iter++;
    }
    return iter;
}

int julia(double zr, double zi, double cr, double ci, int max_iter)
{
    int iter = 0;
    
    while (zr * zr + zi * zi <= 4.0 && iter < max_iter)
    {
        double tmp = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = tmp;
        iter++;
    }
    return iter;
}

int get_color(int iter, int max_iter)
{
    if (iter == max_iter)
        return 0x000000;
    
    double t = (double)iter / max_iter;
    int r = (int)(9 * (1 - t) * t * t * t * 255);
    int g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
    int b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    
    return (r << 16) | (g << 8) | b;
}

void render_fractal(t_fractal *fract)
{
    double re_factor = (fract->max_re - fract->min_re) / (WIDTH - 1);
    double im_factor = (fract->max_im - fract->min_im) / (HEIGHT - 1);
    
    for (int y = 0; y < HEIGHT; y++)
    {
        double c_im = fract->max_im - y * im_factor;
        for (int x = 0; x < WIDTH; x++)
        {
            double c_re = fract->min_re + x * re_factor;
            int iter;
            
            if (fract->type == MANDELBROT)
                iter = mandelbrot(c_re, c_im, fract->max_iter);
            else
                iter = julia(c_re, c_im, fract->c_re, fract->c_im, fract->max_iter);
            
            int color = get_color(iter, fract->max_iter);
            int pixel = y * fract->line_len + x * (fract->bpp / 8);
            *(int *)(fract->addr + pixel) = color;
        }
    }
    
    mlx_put_image_to_window(fract->mlx, fract->win, fract->img, 0, 0);
}

int key_hook(int keycode, t_fractal *fract)
{
    if (keycode == ESC_KEY)
    {
        mlx_destroy_image(fract->mlx, fract->img);
        mlx_destroy_window(fract->mlx, fract->win);
        mlx_destroy_display(fract->mlx);
        free(fract->mlx);
        exit(0);
    }
    
    return 0;
}

int mouse_hook(int button, int x, int y, t_fractal *fract)
{
    double mouse_re = fract->min_re + (double)x * (fract->max_re - fract->min_re) / WIDTH;
    double mouse_im = fract->min_im + (double)y * (fract->max_im - fract->min_im) / HEIGHT;
    
    double zoom_factor;
    
    if (button == 4)
        zoom_factor = 0.9;
    else if (button == 5)
        zoom_factor = 1.1;
    else
        return 0;
    
    double width = (fract->max_re - fract->min_re);
    double height = (fract->max_im - fract->min_im);
    double new_width = width * zoom_factor;
    double new_height = height * zoom_factor;
    double mouse_ratio_x = (mouse_re - fract->min_re) / width;
    double mouse_ratio_y = (mouse_im - fract->min_im) / height;
    
    fract->min_re = mouse_re - new_width * mouse_ratio_x;
    fract->max_re = mouse_re + new_width * (1.0 - mouse_ratio_x);
    fract->min_im = mouse_im - new_height * mouse_ratio_y;
    fract->max_im = mouse_im + new_height * (1.0 - mouse_ratio_y);
    
    render_fractal(fract);
    return 0;
}

int close_hook(t_fractal *fract)
{
    mlx_destroy_image(fract->mlx, fract->img);
    mlx_destroy_window(fract->mlx, fract->win);
    mlx_destroy_display(fract->mlx);
    free(fract->mlx);
    exit(0);
    return 0;
}

void print_usage(void)
{
    printf("Usage: ./fractol [fractal_type] [parameters]\n\n");
    printf("Available fractals:\n");
    printf("  mandelbrot          - Display Mandelbrot set\n");
    printf("  julia [c_re] [c_im] - Display Julia set with parameters\n");
    printf("                        Example: ./fractol julia -0.7 0.27\n\n");
    printf("Controls:\n");
    printf("  Mouse wheel - Zoom in/out\n");
    printf("  ESC         - Exit program\n");
}

int main(int argc, char **argv)
{
    t_fractal fract;
    
    if (argc < 2)
    {
        print_usage();
        return 1;
    }
    
    fract.min_re = -2.5;
    fract.max_re = 2.5;
    fract.min_im = -2.0;
    fract.max_im = fract.min_im + (fract.max_re - fract.min_re) * HEIGHT / WIDTH;
    fract.c_re = -0.7;
    fract.c_im = 0.27;
    fract.max_iter = MAX_ITER;
    
    if (strcmp(argv[1], "mandelbrot") == 0)
    {
        fract.type = MANDELBROT;
    }
    else if (strcmp(argv[1], "julia") == 0)
    {
        fract.type = JULIA;
        if (argc >= 4)
        {
            fract.c_re = atof(argv[2]);
            fract.c_im = atof(argv[3]);
        }
        else
        {
            printf("Error: Julia set requires two parameters\n");
            print_usage();
            return 1;
        }
    }
    else
    {
        printf("Error: Invalid fractal type '%s'\n\n", argv[1]);
        print_usage();
        return 1;
    }
    
    fract.mlx = mlx_init();
    if (!fract.mlx)
    {
        printf("Error: Failed to initialize MLX\n");
        return 1;
    }
    
    fract.win = mlx_new_window(fract.mlx, WIDTH, HEIGHT, "Fractol - 42 Project");
    fract.img = mlx_new_image(fract.mlx, WIDTH, HEIGHT);
    fract.addr = mlx_get_data_addr(fract.img, &fract.bpp, &fract.line_len, &fract.endian);
    
    mlx_hook(fract.win, 17, 1L << 17, close_hook, &fract);
    mlx_key_hook(fract.win, key_hook, &fract);
    mlx_mouse_hook(fract.win, mouse_hook, &fract);
    
    render_fractal(&fract);
    
    mlx_loop(fract.mlx);
    return 0;
}
