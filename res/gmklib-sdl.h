//gmklib-sdl.h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_ttf.h>

#define __GMKLIB__DEBUG     1

SDL_Window*   __gmklib__window;
SDL_Renderer* __gmklib__renderer;
SDL_Surface*  __gmklib__bgsurface;
SDL_Texture*  __gmklib__rendtexture;
SDL_Event     __gmklib__window_event;
SDL_Rect      __gmklib__dispview;

int __gmklib__event_queue= 0;

int SCREEN_WIDTH;
int SCREEN_HEIGHT;

int __temp__w, __temp__h;
SDL_Surface* __temp__surface;

typedef struct __gmklib__sprite_s
{
    SDL_Surface* surface;
    int frames;
} __gmklib__sprite;

typedef struct __gmklib__object_s
{
    __gmklib__sprite* sprite;
    bool visible;
} __gmklib__object;

typedef struct __gmklib__instance_s
{
    struct __gmklib__object_s* object;
    int x;
    int y;
} __gmklib__instance;

typedef struct __gmklib__room_s
{
    int width;
    int height;
    struct __gmklib__instance_s* instances[2048];
    SDL_Color back_color;
    bool fill_back;
} __gmklib__room;

__gmklib__sprite* __gmklib__sprites[128];
int __gmklib__lastsprite= 0;

__gmklib__room* current_room= NULL;

#define TARGET_FPS          300
bool __gmklib__usevsync = true;

typedef unsigned char   u8;
typedef signed char     s8;
typedef unsigned short  u16;
typedef signed short    s16;
typedef unsigned int    u32;
typedef signed int      s32;

u32 frametime = 0;
u32 __gmklib__lastsincesecond = 0;
int fps = 0;

u16 alarm[16];
const u8 *keys;

u8 __gmklib__dcol_r= 255;
u8 __gmklib__dcol_g= 255;
u8 __gmklib__dcol_b= 255;
u8 __gmklib__dcol_a= 255;

SDL_BlendMode __gmklib__blendmode= SDL_BLENDMODE_BLEND;

int mouse_x, mouse_y;

///// Color definitions (macro) /////

#define c_aqua		25,255,224
#define c_black		0,0,0
#define c_blue		0,0,255
#define c_dkgray	64,64,64
#define c_fuchsia	255,0,255
#define c_gray		128,128,128
#define c_green		0,128,0
#define c_lime		0,255,0
#define c_ltgray	196,196,196
#define c_maroon	128,32,8
#define c_navy		0,0,128
#define c_olive		128,128,0
#define c_orange	255,128,0
#define c_purple	128,0,128
#define c_red		255,0,0
#define c_silver	192,192,192
#define c_teal		0,128,128
#define c_white		255,255,255
#define c_yellow	255,255,0

///// SDL Specific /////

/**/void keyboard_poll(); int event_poll();
SDL_Window* display_init(int screen_width, int screen_height, char* title, int args)
{
    if (__GMKLIB__DEBUG) printf("[GMKLib] Initializing SDL...\n");
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("[GMKLib] ERROR! Error initializing SDL: %s\n", SDL_GetError());
    }

    if (__GMKLIB__DEBUG) printf("[GMKLib] Initializing window...\n");
    __gmklib__window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height, args);
    if (__gmklib__window == NULL)
    {
        printf("[GMKLib] CRITICAL! Error initializing window: %s\n", SDL_GetError());
        exit(1);
    }

    if (__GMKLIB__DEBUG) printf("[GMKLib] Initializing renderer...\n");
    __gmklib__bgsurface= SDL_CreateRGBSurface(0, screen_width, screen_height, 24, 0, 0, 0, 0);
    __gmklib__renderer= SDL_CreateRenderer(__gmklib__window, -1, SDL_RENDERER_ACCELERATED | __gmklib__usevsync*SDL_RENDERER_PRESENTVSYNC);
    if (__gmklib__renderer == NULL)
    {
        printf("[GMKLib] CRITICAL! Error initializing renderer: %s\n", SDL_GetError());
        exit(2);
    }

    __gmklib__rendtexture= SDL_CreateTextureFromSurface(__gmklib__renderer, __gmklib__bgsurface);

    SDL_SetRenderDrawColor(__gmklib__renderer, 0, 0, 0, 0);
    SDL_RenderClear(__gmklib__renderer);

    SCREEN_WIDTH=screen_width, SCREEN_HEIGHT=screen_height;
    __gmklib__dispview.x=0, __gmklib__dispview.y=0,
    __gmklib__dispview.w=screen_width, __gmklib__dispview.h=screen_height;

    TTF_Init();

    if (__GMKLIB__DEBUG) printf("[GMKLib] Done. Took %d milliseconds.\n", SDL_GetTicks());

    keyboard_poll();
    event_poll();

    return __gmklib__window;
}

