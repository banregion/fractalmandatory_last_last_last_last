# Fract-ol - Fractal Explorer

## 📋 Proje Hakkında

Fract-ol, 42 School müfredatının bir parçası olan grafik programlama projesidir. Bu proje, matematiksel fraktalların görselleştirilmesini sağlayan interaktif bir uygulama geliştirmeyi amaçlar. MiniLibX grafik kütüphanesi kullanılarak, kullanıcıların fraktal setleri keşfetmesine ve zoom yaparak detayları incelemesine olanak tanır.

## 🎯 Öğrenme Kazanımları

### Teknik Beceriler
- **Grafik Programlama**: MiniLibX kütüphanesi ile pencere yönetimi, görüntü oluşturma ve piksel manipülasyonu
- **Matematiksel Algoritmalar**: Kompleks sayı matematiği ve iteratif hesaplamalar
- **Event Handling**: Fare ve klavye olaylarının yönetimi
- **Bellek Yönetimi**: Dinamik bellek tahsisi ve serbest bırakma
- **Optimizasyon**: Görüntü işleme optimizasyonları

### Matematiksel Kavramlar
- **Kompleks Sayılar**: z = a + bi formatında sayılar ve işlemleri
- **İterasyonlar**: Matematiksel fonksiyonların tekrarlı uygulanması
- **Kaçış Zamanı Algoritması**: Bir noktanın sette olup olmadığını belirleme
- **Koordinat Dönüşümleri**: Ekran koordinatlarından matematiksel düzleme geçiş

### Yazılım Mühendisliği
- **Modüler Kod Yapısı**: Fonksiyonların mantıksal gruplara ayrılması
- **42 Norm Standartları**: Kod yazım kurallarına uyum
- **Hata Yönetimi**: Kullanıcı girdilerinin doğrulanması
- **Kod Okunabilirliği**: Temiz ve anlaşılır kod yazımı

## 🔬 Fraktallar ve Çalışma Mantığı

### Mandelbrot Seti

**Matematiksel Tanım:**
```
z(n+1) = z(n)² + c
z(0) = 0
```

**Çalışma Prensibi:**
1. Ekrandaki her piksel için bir kompleks sayı `c` hesaplanır
2. İterasyon başlatılır: `z = 0`
3. Her adımda: `z_new = z² + c` hesaplanır
4. Eğer |z| > 2 ise nokta sonsuzluğa kaçıyor (sette değil)
5. Kaç iterasyonda kaçtığı, rengi belirler
6. Maksimum iterasyon sayısına ulaşılırsa nokta sette kabul edilir (siyah)

**Kod İmplementasyonu:**
```c
int mandelbrot(double cr, double ci, int max_iter)
{
    double zr = 0.0, zi = 0.0;  // z = 0 + 0i
    int iter = 0;
    
    // |z|² = zr² + zi² <= 4 kontrolü (|z| <= 2)
    while (zr * zr + zi * zi <= 4.0 && iter < max_iter)
    {
        // z² = (zr + zi*i)² = zr² - zi² + 2*zr*zi*i
        double tmp = zr * zr - zi * zi + cr;
        zi = 2.0 * zr * zi + ci;
        zr = tmp;
        iter++;
    }
    return iter;
}
```

### Julia Seti

**Matematiksel Tanım:**
```
z(n+1) = z(n)² + c
z(0) = koordinat noktası
c = sabit kompleks sayı
```

**Mandelbrot'tan Farkı:**
- Mandelbrot'ta: c değişken (her piksel), z başlangıcı 0
- Julia'da: c sabit (kullanıcı parametresi), z başlangıcı değişken (her piksel)

**İlginç Julia Parametreleri:**
- c = -0.7 + 0.27i: Çift spiral
- c = -0.8 + 0.156i: Kristal yapısı
- c = 0.285 + 0.01i: Dendritik (ağaç) yapısı

## 🏗️ Proje Yapısı

### Dosya Organizasyonu

```
fract-ol/
├── fract.h          # Header dosyası (struct ve fonksiyon tanımları)
├── main.c           # Program başlangıcı ve argüman işleme
├── fractals.c       # Fraktal hesaplama fonksiyonları
├── hooks.c          # Event handler fonksiyonları
├── utils.c          # Yardımcı string fonksiyonları
├── Makefile         # Derleme kuralları
└── README.md        # Proje dokümantasyonu
```

