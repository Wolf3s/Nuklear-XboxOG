#include "canvas.h"
#include "..\graphics.h"
#include "..\renderer.h"

#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#include "..\nuklear.h"

/* design-time reference size */
#define CANVAS_DESIGN_W 500.0f
#define CANVAS_DESIGN_H 400.0f

/* make it taller than drawing area so scrollbars appear */
#define CANVAS_VIRTUAL_H 520.0f

static inline float scale_value(float value, float scale)
{
    return value * scale;
}

static inline float to_x(float px, float origin_x, float scale)
{
    return origin_x + scale_value(px, scale);
}

static inline float to_y(float py, float origin_y, float scale)
{
    return origin_y + scale_value(py, scale);
}

void canvas::render()
{
    struct nk_context* context = renderer::get_context();
    struct nk_font*    font    = renderer::get_font();

    if (nk_begin(context, "Canvas", nk_rect(10, 10, (float)500, (float)400), NK_WINDOW_TITLE | NK_WINDOW_BORDER | NK_WINDOW_SCALABLE | NK_WINDOW_MOVABLE | NK_WINDOW_MINIMIZABLE | NK_WINDOW_SCROLL_AUTO_HIDE))
    {
        /* 1. Determine available content area */
        struct nk_rect total_space = nk_window_get_content_region(context);

        /* 2. Compute per-axis scales and use a uniform scale */
        float scale_x = total_space.w / CANVAS_DESIGN_W;
        float scale_y = total_space.h / CANVAS_DESIGN_H;
        float scale_s = (scale_x < scale_y ? scale_x : scale_y);

        /* 3. Use a virtual height bigger than visible to enable scrollbars */
        nk_layout_row_dynamic(context, CANVAS_VIRTUAL_H * scale_s, 1);
        nk_widget(&total_space, context);

        /* 4. Get painter and scroll offsets */
        struct nk_command_buffer* painter = nk_window_get_canvas(context);
        nk_uint scroll_x = 0, scroll_y = 0;
        nk_window_get_scroll(context, &scroll_x, &scroll_y);

        float origin_x = total_space.x - (float)scroll_x;
        float origin_y = total_space.y - (float)scroll_y;

        // 5. Drawing calls
        nk_fill_rect(painter, nk_rect(
            to_x(15, origin_x, scale_s), to_y(15, origin_y, scale_s),
            scale_value(210, scale_s), scale_value(210, scale_s)),
            scale_value(5, scale_s), nk_rgb(247,230,154));

        nk_fill_rect(painter, nk_rect(
            to_x(20, origin_x, scale_s), to_y(20, origin_y, scale_s),
            scale_value(200, scale_s), scale_value(200, scale_s)),
            scale_value(5, scale_s), nk_rgb(188,174,118));

        nk_draw_text(painter, nk_rect(
            to_x(30, origin_x, scale_s), to_y(30, origin_y, scale_s),
            scale_value(150, scale_s), scale_value(20, scale_s)),
            "Text to draw", 12, &font->handle, nk_rgb(188,174,118), nk_rgb(0,0,0));

        nk_fill_rect(painter, nk_rect(
            to_x(250, origin_x, scale_s), to_y(20, origin_y, scale_s),
            scale_value(100, scale_s), scale_value(100, scale_s)),
            0, nk_rgb(0,0,255));

        nk_fill_circle(painter, nk_rect(
            to_x(20, origin_x, scale_s), to_y(250, origin_y, scale_s),
            scale_value(100, scale_s), scale_value(100, scale_s)), nk_rgb(255,0,0));

        nk_fill_triangle(painter,
            to_x(250, origin_x, scale_s), to_y(250, origin_y, scale_s),
            to_x(350, origin_x, scale_s), to_y(250, origin_y, scale_s),
            to_x(300, origin_x, scale_s), to_y(350, origin_y, scale_s),
            nk_rgb(0,255,0));

        nk_fill_arc(painter,
            to_x(300, origin_x, scale_s), to_y(420, origin_y, scale_s),
            scale_value(50, scale_s), 0, 3.141592654f * 3.0f / 4.0f, nk_rgb(255,255,0));

        float points1[12] = {
            to_x(200, origin_x, scale_s), to_y(250, origin_y, scale_s),
            to_x(250, origin_x, scale_s), to_y(350, origin_y, scale_s),
            to_x(225, origin_x, scale_s), to_y(350, origin_y, scale_s),
            to_x(200, origin_x, scale_s), to_y(300, origin_y, scale_s),
            to_x(175, origin_x, scale_s), to_y(350, origin_y, scale_s),
            to_x(150, origin_x, scale_s), to_y(350, origin_y, scale_s)
        };
        nk_fill_polygon(painter, points1, 6, nk_rgb(0,0,0));

        float points2[12] = {
            to_x(200, origin_x, scale_s), to_y(370, origin_y, scale_s),
            to_x(250, origin_x, scale_s), to_y(470, origin_y, scale_s),
            to_x(225, origin_x, scale_s), to_y(470, origin_y, scale_s),
            to_x(200, origin_x, scale_s), to_y(420, origin_y, scale_s),
            to_x(175, origin_x, scale_s), to_y(470, origin_y, scale_s),
            to_x(150, origin_x, scale_s), to_y(470, origin_y, scale_s)
        };
        nk_stroke_polygon(painter, points2, 6, scale_value(4, scale_s), nk_rgb(0,0,0));

        float points3[8] = {
            to_x(250, origin_x, scale_s), to_y(200, origin_y, scale_s),
            to_x(275, origin_x, scale_s), to_y(220, origin_y, scale_s),
            to_x(325, origin_x, scale_s), to_y(170, origin_y, scale_s),
            to_x(350, origin_x, scale_s), to_y(200, origin_y, scale_s)
        };
        nk_stroke_polyline(painter, points3, 4, scale_value(2, scale_s), nk_rgb(255,128,0));

        nk_stroke_line(painter,
            to_x(15, origin_x, scale_s), to_y(10, origin_y, scale_s),
            to_x(200, origin_x, scale_s), to_y(10, origin_y, scale_s),
            scale_value(2.0f, scale_s), nk_rgb(189,45,75));

        nk_stroke_rect(painter, nk_rect(
            to_x(370, origin_x, scale_s), to_y(20, origin_y, scale_s),
            scale_value(100, scale_s), scale_value(100, scale_s)),
            scale_value(10, scale_s), scale_value(3, scale_s), nk_rgb(0,0,255));

        nk_stroke_curve(painter,
            to_x(380, origin_x, scale_s), to_y(200, origin_y, scale_s),
            to_x(405, origin_x, scale_s), to_y(270, origin_y, scale_s),
            to_x(455, origin_x, scale_s), to_y(120, origin_y, scale_s),
            to_x(480, origin_x, scale_s), to_y(200, origin_y, scale_s),
            scale_value(2, scale_s), nk_rgb(0,150,220));

        nk_stroke_circle(painter, nk_rect(
            to_x(20, origin_x, scale_s), to_y(370, origin_y, scale_s),
            scale_value(100, scale_s), scale_value(100, scale_s)),
            scale_value(5, scale_s), nk_rgb(0,255,120));

        nk_stroke_triangle(painter,
            to_x(370, origin_x, scale_s), to_y(250, origin_y, scale_s),
            to_x(470, origin_x, scale_s), to_y(250, origin_y, scale_s),
            to_x(420, origin_x, scale_s), to_y(350, origin_y, scale_s),
            scale_value(6, scale_s), nk_rgb(255,0,143));

        nk_stroke_arc(painter,
            to_x(420, origin_x, scale_s), to_y(420, origin_y, scale_s),
            scale_value(50, scale_s), 0, 3.141592654f * 3.0f / 4.0f,
            scale_value(5, scale_s), nk_rgb(0,255,255));
    }
    nk_end(context);
}