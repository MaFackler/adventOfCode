#include "adventviz.h"

typedef struct Light {
    V2i pos;
    V2i dir;
} Light;

static Light *lights = NULL;
static size_t *seen = NULL;


LOOP {
    (void) lights;
    (void) seen;

    if (KEY_PRESSED('s')) {
        SwapData();
    }
    if (KEY_PRESSED('l')) {
        ResetData();
        vec_clear(lights);
        vec_clear(seen);
        Light l = {0};
        l.dir.x = 1;
        append(lights, l);
    }
    if (KEY_PRESSED('r')) {
        ResetData();
        vec_clear(lights);
        vec_clear(seen);
    }

    if (TUPDATE(10)) {
        TRESET();
        Light *new_lights = NULL;
        for (int i = 0; i < len(lights); ++i) {
            Light *light = &lights[i];
            Set(light->pos.x, light->pos.y, 'l');
            size_t h = stbds_hash_bytes(light, sizeof(Light), 0);
            bool isseen = false;
            char c = GetOrig(light->pos.x, light->pos.y);
            for each(hash, seen) {
                if (*hash == h) {
                    isseen = true;
                }
            }
            if (isseen) {
                continue;
            }

            if (c == '|') {
                if (abs(light->dir.x) > 0) {
                    light->dir.x = 0;
                    light->dir.y = 1;
                    Light fork = {
                        .pos=light->pos,
                        .dir={0, -1},
                    };
                    append(new_lights, fork);
                }
            } else if (c == '-') {
                if (abs(light->dir.y) > 0) {
                    light->dir.y = 0;
                    light->dir.x = 1;
                    Light fork = {
                        .pos=light->pos,
                        .dir={-1, 0},
                    };
                    append(new_lights, fork);
                }
            } else if (c == '/') {
                if (abs(light->dir.x) > 0) {
                    light->dir.y = -light->dir.x;
                    light->dir.x = 0;
                } else {
                    light->dir.x = -light->dir.y;
                    light->dir.y = 0;
                }
            } else if (c == '\\') {
                if (abs(light->dir.x) > 0) {
                    light->dir.y = light->dir.x;
                    light->dir.x = 0;
                } else {
                    light->dir.x = light->dir.y;
                    light->dir.y = 0;
                }
            }

            append(new_lights, *light);
        }

        vec_clear(lights);
        for (int i = 0; i < len(new_lights); ++i) {
            Light *light = &new_lights[i];
            light->pos.x += light->dir.x;
            light->pos.y += light->dir.y;
            if (inrange(light->pos.x, 0, DX) && inrange(light->pos.y, 0, DY)) {
                append(lights, *light);
            }
        }
        vec_clear(new_lights);
    }


    int spacing = 2;
    for (int y = 0; y < DY; ++y) {
        for (int x = 0; x < DX; ++x) {
            int color = BLACK;
            int sx = spacing + x * DIM;
            int sy = spacing + y * DIM;
            int rdimx = DIM - spacing * 2;
            int rdimy = DIM - spacing * 2;
            char c = GetOrig(x, y);
            bool energized = Get(x, y) == 'l';
            int base_color = energized ? YELLOW : RED;
            if (c == '|') {
                color = base_color;
                rdimx *= 0.5f;
                sx += rdimx * 0.5f;
            } else if (c == '-') {
                color = base_color;
                rdimy *= 0.5f;
                sy += rdimy * 0.5f;
            } else if (c == '/') {
                color = base_color;
                rdimx *= 0.33f;
                rdimy *= 0.33f;
                sy += 2 * rdimx;
                COLOR(color);
                RECT(sx, sy, rdimx, rdimy);
                sy -= rdimy;
                sx += rdimx;
                COLOR(color);
                RECT(sx, sy, rdimx, rdimy);
                sy -= rdimy;
                sx += rdimx;
            } else if (c == '\\') {
                color = base_color;
                rdimx *= 0.33f;
                rdimy *= 0.33f;
                COLOR(color);
                RECT(sx, sy, rdimx, rdimy);
                sy += rdimy;
                sx += rdimx;
                COLOR(color);
                RECT(sx, sy, rdimx, rdimy);
                sy += rdimy;
                sx += rdimx;
            }
            if (energized) {
                color = YELLOW;
            }
            COLOR(color);
            RECT(sx, sy, rdimx, rdimy);
        }
    }

}
