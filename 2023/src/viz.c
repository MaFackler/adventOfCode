#include <stdio.h>
#include <platform.h>
#include <shared.h>
#include <framebuffer.h>
#include "advent.h"
#include "adventviz.h"
#include <time.h>



u64 get_ticks() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC_RAW, &now);
    u64 res = ((u64) now.tv_sec * 1000) + ((u64) now.tv_nsec / 1000000);
    return res;
}

static Platform *p = NULL;
static Data *d = NULL;
static char *small_data = NULL;
static char *small_copy = NULL;
static int small_dx = 0;
static int small_dy = 0;
static int small_dim = 0;
static char *large_data = NULL;
static char *large_copy = NULL;
static int large_dx = 0;
static int large_dy = 0;
static int large_dim = 0;
static bool large_used = false;

void reset_data(void) {
    memcpy(d->current_data, d->current_copy, sizeof(char) * d->current_dy * d->current_dx); \
}

void set_char(int x, int y, char c) {
    if (inrange(x, 0, d->current_dx) && inrange(y, 0, d->current_dy)) {
        d->current_data[y * d->current_dx + x] = c;
    }
}

char get_char(int x, int y) {
    if (inrange(x, 0, d->current_dx) && inrange(y, 0, d->current_dy)) {
        return d->current_data[y * d->current_dx + x];
    }
    return 0;
}

char get_char_orig(int x, int y) {
    if (inrange(x, 0, d->current_dx) && inrange(y, 0, d->current_dy)) {
        return d->current_copy[y * d->current_dx + x];
    }
    return 0;
}

bool key_down(char c) {
    return platform_key_down(p, c);
}

bool key_released(char c) {
    return platform_key_released(p, c);
}

bool key_pressed(char c) {
    return platform_key_pressed(p, c);
}

void swap_data(void) {
    print("SWAP\n");
    large_used = !large_used;
    d->current_data = large_used ? large_data : small_data;
    d->current_copy = large_used ? large_copy : small_copy;
    d->current_dx = large_used ? large_dx : small_dx;
    d->current_dy = large_used ? large_dy : small_dy;
    d->current_dim = large_used ? large_dim : small_dim;
}

int main() {

    const int width = 1600;
    const int height = 900;
    p = platform_create();
    PlatformWindow window = platform_window_open(p, 0, 0, width, height);
    bool quit = false;
    char *reload_file = "build/reload.so";
    SharedLib lib = {0};
    shared_lib_open(&lib, reload_file);
    void (*update)(Platform *p, Framebuffer *, Data *) = shared_lib_get_function(&lib, "update");
    Framebuffer *framebuffer = framebuffer_create_external(window.pixels, window.width, window.height);

    char *line = NULL;
    FILE *fp = fopen("data/day16e.txt", "r");
    while (readlinefp(&line, fp)) {
        for (int i = 0; i < strlen(line); ++i) {
            append(small_data, line[i]);
        }
        small_dx = MAX(small_dx, strlen(line));
        small_dy++;
    }
    fclose(fp);

    fp = fopen("data/day16.txt", "r");
    while (readlinefp(&line, fp)) {
        for (int i = 0; i < strlen(line); ++i) {
            append(large_data, line[i]);
        }
        large_dx = MAX(large_dx, strlen(line));
        large_dy++;
    }
    fclose(fp);

    small_copy = malloc(sizeof(char) * small_dx * small_dy);
    memcpy(small_copy, small_data, sizeof(char) * small_dx * small_dy);

    large_copy = malloc(sizeof(char) * large_dx * large_dy);
    memcpy(large_copy, large_data, sizeof(char) * large_dx * large_dy);

    small_dim = height / small_dx;
    large_dim = height / large_dx;

    d = (Data *) malloc(sizeof(Data));
    *d = (Data) {
        .current_data=small_data,
        .current_copy=small_copy,
        .current_dx=small_dx,
        .current_dy=small_dy,
        .current_dim=small_dim,
        .key_pressed=key_pressed,
        .key_down=key_down,
        .key_released=key_released,
        .get_char=get_char,
        .set_char=set_char,
        .get_char_orig=get_char_orig,
        .swap_data=swap_data,
        .reset_data=reset_data,
        .timer=0,
        .dt=0,
    };
    u64 ticks = get_ticks();

    while (!quit) {
        platform_begin(p);
        if (platform_key_pressed(p, 'Q')) {
            quit = true;
        }
        framebuffer_clean(framebuffer, 0xFF1a1a1a);
        assert(framebuffer);
        if (update)
            update(p, framebuffer, d);

        if (shared_lib_check_and_reload(&lib)) {
            shared_lib_get_function(&lib, "update");
            assert(framebuffer);
        }
        platform_window_render(p, &window);
        platform_end(p);
        u64 new_ticks = get_ticks();
        d->dt = new_ticks - ticks;
        d->timer += d->dt;
        ticks = new_ticks;
    }
}