### Kod Modülleri

#### 1. **fract.h** - Header Dosyası

**Sabitler:**
```c
#define WIDTH 1024        // Pencere genişliği (piksel)
#define HEIGHT 768        // Pencere yüksekliği (piksel)
#define MAX_ITER 100      // Maksimum iterasyon sayısı
#define ESC_KEY 65307     // ESC tuşu (Linux)
```

**Enum Tanımları:**
```c
typedef enum e_fractal_type
{
    MANDELBROT,    // Mandelbrot seti
    JULIA          // Julia seti
} t_fractal_type;
```

**Ana Veri Yapısı:**
```c
typedef struct s_fractal
{
    // MiniLibX yapıları
    void    *mlx;          // MLX bağlantısı
    void    *win;          // Pencere pointer'ı
    void    *img;          // Görüntü buffer'ı
    char    *addr;         // Piksel data adresi
    int     bpp;           // Bits per pixel
    int     line_len;      // Satır uzunluğu (byte)
    int     endian;        // Byte sıralaması
    
    // Matematiksel koordinatlar
    double  min_re;        // Minimum gerçek kısım
    double  max_re;        // Maksimum gerçek kısım
    double  min_im;        // Minimum imajiner kısım
    double  max_im;        // Maksimum imajiner kısım
    
    // Fraktal parametreleri
    double  c_re;          // Julia sabiti (gerçek)
    double  c_im;          // Julia sabiti (imajiner)
    t_fractal_type type;   // Fraktal tipi
    int     max_iter;      // Maksimum iterasyon
} t_fractal;
```

#### 2. **main.c** - Program Başlangıcı

**Fonksiyonlar:**

**`print_usage()`**: Kullanım bilgisi gösterir
- `write()` sistem çağrısı kullanır (printf yerine)
- Standart output'a (fd=1) yazar

**`init_fractal()`**: Fraktal parametrelerini başlatır
```c
static void init_fractal(t_fractal *fract)
{
    fract->min_re = -2.5;    // Sol sınır
    fract->max_re = 2.5;     // Sağ sınır
    fract->min_im = -2.0;    // Alt sınır
    // Üst sınır: Aspect ratio'yu korumak için hesaplanır
    fract->max_im = fract->min_im + 
        (fract->max_re - fract->min_re) * HEIGHT / WIDTH;
}
```

**`parse_args()`**: Komut satırı argümanlarını işler
- Fraktal tipini kontrol eder
- Julia için c_re ve c_im parametrelerini `ft_atof()` ile parse eder
- Hatalarda kullanıcıya bilgi verir

**`init_mlx()`**: MiniLibX başlatır
```c
static int init_mlx(t_fractal *fract)
{
    fract->mlx = mlx_init();                    // MLX başlat
    fract->win = mlx_new_window(...);           // Pencere oluştur
    fract->img = mlx_new_image(...);            // Image buffer oluştur
    fract->addr = mlx_get_data_addr(...);       // Piksel data adresini al
    
    // Event hook'ları kaydet
    mlx_hook(fract->win, 17, ..., close_hook, fract);    // Pencere kapatma
    mlx_key_hook(fract->win, key_hook, fract);           // Klavye
    mlx_mouse_hook(fract->win, mouse_hook, fract);       // Fare
}
```

**`main()`**: Program akışını yönetir
1. Argüman sayısını kontrol eder
2. Fraktal parametrelerini başlatır
3. Argümanları parse eder
4. MLX'i başlatır
5. İlk render'ı yapar
6. Event loop'a girer (`mlx_loop`)

#### 3. **fractals.c** - Fraktal Hesaplamaları

**`mandelbrot()`**: Mandelbrot iterasyonu
```c
int mandelbrot(double cr, double ci, int max_iter)
{
    double zr = 0.0, zi = 0.0;  // z başlangıcı
    int iter = 0;
    
    while (zr * zr + zi * zi <= 4.0 && iter < max_iter)
    {
        // Kompleks sayı karesi: (a+bi)² = a²-b² + 2abi
        double tmp = zr * zr - zi * zi + cr;  // Gerçek kısım
        zi = 2.0 * zr * zi + ci;              // İmajiner kısım
        zr = tmp;
        iter++;
    }
    return iter;  // Kaçış iterasyonu
}
```