void display_render()
{
    //SDLRect vw;
    //vw.x= __gmklib__dispview.x, vw.y= __gmklib__dispview.y,
    //vw.h= __gmklib__dispview.x, vw.y= __gmklib__dispview.y,

    if (SDL_GetTicks()-__gmklib__lastsincesecond >= 1000.0/(float)TARGET_FPS)
    {
        frametime = SDL_GetTicks()-__gmklib__lastsincesecond;
        __gmklib__lastsincesecond = SDL_GetTicks();
        fps = 1000.0/(float)frametime;//__gmklib__frames;
        //__gmklib__frames = 0;
    }

    SDL_RenderPresent(__gmklib__renderer);
    SDL_RenderCopy(__gmklib__renderer, __gmklib__rendtexture, NULL, NULL);
}

void display_destroy()
{
    SDL_DestroyRenderer(__gmklib__renderer);
    SDL_DestroyTexture(__gmklib__rendtexture);
    SDL_FreeSurface(__gmklib__bgsurface);
    for (int i=0; i<__gmklib__lastsprite; i++)
        SDL_FreeSurface(__gmklib__sprites[i]->surface);
    if (__GMKLIB__DEBUG) printf("[GMKLib] Freed sprites.\n");
    SDL_DestroyWindow(__gmklib__window);
    TTF_Quit();
    if (__GMKLIB__DEBUG) printf("[GMKLib] Display destroyed.\n");
}

///// Color /////

void draw_set_color(u8 r, u8 g, u8 b)
{
    __gmklib__dcol_r= r;
    __gmklib__dcol_g= g;
    __gmklib__dcol_b= b;
    __gmklib__dcol_a= 255;
    SDL_SetRenderDrawColor(__gmklib__renderer, r, g, b, 255);
}

void draw_set_color_alpha(u8 r, u8 g, u8 b, u8 alpha)
{
    __gmklib__dcol_r= r;
    __gmklib__dcol_g= g;
    __gmklib__dcol_b= b;
    __gmklib__dcol_a= alpha;
    SDL_SetRenderDrawColor(__gmklib__renderer, r, g, b, alpha);
}

#define draw_set_blendmode(bmode) \
    __gmklib__blendmode = bmode

///// Graphics /////

void view_set_ext(int xview, int yview, int wview, int hview, int xport, int yport, int wport, int hport, bool centerpos)
{
    if (centerpos)
        xview -= wview/2, yview-=hview/2;

    SDL_Rect vp;
    vp.x = xport-xview, vp.y= yport-yview,
    vp.w = wport+xview, vp.h = hport+yview;
    SDL_Rect vv;
    vv.x = xview, vv.y= yview,
    vv.w = wview, vv.h = hview;

    SDL_RenderSetLogicalSize(__gmklib__renderer, wview, hview);
    SDL_RenderSetViewport(__gmklib__renderer, &vp);
    SDL_RenderSetClipRect(__gmklib__renderer, &vv);
    SDL_RenderSetScale(__gmklib__renderer, (float)SCREEN_WIDTH/(float)wport, (float)SCREEN_HEIGHT/(float)hport);
    __gmklib__dispview.x = xview, __gmklib__dispview.y = yview;
    __gmklib__dispview.w = wview, __gmklib__dispview.h = hview;
}

