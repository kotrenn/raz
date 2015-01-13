#ifndef __SURFACE_H
#define __SURFACE_H

#include "common.h"
#include "shape2d.h"

Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b);
Uint32 mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
Uint32 mapRGBA(Uint32 c, Uint8 a = 255);
Uint32 invertRGB(Uint32 color);
Uint32 map_hue(int hue, Uint8 alpha);
Uint32 map_hueB(int hue, Uint8 brightness);

Uint32 random_color(int val);

class Surface
{
public:
	Surface(int w, int h);
	Surface(SDL_Surface *src);
	Surface(Surface *src);
	Surface(const char *filename);
	~Surface();

	SDL_Surface *get_surface() { return m_surface; }

	int get_w() const { return m_surface->w; }
	int get_h() const { return m_surface->h; }

	SDL_PixelFormat *get_format() const;

	void blit(Surface *src, int x, int y);
	void blit(Surface *src, int x, int y, const Rect &srcdims);
	void fill(Uint32 color);
	void set_alpha(Uint8 alpha);
	Surface *subsurface(int x, int y, int w, int h) const;
	void invert();
	void flip() const;

	Uint32 get_pixel(int x, int y);

	void draw_point(Uint32 color, int x, int y);
	void draw_line(Uint32 color, int x1, int y1,
	               int x2, int y2, int w);
	void draw_line(Uint32 color, int x1, int y1, int x2, int y2);
	void draw_rect(Uint32 color, const Rect &rect, int width = 0);
	void draw_rect_3d(Uint32 color, const Rect &rect);
	void draw_circle(Uint32 color, int cx, int cy,
	                 int radius, int width = 0);

	void draw_lightning(Uint32 color, int x1, int y1,
	                    int x2, int y2, int width = 12);
	void draw_lightning_circle(Uint32 color, int cx, int cy,
	                           int radius, int width = 12);
private:
	void circle_points(Uint32 color, int cx, int cy, int x, int y);
    
	SDL_Surface *m_surface;
	SDL_Rect m_dims;
};

#endif