**`julia()`**: Julia iterasyonu
```c
int julia(double zr, double zi, double cr, double ci, int max_iter)
{
    // Mandelbrot'a benzer ama z başlangıcı farklı
    // z = piksel koordinatı, c = sabit parametre
}
```

**`get_color()`**: İterasyon sayısını renge çevirir
```c
int get_color(int iter, int max_iter)
{
    if (iter == max_iter)
        return 0x000000;  // Sette olan noktalar siyah
    
    // Normalize edilmiş iterasyon (0.0 - 1.0)
    double t = (double)iter / max_iter;
    
    // Bernstein polinomları ile yumuşak renk geçişi
    int r = (int)(9 * (1 - t) * t * t * t * 255);
    int g = (int)(15 * (1 - t) * (1 - t) * t * t * 255);
    int b = (int)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    
    // RGB'yi hex formatına çevir: 0xRRGGBB
    return (r << 16) | (g << 8) | b;
}
```

**`calculate_pixel()`**: Tek bir pikseli hesaplar
```c
static void calculate_pixel(t_fractal *fract, int x, int y, double factors[2])
{
    // Ekran koordinatını kompleks düzleme dönüştür
    double c_re = fract->min_re + x * factors[0];
    double c_im = fract->max_im - y * factors[1];
    
    // Fraktal tipine göre iterasyon yap
    int iter;
    if (fract->type == MANDELBROT)
        iter = mandelbrot(c_re, c_im, fract->max_iter);
    else
        iter = julia(c_re, c_im, fract->c_re, fract->c_im, ...);
    
    // Rengi hesapla ve piksele yaz
    int color = get_color(iter, fract->max_iter);
    int pixel = y * fract->line_len + x * (fract->bpp / 8);
    *(int *)(fract->addr + pixel) = color;
}
```

**`render_fractal()`**: Tüm ekranı render eder
```c
void render_fractal(t_fractal *fract)
{
    // Koordinat dönüşüm faktörleri
    double factors[2];
    factors[0] = (fract->max_re - fract->min_re) / (WIDTH - 1);
    factors[1] = (fract->max_im - fract->min_im) / (HEIGHT - 1);
    
    // Her piksel için hesaplama
    int y = 0;
    while (y < HEIGHT)
    {
        int x = 0;
        while (x < WIDTH)
        {
            calculate_pixel(fract, x, y, factors);
            x++;
        }
        y++;
    }
    
    // Ekrana çiz
    mlx_put_image_to_window(fract->mlx, fract->win, fract->img, 0, 0);
}
```

#### 4. **hooks.c** - Event Yönetimi

**`key_hook()`**: Klavye olayları
```c
int key_hook(int keycode, t_fractal *fract)
{
    if (keycode == ESC_KEY)  // 65307 (Linux)
    {
        // Temizlik ve çıkış
        mlx_destroy_image(fract->mlx, fract->img);
        mlx_destroy_window(fract->mlx, fract->win);
        mlx_destroy_display(fract->mlx);
        free(fract->mlx);
        exit(0);
    }
    return 0;
}
```

**`close_hook()`**: Pencere kapatma
```c
int close_hook(t_fractal *fract)
{
    // X butonu ile kapatma
    // Bellekleri temizle ve çık
}
```

**`calculate_zoom()`**: Zoom hesaplamaları
```c
static void calculate_zoom(t_fractal *fract, double mouse[2], double zoom)
{
    // Mevcut görüntü boyutları
    double width = fract->max_re - fract->min_re;
    double height = fract->max_im - fract->min_im;
    
    // Yeni boyutlar (zoom faktörü ile)
    double new_width = width * zoom;
    double new_height = height * zoom;
    
    // Fare pozisyonunun oranı
    double ratio_x = (mouse[0] - fract->min_re) / width;
    double ratio_y = (mouse[1] - fract->min_im) / height;
    
    // Yeni sınırları ayarla (fare konumu merkez kalacak şekilde)
    fract->min_re = mouse[0] - new_width * ratio_x;
    fract->max_re = mouse[0] + new_width * (1.0 - ratio_x);
    fract->min_im = mouse[1] - new_height * ratio_y;
    fract->max_im = mouse[1] + new_height * (1.0 - ratio_y);
}
```

