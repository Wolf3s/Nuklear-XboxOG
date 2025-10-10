// NuklearOG.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "graphics.h"

#define NK_PRIVATE
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include "nuklear.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* macros */
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 800

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

struct nk_glfw_vertex {
    float position[3];
    nk_byte col[4];
    float uv[2];
};

struct device {
    struct nk_buffer cmds;
    struct nk_draw_null_texture tex_null;

    //IDirect3DVertexBuffer8 *vbo;
    //IDirect3DIndexBuffer8 *ebo;
    D3DTexture* font_tex;
};

static void
device_init(struct device *dev)
{
    graphics::createDevice();

    nk_buffer_init_default(&dev->cmds);

 //   HRESULT hr = graphics::getDevice()->CreateVertexBuffer(MAX_VERTEX_MEMORY, D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &dev->vbo);
	//if (FAILED(hr)) {
	//	return;
	//}

 //   hr = graphics::getDevice()->CreateIndexBuffer(MAX_ELEMENT_MEMORY, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &dev->ebo);
 //   if (FAILED(hr)) {
	//	return;
	//}
    //{
    //    /* buffer setup */
    //    GLsizei vs = sizeof(struct nk_glfw_vertex);
    //    size_t vp = offsetof(struct nk_glfw_vertex, position);
    //    size_t vt = offsetof(struct nk_glfw_vertex, uv);
    //    size_t vc = offsetof(struct nk_glfw_vertex, col);

    //    glGenBuffers(1, &dev->vbo);
    //    glGenBuffers(1, &dev->ebo);
    //    glGenVertexArrays(1, &dev->vao);

    //    glBindVertexArray(dev->vao);
    //    glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
    //    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

    //    glEnableVertexAttribArray((GLuint)dev->attrib_pos);
    //    glEnableVertexAttribArray((GLuint)dev->attrib_uv);
    //    glEnableVertexAttribArray((GLuint)dev->attrib_col);

    //    glVertexAttribPointer((GLuint)dev->attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
    //    glVertexAttribPointer((GLuint)dev->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
    //    glVertexAttribPointer((GLuint)dev->attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
    //}

    //glBindTexture(GL_TEXTURE_2D, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
}

static void
device_upload_atlas(struct device *dev, const void *image, int width, int height)
{
    D3DTexture* texture = graphics::createImage((uint8_t*)image, D3DFMT_A8R8G8B8, width, height);
    dev->font_tex = texture;
}


static void
device_draw(struct device *dev, struct nk_context *ctx, int width, int height,
    enum nk_anti_aliasing AA)
{


        struct nk_buffer vbuf, ebuf;
        const struct nk_draw_command *cmd;
        const nk_draw_index *offset = NULL;
        UINT vertex_count;


