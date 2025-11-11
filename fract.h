#ifndef FRACT_H
#define FRACT_H

#include <pthread.h>
#include <sys/time.h>

#define WIDTH 1024
#define HEIGHT 768
#define MAX_ITER 100
#define NUM_THREADS 8
#define NUM_PRESETS 9
#define NUM_PALETTES 6

// Key codes for Linux
#define ESC_KEY 65307
#define W_KEY 119
#define A_KEY 97
#define S_KEY 115
#define D_KEY 100
#define R_KEY 114
#define H_KEY 104
#define C_KEY 99
#define SPACE_KEY 32
#define PLUS_KEY 61
#define MINUS_KEY 45
#define UP_KEY 65362
#define DOWN_KEY 65364
#define LEFT_KEY 65361
#define RIGHT_KEY 65363

// Numpad key codes for Linux
#define NUMPAD_1 65457
#define NUMPAD_2 65458
#define NUMPAD_3 65459
#define NUMPAD_4 65460
#define NUMPAD_5 65461
#define NUMPAD_6 65462
#define NUMPAD_7 65463
#define NUMPAD_8 65464
#define NUMPAD_9 65465

typedef enum e_fractal_type
{
    MANDELBROT,
    JULIA,
    BURNING_SHIP,
    TRICORN,
    NEWTON
} t_fractal_type;

typedef enum e_color_palette
{
    PALETTE_DEFAULT,
    PALETTE_FIRE,
    PALETTE_OCEAN,
    PALETTE_PSYCHEDELIC,
    PALETTE_GRAYSCALE,
    PALETTE_RAINBOW
} t_color_palette;

typedef struct s_preset
{
    double zoom;
    double offset_x;
    double offset_y;
    double c_re;
    double c_im;
    char *name;
} t_preset;

typedef struct s_thread_data
{
    struct s_fractal *fract;
    int start_y;
    int end_y;
    int thread_id;
} t_thread_data;

typedef struct s_fractal
{
    void    *mlx;
    void    *win;
    void    *img;
    char    *addr;
    int     bpp;
    int     line_len;
    int     endian;
    double  zoom;
    double  offset_x;
    double  offset_y;
    double  min_re;
    double  max_re;
    double  min_im;
    double  max_im;
    double  c_re;
    double  c_im;
    t_fractal_type type;
    t_color_palette palette;
    int     max_iter;
    int     mouse_x;
    int     mouse_y;
    int     show_info;
    int     animate;
    double  animation_time;
    double  color_shift;
    t_preset presets[NUM_PRESETS];
    pthread_t threads[NUM_THREADS];
    t_thread_data thread_data[NUM_THREADS];
    int     fps;
    struct timeval last_time;
}	t_fractal;

// Function prototypes
int mandelbrot(double cr, double ci, int max_iter);
int julia(double zr, double zi, double cr, double ci, int max_iter);
int burning_ship(double cr, double ci, int max_iter);
int tricorn(double cr, double ci, int max_iter);
int newton(double zr, double zi, int max_iter);
int get_color(int iter, int max_iter, t_color_palette palette, double color_shift);
int get_smooth_color(double smooth_iter, t_color_palette palette, double color_shift);
int hsv_to_rgb(double h, double s, double v);
void *render_thread(void *arg);
void render_fractal(t_fractal *fract);
int key_hook(int keycode, t_fractal *fract);
int mouse_hook(int button, int x, int y, t_fractal *fract);
int mouse_move_hook(int x, int y, t_fractal *fract);
int close_hook(t_fractal *fract);
int loop_hook(t_fractal *fract);
void init_presets(t_fractal *fract);
void draw_info(t_fractal *fract);
void save_image(t_fractal *fract);
double get_time(void);
void update_fps(t_fractal *fract);

#endif