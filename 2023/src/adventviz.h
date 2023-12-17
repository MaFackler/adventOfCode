#include "advent.h"
#include <stdio.h>
#include <stdbool.h>
#include <framebuffer.h>
#include <platform.h>

typedef struct Data {
    char *current_data;
    char *current_copy;
    int current_dy;
    int current_dx;
    int current_dim;
    u64 dt;
    u64 timer;
    bool (*key_down)(char c);
    bool (*key_pressed)(char c);
    bool (*key_released)(char c);
    void (*swap_data)(void);
    void (*reset_data)(void);
    char (*get_char_orig)(int x, int y);
    char (*get_char)(int x, int y);
    void (*set_char)(int x, int y, char v);
} Data;

typedef struct V2i {
    int x;
    int y;
} V2i;


#define ResetData() data->reset_data()

#define SwapData() data->swap_data()



#define DX data->current_dx
#define DY data->current_dy
#define DIM data->current_dim

#define GetOrig(x, y) data->get_char_orig(x, y)
// #define SetOrig(x, y, v) set_char(data->copy, data->dx, data->dy, x, y, v)

#define Get(x, y) data->get_char(x, y)
#define Set(x, y, v) data->set_char(x, y, v)

#define TUPDATE(v) (data->timer >= v)
#define TRESET() data->timer = 0


#define LOOP void update(Platform *p, Framebuffer *framebuffer, Data *data)

#define RED 0xFF0000
#define GREEN 0x00FF00
#define BLUE 0x0000FF
#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define YELLOW 0xFFFF00

#define COLOR(c) framebuffer->color = c
#define RECT(x, y, w, h) framebuffer_fill_rect(framebuffer, x, y, w, h)
#define RECTC(x, y, w, h) framebuffer_fill_rect(framebuffer, x - w * 0.5f, y - h * 0.5f, w, h)
#define TRIANGLE(ax, ay, bx, by, cx, cy) framebuffer_fill_triangle(framebuffer, ax, ay, bx, by, cx, cy)

#define KEY_PRESSED(k) data->key_pressed(k)
#define KEY_DOWN(k) data->key_down(k)
#define KEY_RELEASED(k) data->key_released(k)