#define view_set(xview, yview, wview, hview, follow) \
    view_set_ext(xview, yview, wview, hview, 0, 0, wview, hview, follow)

void view_reset()
{
    SDL_RenderSetLogicalSize(__gmklib__renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderSetViewport(__gmklib__renderer, NULL);
}

void display_set_scaling(float xscale, float yscale)
{
    SDL_RenderSetScale(__gmklib__renderer, xscale, yscale);
}

///// Drawing /////

void draw_clear(u8 r, u8 g, u8 b)
{
    SDL_SetRenderDrawColor(__gmklib__renderer, r, g, b, 255);
    SDL_RenderClear(__gmklib__renderer);
    SDL_FillRect(__gmklib__bgsurface, NULL, SDL_MapRGB(__gmklib__bgsurface->format, r, g, b));
}

void draw_clear_alpha(u8 r, u8 g, u8 b, u8 alpha)
{
    SDL_SetRenderDrawColor(__gmklib__renderer, r, g, b, alpha);
    SDL_RenderClear(__gmklib__renderer);
}

void draw_rectangle(int x1, int y1, int x2, int y2, bool outline)
{
    SDL_Rect rect;

    rect.x= x1,    rect.y= y1,
    rect.w= x2-x1, rect.h= y2-y1;

    if (outline)
        SDL_RenderDrawRect(__gmklib__renderer, &rect);
    else
        SDL_FillRect(__gmklib__bgsurface, &rect, SDL_MapRGB(__gmklib__bgsurface->format, __gmklib__dcol_r, __gmklib__dcol_g, __gmklib__dcol_b));
}

void draw_rectangle_surface(int x1, int y1, int x2, int y2/*, bool astexture*/) //It's always filled
{
    SDL_Surface* surface= SDL_CreateRGBSurface(0, x2-x1, y2-y1, 32, 0, 0, 0, 0);

    SDL_SetSurfaceBlendMode(surface, __gmklib__blendmode);

    SDL_Rect rect;

    rect.x= x1,    rect.y= y1,
    rect.w= x2-x1, rect.h= y2-y1;

    SDL_FillRect(surface, NULL,  SDL_MapRGB(surface->format, __gmklib__dcol_r, __gmklib__dcol_g, __gmklib__dcol_b));


    SDL_Texture* texture= SDL_CreateTextureFromSurface(__gmklib__renderer, surface);
    SDL_SetTextureAlphaMod(texture, __gmklib__dcol_a);

    SDL_RenderCopy(__gmklib__renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);

    //SDL_BlitSurface(surface, NULL, __gmklib__bgsurface, &rect);

    SDL_FreeSurface(surface);
}

void draw_line(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(__gmklib__renderer, x1, y1, x2, y2);
}

void draw_point(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawPoint(__gmklib__renderer, x1, y1);
}

void draw_pixel(int x1, int y1, int x2, int y2, u8 r, u8 g, u8 b)
{
    SDL_SetRenderDrawColor(__gmklib__renderer, r, g, b, 255);
    SDL_RenderDrawPoint(__gmklib__renderer, x1, y1);
}

/**/int StrAppendIntAt(char* str, int pos, int n);
void draw_text_stretched(TTF_Font* font, int x, int y, int w, int h, char* str, ...)
{
    va_list valist;
    va_start(valist, (int)str[0]);
    char txt[256]; memset(txt,'\0',sizeof(txt));//sprintf(txt, str, va_arg(valist, int));

    SDL_Color col = { __gmklib__dcol_r, __gmklib__dcol_g, __gmklib__dcol_b };

    int ilen = strlen(str);

    for (int ich= 0, i=0; str[ich]!='\0' && i<sizeof(txt); ich++)
    {
        if (str[ich] == '&' && str[ich+1] == 'n') { txt[i]='\n'; i++; ich++; } else
        if (str[ich] == '%' && str[ich+1] == 'd') { int n= va_arg(valist, int); i+=StrAppendIntAt(txt,i,n); ich++; }
        else { txt[i]= str[ich]; i++; }//strncpy(txt,str,sizeof(txt));
    }

    SDL_Surface* surfaceMessage= TTF_RenderText_Solid(font, txt, col);

    SDL_Texture* Message = SDL_CreateTextureFromSurface(__gmklib__renderer, surfaceMessage);

    SDL_Rect Message_rect;
    Message_rect.x = x, Message_rect.y = y;
    Message_rect.w = w, Message_rect.h = h;

    SDL_RenderCopy(__gmklib__renderer, Message, NULL, &Message_rect);

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);

    va_end(valist);
}