        /* fill convert configuration */
        struct nk_convert_config config;
        static const struct nk_draw_vertex_layout_element vertex_layout[] = {
            {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_glfw_vertex, position)},
            {NK_VERTEX_COLOR, NK_FORMAT_B8G8R8A8, NK_OFFSETOF(struct nk_glfw_vertex, col)},
            {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_glfw_vertex, uv)},
            {NK_VERTEX_LAYOUT_END}
        };
        NK_MEMSET(&config, 0, sizeof(config));
        config.vertex_layout = vertex_layout;
        config.vertex_size = sizeof(struct nk_glfw_vertex);
        config.vertex_alignment = NK_ALIGNOF(struct nk_glfw_vertex);
        config.tex_null = dev->tex_null;
        config.circle_segment_count = 22;
        config.curve_segment_count = 22;
        config.arc_segment_count = 22;
        config.global_alpha = 1.0f;
        config.shape_AA = AA;
        config.line_AA = AA;

        /* convert shapes into vertexes */
        nk_buffer_init_default(&vbuf);
        nk_buffer_init_default(&ebuf);
        nk_convert(ctx, &dev->cmds, &vbuf, &ebuf, &config);

        offset = (const nk_draw_index *)nk_buffer_memory_const(&ebuf);
            vertex_count = (UINT)vbuf.needed / sizeof(struct nk_glfw_vertex);

    

      /* iterate over and execute each draw command */
        nk_draw_foreach(cmd, ctx, &dev->cmds)
        {
            if (!cmd->elem_count) continue;
            //glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
            //glScissor(
            //    (GLint)(cmd->clip_rect.x),
            //    (GLint)((height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h))),
            //    (GLint)(cmd->clip_rect.w),
            //    (GLint)(cmd->clip_rect.h));
            //glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);

            graphics::getDevice()->SetTexture(0, (D3DTexture*)cmd->texture.ptr);
        
            const struct nk_rect null_rect = nk_get_null_rect();
            bool isNullRect = memcmp(&null_rect, &cmd->clip_rect, sizeof(struct nk_rect)) == 0;
            if (isNullRect)
            {
                D3DVIEWPORT8 vp = { 0, 0, graphics::getWidth(), graphics::getHeight(), 0.0f, 1.0f };
                graphics::getDevice()->SetViewport(&vp);
            }
            else
            {
                D3DVIEWPORT8 vp = { cmd->clip_rect.x, cmd->clip_rect.y, cmd->clip_rect.w, cmd->clip_rect.h, 0.0f, 1.0f };
                graphics::getDevice()->SetViewport(&vp);
            }

            graphics::getDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
                0, vertex_count, cmd->elem_count/3, offset, D3DFMT_INDEX16,
                nk_buffer_memory_const(&vbuf), sizeof(struct nk_glfw_vertex));
    
            offset += cmd->elem_count;
        }

        nk_buffer_free(&vbuf);
        nk_buffer_free(&ebuf);

        nk_clear(ctx);
        nk_buffer_clear(&dev->cmds);
    

    /* default OpenGL state */
    //glUseProgram(0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
    //glDisable(GL_BLEND);
    //glDisable(GL_SCISSOR_TEST);
}

static void
pump_input(struct nk_context *ctx /*GLFWwindow *win*/)
{
    /*double x, y;
    nk_input_begin(ctx);
    glfwPollEvents();

    nk_input_key(ctx, NK_KEY_DEL, glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_ENTER, glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_TAB, glfwGetKey(win, GLFW_KEY_TAB) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_BACKSPACE, glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_LEFT, glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_RIGHT, glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_UP, glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_DOWN, glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS);

    if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        nk_input_key(ctx, NK_KEY_COPY, glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_PASTE, glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_SHIFT, 1);
    } else {
        nk_input_key(ctx, NK_KEY_COPY, 0);
        nk_input_key(ctx, NK_KEY_PASTE, 0);
        nk_input_key(ctx, NK_KEY_CUT, 0);
        nk_input_key(ctx, NK_KEY_SHIFT, 0);
    }

    glfwGetCursorPos(win, &x, &y);
    nk_input_motion(ctx, (int)x, (int)y);
    nk_input_button(ctx, NK_BUTTON_LEFT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    nk_input_button(ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    nk_input_button(ctx, NK_BUTTON_RIGHT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    nk_input_end(ctx);*/
}

struct nk_canvas {
    struct nk_command_buffer *painter;
    struct nk_vec2 item_spacing;
    struct nk_vec2 panel_padding;
    struct nk_style_item window_background;
};

static void
canvas_begin(struct nk_context *ctx, struct nk_canvas *canvas, nk_flags flags,
    int x, int y, int width, int height, struct nk_color background_color)
{
    /* save style properties which will be overwritten */
    canvas->panel_padding = ctx->style.window.padding;
    canvas->item_spacing = ctx->style.window.spacing;
    canvas->window_background = ctx->style.window.fixed_background;

    /* use the complete window space and set background */
    ctx->style.window.spacing = nk_vec2(0,0);
    ctx->style.window.padding = nk_vec2(0,0);
    ctx->style.window.fixed_background = nk_style_item_color(background_color);

    /* create/update window and set position + size */
    flags = flags & ~NK_WINDOW_DYNAMIC;
    nk_window_set_bounds(ctx, "Window", nk_rect(x, y, width, height));
    nk_begin(ctx, "Window", nk_rect(x, y, width, height), NK_WINDOW_NO_SCROLLBAR|flags);

    /* allocate the complete window space for drawing */
    {struct nk_rect total_space;
    total_space = nk_window_get_content_region(ctx);
    nk_layout_row_dynamic(ctx, total_space.h, 1);
    nk_widget(&total_space, ctx);
    canvas->painter = nk_window_get_canvas(ctx);}
}