**`mouse_hook()`**: Fare scroll olayları
```c
int mouse_hook(int button, int x, int y, t_fractal *fract)
{
    // Fare pozisyonunu kompleks düzleme dönüştür
    double mouse[2];
    mouse[0] = fract->min_re + (double)x * 
               (fract->max_re - fract->min_re) / WIDTH;
    mouse[1] = fract->min_im + (double)y * 
               (fract->max_im - fract->min_im) / HEIGHT;
    
    // Button 4: Scroll up (zoom in)
    // Button 5: Scroll down (zoom out)
    if (button == 4)
        zoom_factor = 0.9;   // %10 yakınlaş
    else if (button == 5)
        zoom_factor = 1.1;   // %10 uzaklaş
    
    calculate_zoom(fract, mouse, zoom_factor);
    render_fractal(fract);  // Yeniden render et
}
```

#### 5. **utils.c** - Yardımcı Fonksiyonlar

**`ft_strcmp()`**: String karşılaştırma
```c
int ft_strcmp(const char *s1, const char *s2)
{
    int i = 0;
    // Her karakteri karşılaştır
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    // ASCII değer farkını döndür
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
```

**`is_it_int()`**: String'den tam sayı kısmını parse eder
```c
double is_it_int(const char **s)
{
    double res = 0.0;
    // Rakam karakterlerini say ve değeri hesapla
    while (**s >= '0' && **s <= '9')
    {
        res = res * 10 + (**s - '0');  // ASCII'den sayıya
        (*s)++;  // Pointer'ı ilerlet
    }
    return res;
}
```

**`is_it_double()`**: Ondalık kısmı parse eder
```c
double is_it_double(const char **s)
{
    double fraction = 0.0;
    double divisor = 1.0;
    
    while (**s >= '0' && **s <= '9')
    {
        fraction = fraction * 10 + (**s - '0');
        divisor *= 10;  // Her basamak için 10'a böl
        (*s)++;
    }
    return fraction / divisor;  // 0.xxx formatı
}
```

**`ft_atof()`**: ASCII to Float (string'den double'a)
```c
double ft_atof(const char *str)
{
    double res = 0.0;
    int sign = 1;
    
    // Whitespace'leri atla
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    
    // İşareti kontrol et
    if (*str == '-' || *str == '+')
        if (*str++ == '-')
            sign = -1;
    
    // Tam sayı kısmını parse et
    res = is_it_int(&str);
    
    // Ondalık kısmı varsa parse et
    if (*str == '.' || *str == ',')
    {
        str++;
        res += is_it_double(&str);
    }
    
    return res * sign;
}
```

## 🔢 Veri Tipleri ve Kullanımları

### Temel Tipler

| Tip | Kullanım Alanı | Açıklama |
|-----|----------------|----------|
| `int` | İterasyon sayacı, renk değerleri, piksel koordinatları | 32-bit tam sayı |
| `double` | Kompleks sayılar, koordinatlar, zoom faktörleri | 64-bit floating-point |
| `char *` | String işlemleri, piksel data | Karakter dizisi pointer'ı |
| `void *` | MLX yapıları | Generic pointer |

### Pointer Kullanımı

```c
// MLX fonksiyonları void* döndürür (tip belirsiz)
void *mlx = mlx_init();
void *win = mlx_new_window(mlx, 800, 600, "Title");

// Piksel data'ya erişim için char* cast
char *addr = mlx_get_data_addr(img, &bpp, &line_len, &endian);

// Piksele int olarak yazma (ARGB formatı)
*(int *)(addr + pixel_offset) = color;
```

### Array ve Struct

```c
// Faktör dizisi: [re_factor, im_factor]
double factors[2];

// Fare koordinatları: [mouse_re, mouse_im]
double mouse[2];

// Struct kullanımı
t_fractal fract;
fract.mlx = mlx_init();
fract.type = MANDELBROT;
```

## 🚀 Kullanım

### Derleme