#define draw_text(font, x, y, str, more ...)                                    TTF_SizeText(font, str, &__temp__w, &__temp__h); \
                                                                                draw_text_stretched(font, x, y, __temp__w, __temp__h, str, more)

#define draw_text_scaled(font, x, y, xscale, yscale, str, more ...)             TTF_SizeText(font, str, &__temp__w, &__temp__h); \
                                                                                draw_text_stretched(font, x, y, __temp__w*xscale, __temp__h*yscale, str, more)

void draw_sprite_manual_surface(__gmklib__sprite* spr, int top, int left, int w, int h, int x, int y, float xscale, float yscale, unsigned char alpha, SDL_Surface* dest)
{
    SDL_Rect src_rect;
        src_rect.x=left, src_rect.y=top,
        src_rect.w=w, src_rect.h=h;
    SDL_Rect dest_rect;
        dest_rect.x=x, dest_rect.y=y,
        dest_rect.w=(int)((float)w*xscale), dest_rect.h=(int)((float)h*yscale);
    SDL_SetSurfaceAlphaMod(spr->surface, alpha);
    SDL_BlitSurface(spr->surface, &src_rect, dest, &dest_rect);
}

void draw_sprite_manual(__gmklib__sprite* spr, int left, int top, int w, int h, int x, int y,
                        float xscale, float yscale, double angle, int col_r, int col_g, int col_b, unsigned char alpha)
{
    SDL_Rect src_rect;
        src_rect.x=left, src_rect.y=top,
        src_rect.w=w, src_rect.h=h;
    SDL_Rect dest_rect;
        dest_rect.x=x, dest_rect.y=y,
        dest_rect.w=abs((int)((float)w*xscale)), dest_rect.h=abs((int)((float)h*yscale));

    SDL_SetSurfaceBlendMode(spr->surface, __gmklib__blendmode);
    //SDL_SetSurfaceColorMod(spr->surface, col_r, col_g, col_b);
    SDL_SetSurfaceColorMod(spr->surface, c_white);
    SDL_Texture* texture= SDL_CreateTextureFromSurface(__gmklib__renderer, spr->surface);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureColorMod(texture, col_r, col_g, col_b);
    SDL_SetTextureAlphaMod(texture, alpha);
    SDL_RenderCopyEx(__gmklib__renderer, texture, &src_rect, &dest_rect, angle, NULL,
        (SDL_RendererFlip)(((xscale < 0)?1:0)*SDL_FLIP_HORIZONTAL | ((yscale < 0)?1:0)*SDL_FLIP_VERTICAL));

    SDL_DestroyTexture(texture);
}

#define draw_sprite(spr, subimg, x, y) \
    draw_sprite_manual(spr, subimg*(spr->surface->w/spr->frames), 0, spr->surface->w/spr->frames, spr->surface->h, x, y, 1, 1, 0, c_white, 255)

#define draw_sprite_ext(spr, subimg, x, y, xscale, yscale, rot, color, alpha) \
    draw_sprite_manual(spr, subimg*(spr->surface->w/spr->frames), 0, spr->surface->w/spr->frames, spr->surface->h, x, y, xscale, yscale, rot, color, alpha)

///// Input /////

void keyboard_poll()
{
     keys= SDL_GetKeyboardState(NULL);
}

u8 keyboard_check(int key)
{
    return keys[key];
}

u8 keyboard_check_pressed(int key)
{
    if (__gmklib__window_event.type == SDL_KEYDOWN)
        return keys[key];
    else
        return 0;
}

u8 keyboard_check_released(int key)
{
    if (__gmklib__window_event.type == SDL_KEYUP && !keys[key])
        return 1;
    else
        return 0;
}