static void
canvas_end(struct nk_context *ctx, struct nk_canvas *canvas)
{
    nk_end(ctx);
    ctx->style.window.spacing = canvas->panel_padding;
    ctx->style.window.padding = canvas->item_spacing;
    ctx->style.window.fixed_background = canvas->window_background;
}

void __cdecl main()
{
    /* GUI */
    struct device device;
    struct nk_font_atlas atlas;
    struct nk_context ctx;

    /* GUI */
    {device_init(&device);
    {const void *image; int w, h;
    struct nk_font *font;
    nk_font_atlas_init_default(&atlas);
    nk_font_atlas_begin(&atlas);
    font = nk_font_atlas_add_default(&atlas, 13, 0);
    image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    device_upload_atlas(&device, image, w, h);
    nk_font_atlas_end(&atlas, nk_handle_ptr(device.font_tex), &device.tex_null);
    nk_init_default(&ctx, &font->handle);

    int width = graphics::getWidth();
    int height = graphics::getHeight();

    while (true)
    {
        pump_input(&ctx/*, win*/);

        /* draw */
        {struct nk_canvas canvas;
        canvas_begin(&ctx, &canvas, 0, 0, 0, width, height, nk_rgb(250,250,250));
        {
            nk_fill_rect(canvas.painter, nk_rect(15,15,210,210), 5, nk_rgb(247, 230, 154));
            nk_fill_rect(canvas.painter, nk_rect(20,20,200,200), 5, nk_rgb(188, 174, 118));
            nk_draw_text(canvas.painter, nk_rect(30, 30, 150, 20), "Text to draw", 12, &font->handle, nk_rgb(188,174,118), nk_rgb(0,0,0));
            nk_fill_rect(canvas.painter, nk_rect(250,20,100,100), 0, nk_rgb(0,0,255));
            nk_fill_circle(canvas.painter, nk_rect(20,250,100,100), nk_rgb(255,0,0));
            nk_fill_triangle(canvas.painter, 250, 250, 350, 250, 300, 350, nk_rgb(0,255,0));
            nk_fill_arc(canvas.painter, 300, 180, 50, 0, 3.141592654f * 3.0f / 4.0f, nk_rgb(255,255,0));

            {float points[12];
            points[0] = 200; points[1] = 250;
            points[2] = 250; points[3] = 350;
            points[4] = 225; points[5] = 350;
            points[6] = 200; points[7] = 300;
            points[8] = 175; points[9] = 350;
            points[10] = 150; points[11] = 350;
            nk_fill_polygon(canvas.painter, points, 6, nk_rgb(0,0,0));}

            nk_stroke_line(canvas.painter, 15, 10, 200, 10, 2.0f, nk_rgb(189,45,75));
            nk_stroke_rect(canvas.painter, nk_rect(370, 20, 100, 100), 10, 3, nk_rgb(0,0,255));
            nk_stroke_curve(canvas.painter, 380, 200, 405, 270, 455, 120, 480, 200, 2, nk_rgb(0,150,220));
            nk_stroke_circle(canvas.painter, nk_rect(20, 370, 100, 100), 5, nk_rgb(0,255,120));
            nk_stroke_triangle(canvas.painter, 370, 250, 470, 250, 420, 350, 6, nk_rgb(255,0,143));
        }
        canvas_end(&ctx, &canvas);}

        /* Draw */
        graphics::getDevice()->BeginScene();
        graphics::getDevice()->Clear(0L, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, 0xff333333, 1.0f, 0L);
        device_draw(&device, &ctx, width, height, NK_ANTI_ALIASING_ON);
		graphics::getDevice()->EndScene();
		graphics::getDevice()->Present(NULL, NULL, NULL, NULL);
    }}}
}