```bash
make          # Projeyi derle
make clean    # Object dosyalarını temizle
make fclean   # Tüm ürünleri temizle
make re       # Yeniden derle
```

### Çalıştırma

**Mandelbrot Seti:**
```bash
./fractol mandelbrot
```

**Julia Seti:**
```bash
./fractol julia -0.7 0.27
./fractol julia -0.8 0.156
./fractol julia 0.285 0.01
```

### Kontroller

| Kontrol | Aksiyon |
|---------|---------|
| Fare Tekerleği | Zoom in/out (fare konumunda) |
| ESC | Programdan çık |
| X Butonu | Pencereyi kapat |

## 🧮 Matematiksel Detaylar

### Koordinat Dönüşümü

**Ekran → Kompleks Düzlem:**
```
c_re = min_re + (x / WIDTH) * (max_re - min_re)
c_im = max_im - (y / HEIGHT) * (max_im - min_im)
```

**Zoom Hesaplaması:**
```
new_width = old_width * zoom_factor
new_height = old_height * zoom_factor

// Fare konumu oranı
ratio_x = (mouse_re - min_re) / old_width

// Yeni sınırlar (fare merkez kalır)
min_re = mouse_re - new_width * ratio_x
max_re = mouse_re + new_width * (1 - ratio_x)
```

### Renk Algoritması

Bernstein polinomları kullanılarak yumuşak renk geçişi:

```
t = iter / max_iter  (normalize edilmiş değer)

R(t) = 9(1-t)t³ * 255
G(t) = 15(1-t)²t² * 255
B(t) = 8.5(1-t)³t * 255
```

Bu formül, t=0'dan t=1'e doğru yumuşak bir renk gradyanı oluşturur.

## ⚙️ Optimizasyon Stratejileri

### 1. Erken Çıkış (Early Exit)
```c
while (zr * zr + zi * zi <= 4.0 && iter < max_iter)
```
|z| > 2 olduğunda hemen döngüden çık.

### 2. Piksel Buffer
Image buffer kullanarak tüm pikselleri önce hesapla, sonra tek seferde ekrana bas:
```c
mlx_put_image_to_window(mlx, win, img, 0, 0);
```

### 3. Pointer Aritmetiği
Piksel adresini direkt hesapla:
```c
int pixel = y * line_len + x * (bpp / 8);
*(int *)(addr + pixel) = color;
```

## 📚 42 Norm Uyumluluğu

### Uyulan Kurallar

✅ **Fonksiyon başına maksimum 25 satır**
✅ **Dosya başına maksimum 5 fonksiyon**
✅ **Satır başına maksimum 80 karakter**
✅ **Tab kullanımı (space değil)**
✅ **Değişkenler fonksiyon başında declare edilir**
✅ **C89 standardı (C99 features yok)**
✅ **Bellek leak yok**
✅ **Norminette clean**

### Kod Organizasyonu

```
main.c      → 5 fonksiyon (init ve parsing)
fractals.c  → 5 fonksiyon (hesaplama ve render)
hooks.c     → 4 fonksiyon (event handling)
utils.c     → 4 fonksiyon (string utils)
```

## 🐛 Hata Yönetimi

### Input Validasyonu
- Argüman sayısı kontrolü
- Fraktal tipi validasyonu
- Julia parametrelerinin varlığı

### MLX Hata Kontrolü
- `mlx_init()` NULL check
- Window/image oluşturma kontrolü

### Bellek Yönetimi
- Çıkışta tüm MLX kaynakları temizlenir
- `free()` ve `mlx_destroy_*` fonksiyonları kullanılır

## 📖 Kaynaklar

- [MiniLibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx)
- [Mandelbrot Set - Wikipedia](https://en.wikipedia.org/wiki/Mandelbrot_set)
- [Julia Set - Wikipedia](https://en.wikipedia.org/wiki/Julia_set)
- [Complex Numbers](https://en.wikipedia.org/wiki/Complex_number)

## 👤 Geliştirici

42 School öğrencisi tarafından geliştirilmiştir.

## 📝 Lisans

Bu proje 42 School müfredatının bir parçasıdır.

---

**Not**: Bu proje eğitim amaçlıdır ve 42 School Fract-ol projesinin gereksinimlerini karşılamak için hazırlanmıştır.