///// Misc /////

void sleep_millis(u16 millisec)
{
    SDL_Delay(millisec);
}

void sleep(u16 frames)
{
    SDL_Delay((1000.0/(float)TARGET_FPS)*frames);
}

void do_update()
{
    for (int i=0; i < 12; i++)
        if (alarm[i] > 0) alarm[i]--;
}

void screen_wait_vsync()
{
    keyboard_poll();
    event_poll();

    sleep(1);
}

int event_poll()
{
    __gmklib__event_queue= SDL_PollEvent(&__gmklib__window_event);

    return __gmklib__event_queue;
}

int check_exit()
{
    if (__gmklib__event_queue > 0)
    if (__gmklib__window_event.type == SDL_QUIT)
        return 1;
    return 0;
}

///// Helper functions /////

int StrAppendIntAt(char* str, int pos, int n)
{
    short inlen= log10(n)+1;
    unsigned int na[inlen];
    int tin= n;

    if (n > 0) {
        for(int i=0; i<=inlen;i++) {
            if (tin < 1) {inlen= i; break;}
            na[i]= tin%10;
            tin/=10;
        }
    } else if (n == 0) {inlen=1; na[0]= 0;}

    for (int i=0; i<=inlen; i++)
        str[pos+i]= '0'+na[inlen-i-1];

    return inlen;
}

bool inside_rectangle(int x1, int y1, int x2, int y2, int w, int h)
{
    return (x1 >= x2 && x1 <= x2+w) && (y1 >= y2 && y1 <= y2+h);
}

///// Game /////

#define sprite_add(name, spr_s, subno) /*sprite_add(__gmklib__sprite, spr_struct, int)*/ \
    name.frames= subno; \
    name.surface= SDL_CreateRGBSurface(0, spr_s.width, spr_s.height, spr_s.bytes_per_pixel*8, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000); \
    memcpy((u8*)name.surface->pixels, (u8*)spr_s.pixel_data, spr_s.width*spr_s.height*spr_s.bytes_per_pixel+1); \
    __gmklib__sprites[__gmklib__lastsprite]= &name; \
    __gmklib__lastsprite++

/*
#define room_add(name, _width, _height, back_color_rgb, _fill_back) \
    name.width= _width; \
    name.height= _height; \
    name.back_color= { back_color_rgb }; \
    name.fill_back= _fill_back; \
    for (int __i=0; __i<2048; __i++) name.instances[__i]= NULL
*/

#define object_add(name, spr, _visible) \
    name.sprite= spr; \
    name.visible= _visible

#define room_add(name, _width, _height, back_color_r, back_color_g, back_color_b, _fill_back) \
    name.width= _width; \
    name.height= _height; \
    name.back_color.r= back_color_r; \
    name.back_color.g= back_color_g; \
    name.back_color.b= back_color_b; \
    name.fill_back= _fill_back; \
    for (int __i=0; __i<sizeof(name.instances)/sizeof(__gmklib__instance*); __i++) name.instances[__i]= NULL

int instance_create(int x, int y, __gmklib__object* obj)
{
    for (int i=0; i<sizeof(current_room->instances)/sizeof(__gmklib__instance*); i++)
    {
        if (!current_room->instances[i])
        {
            __gmklib__instance* newinst= (__gmklib__instance*)malloc(sizeof(__gmklib__instance));
            newinst->x= x;
            newinst->y= y;
            newinst->object= obj;
            current_room->instances[i]= newinst;
            return i;
        }
    }

    return -1; //Instance limit reached
}

#define room_goto(room) current_room= &room

void draw_current_room()
{
    __gmklib__room* room= current_room;

    if (room->fill_back)
        draw_clear(room->back_color.r, room->back_color.g, room->back_color.b);

    for (int i=0; i<sizeof(current_room->instances)/sizeof(__gmklib__instance*); i++)
    {
        if (!current_room->instances[i]) continue;

        __gmklib__instance* inst= current_room->instances[i];

        draw_sprite(inst->object->sprite, 0, inst->x, inst->y);
    }
}
