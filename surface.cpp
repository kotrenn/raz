#include "surface.h"
#include "resourcemanager.h"

Uint32 mapRGB(Uint8 r, Uint8 g, Uint8 b)
{
	return mapRGBA(r, g, b, 255);
}

Uint32 mapRGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	Uint32 color = 0;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	color |= r << 24;
	color |= g << 16;
	color |= b << 8;
	color |= a;
#else
	color |= r;
	color |= g << 8;
	color |= b << 16;
	color |= a << 24;
#endif
	return color;
}

Uint32 mapRGBA(Uint32 c, Uint8 a)
{
	Uint32 color;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	color = (c & 0xffffff00) | (a);
#else
	color = (c & 0x00ffffff) | (a << 24);
#endif
	return color;
}

Uint32 invertRGB(Uint32 color)
{
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 RGBmask = 0xffffff00;
	Uint32 Amask   = 0x000000ff;
#else
	Uint32 RGBmask = 0x00ffffff;
	Uint32 Amask   = 0xff000000;
#endif
	return (color & Amask) | (~color & RGBmask);
}

Uint32 map_hue(int hue, Uint8 alpha)
{
	int c = (hue / 60) % 6;
	float f = hue / 60.0 - (int)(hue / 60.0);
	float r = 0, g = 0, b = 0;
	switch(c)
	{
	case 0: r = 1; g = f; b = 0; break;
	case 1: r = 1 - f; g = 1; b = 0; break;
	case 2: r = 0; g = 1; b = f; break;
	case 3: r = 0; g = 1 - f; b = 1; break;
	case 4: r = f; g = 0; b = 1; break;
	case 5: r = 1; g = 0; b = 1 - f; break;
	}
	int c_r = (int)(r * 255);
	int c_g = (int)(g * 255);
	int c_b = (int)(b * 255);
	return mapRGBA(c_r, c_g, c_b, alpha);
}

Uint32 map_hueB(int hue, Uint8 brightness)
{
	int c = (hue / 60) % 6;
	float f = hue / 60.0 - (int)(hue / 60.0);
	float r = 0, g = 0, b = 0;
	switch(c)
	{
	case 0: r = 1; g = f; b = 0; break;
	case 1: r = 1 - f; g = 1; b = 0; break;
	case 2: r = 0; g = 1; b = f; break;
	case 3: r = 0; g = 1 - f; b = 1; break;
	case 4: r = f; g = 0; b = 1; break;
	case 5: r = 1; g = 0; b = 1 - f; break;
	}
	int c_r = (int)(r * brightness);
	int c_g = (int)(g * brightness);
	int c_b = (int)(b * brightness);
	return mapRGBA(c_r, c_g, c_b, 255);
}

Uint32 random_color(int val)
{
	Uint32 ret = mapRGB(0, 0, 0);
	int n = randint(1, 6);
	switch(n)
	{
	case 1: ret = mapRGB(val, 0, 0); break;
	case 2: ret = mapRGB(val, val, 0); break;
	case 3: ret = mapRGB(0, val, 0); break;
	case 4: ret = mapRGB(0, val, val); break;
	case 5: ret = mapRGB(0, 0, val); break;
	case 6: ret = mapRGB(val, 0, val); break;
	}
	return ret;
}

Surface::Surface(int w, int h)
	:m_surface(NULL),
	 m_dims((SDL_Rect){ 0, 0, 0, 0 })
{
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif
	m_surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
	                                 w, h, 32, rmask, gmask, bmask, amask);
	if (m_surface == NULL)
	{
		fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	m_dims = (SDL_Rect){ 0, 0, (Uint16)w, (Uint16)h };
	SDL_SetAlpha(m_surface, 0, 255);
}

Surface::Surface(SDL_Surface *src)
	:m_surface(NULL),
	 m_dims((SDL_Rect){ 0, 0, 0, 0 })
{
	m_surface = src;
	m_surface->refcount++;
	m_dims.w = m_surface->w;
	m_dims.h = m_surface->h;
}

Surface::Surface(Surface *src)
	:m_surface(NULL),
	 m_dims((SDL_Rect){ 0, 0, 0, 0 })
{
	m_surface = src->m_surface;
	m_surface->refcount++;
	m_dims.w = m_surface->w;
	m_dims.h = m_surface->h;
}

Surface::Surface(const char *filename)
	:m_surface(NULL),
	 m_dims((SDL_Rect){ 0, 0, 0, 0 })
{
	if ((m_surface = IMG_Load(filename)) == NULL)
	{
		fprintf(stderr, "IMG_Load failed: %s\n", SDL_GetError());
		exit(1);
	}
	m_dims.w = m_surface->w;
	m_dims.h = m_surface->h;
}

Surface::~Surface()
{
	SDL_FreeSurface(m_surface);
}

SDL_PixelFormat *Surface::get_format() const
{
	return m_surface->format;
}

void Surface::blit(Surface *src, int x, int y)
{
	SDL_Rect dstrect = (SDL_Rect){ (Sint16)x, (Sint16)y, 0, 0 };
	SDL_BlitSurface(src->m_surface, &(src->m_dims), m_surface, &dstrect);
}

void Surface::blit(Surface *src, int x, int y, const Rect &srcdims)
{
	SDL_Rect srcrect = (SDL_Rect){ (Sint16)(src->m_dims.x + srcdims[0]),
	                               (Sint16)(src->m_dims.y + srcdims[1]),
	                               (Uint16)srcdims[2],
	                               (Uint16)srcdims[3] };
	SDL_Rect dstrect = (SDL_Rect){ (Sint16)x, (Sint16)y, 0, 0 };
	SDL_BlitSurface(src->m_surface, &srcrect, m_surface, &dstrect);
}

void Surface::fill(Uint32 color)
{
	SDL_FillRect(m_surface, NULL, color);
}

void Surface::set_alpha(Uint8 alpha)
{
	SDL_SetAlpha(m_surface, 0, alpha);
}

Surface *Surface::subsurface(int x, int y, int w, int h) const
{
	Surface *sub = new Surface(m_surface);
	sub->m_dims.x = x + m_dims.x;
	sub->m_dims.y = y + m_dims.y;
	sub->m_dims.w = w;
	sub->m_dims.h = h;
	return sub;
}

void Surface::invert()
{
	SDL_LockSurface(m_surface);
	SDL_PixelFormat *fmt = m_surface->format;
	int bpp = fmt->BytesPerPixel;

	for (int x = 0; x < get_w(); ++x)
	{
		for (int y = 0; y < get_h(); ++y)
		{
			Uint32 *p = (Uint32 *)((Uint8 *)m_surface->pixels +
			                       y * m_surface->pitch + x * bpp);
			Uint32 RGBmask = fmt->Rmask | fmt->Gmask | fmt->Bmask;
			*p = (*p & fmt->Amask) | (~(*p) & RGBmask);
		}
	}

	SDL_UnlockSurface(m_surface);
}

void Surface::flip() const
{
	SDL_Flip(m_surface);
}

Uint32 Surface::get_pixel(int x, int y)
{
	SDL_LockSurface(m_surface);
	SDL_PixelFormat *fmt = m_surface->format;
	int bpp = fmt->BytesPerPixel;
	Uint8 *p = (Uint8 *)m_surface->pixels + y * m_surface->pitch + x * bpp;
	Uint32 pixel = *(Uint32 *)p;
	SDL_UnlockSurface(m_surface);
	return pixel;
}

void Surface::draw_point(Uint32 color, int x, int y)
{
	if (x < 0 || x >= get_w() ||
	    y < 0 || y >= get_h())
		return;

	SDL_LockSurface(m_surface);
	SDL_PixelFormat *fmt = m_surface->format;
	int bpp = fmt->BytesPerPixel;
	Uint8 *p = (Uint8 *)m_surface->pixels +
		y * m_surface->pitch + x * bpp;
	Uint32 pixel;

	/* if (c_a < 255) */
	if (((color & fmt->Amask) >> fmt->Ashift) << fmt->Aloss < 255)
	{
		Uint8 c_r, c_g, c_b, c_a;
		SDL_GetRGBA(color, fmt, &c_r, &c_g, &c_b, &c_a);
		Uint8 r, g, b;
		SDL_GetRGB(*(Uint32 *)p, fmt, &r, &g, &b);

		Uint8 a = ~c_a; // 255 - c_a
		Uint8 a_r = (a * r + c_a * c_r) / 255;
		Uint8 a_g = (a * g + c_a * c_g) / 255;
		Uint8 a_b = (a * b + c_a * c_b) / 255;
		pixel = SDL_MapRGB(fmt, a_r, a_g, a_b);
	}
	else
		pixel = color;
	*(Uint32 *)p = pixel;

	SDL_UnlockSurface(m_surface);
}

void Surface::draw_line(Uint32 color, int x1, int y1,
                        int x2, int y2, int w)
{
	w *= 2;
	if (w < 2)
		return;
	float slope = (y2 == y1)? 1 : (x2 - x1) / (y2 - y1);
	if (fabs(slope) < 1)
	{
		for (int i = 0; i < 1 + w / 2; ++i)
		{
			draw_line(color, x1 + i, y1, x2 + i, y2);
			draw_line(color, x1 - i, y1, x2 - i, y2);
		}
	}
	else
	{
		for (int i = 0; i < 1 + w / 2; ++i)
		{
			draw_line(color, x1, y1 + i, x2, y2 + i);
			draw_line(color, x1, y1 - i, x2, y2 - i);
		}
	}
}

void Surface::draw_line(Uint32 color, int x1, int y1, int x2, int y2)
{
	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);

	if (dx == 0)
	{
		if (y2 > y1)
			for (int i = 0; i <= dy; ++i)
				draw_point(color, x1, y1 + i);
		else
			for (int i = 0; i <= dy; ++i)
				draw_point(color, x1, y1 - i);
		return;
	}
	else if (dy == 0)
	{
		if (x2 > x1)
			for (int i = 0; i <= dx; ++i)
				draw_point(color, x1 + i, y1);
		else
			for (int i = 0; i <= dx; ++i)
				draw_point(color, x1 - i, y1);
		return;
	}

	if (dx > dy)
	{
		if (x1 > x2)
		{
			x1 ^= x2;
			x2 ^= x1;
			x1 ^= x2;

			y1 ^= y2;
			y2 ^= y1;
			y1 ^= y2;
		}

		int ddy = 1;
		if (y1 >= y2) ddy = -1;
		int y = y1;
		int p = 0;

		for (int x = x1; x <= x2; ++x)
		{
			draw_point(color, x, y);

			p += dy;

			if (p * 2 >= dx)
			{
				y += ddy;
				p -= dx;
			}
		}
	}
	else
	{
		if (y1 > y2)
		{
			x1 ^= x2;
			x2 ^= x1;
			x1 ^= x2;

			y1 ^= y2;
			y2 ^= y1;
			y1 ^= y2;
		}

		int x = x1;
		int p = 0;
		int ddx = 1;
		if (x1 >= x2) ddx = -1;

		for (int y = y1; y <= y2; ++y)
		{
			draw_point(color, x, y);

			p += dx;

			if (p * 2 >= dy)
			{
				x += ddx;
				p -= dy;
			}
		}
	}
}

void Surface::draw_rect(Uint32 color, const Rect &rect, int width)
{
	if (width == 0)
	{
		int max_x = (int)(rect.get_x() + rect.get_w()) - 1;
		int max_y = (int)(rect.get_y() + rect.get_h()) - 1;
		int x = (int)rect.get_x();
		for (int y = (int)rect.get_y(); y <= max_y; ++y)
			draw_line(color, x, y, max_x, y);
		return;
	}

	if (width * 2 > rect.get_w()) width = (int)(rect.get_w() / 2);
	if (width * 2 > rect.get_h()) width = (int)(rect.get_h() / 3);

	for (int i = 0; i < width; ++i)
	{
		int x1 = (int)rect.get_x() + i;
		int y1 = (int)rect.get_y() + i;
		int x2 = (int)(rect.get_x() + rect.get_w()) - i - 1;
		int y2 = (int)(rect.get_y() + rect.get_h()) - i - 1;
		draw_line(color, x1, y1, x2, y1);
		draw_line(color, x1, y2, x2, y2);
		draw_line(color, x1, y1, x1, y2);
		draw_line(color, x2, y1, x2, y2);
	}
}

void Surface::draw_rect_3d(Uint32 color, const Rect &rect)
{
	draw_rect(color, rect);
	Uint8 c[3];
	SDL_GetRGB(color, m_surface->format, c, c + 1, c + 2);
	Uint32 color1 = mapRGB((255 + c[0]) / 2, (255 + c[1]) / 2,
	                       (255 + c[2]) / 2);
	Uint32 color2 = mapRGB(c[0] / 2, c[1] / 2, c[2] / 2);

	int x1 = (int)rect.get_x();
	int y1 = (int)rect.get_y();
	int x2 = (int)(rect.get_x() + rect.get_w());
	int y2 = (int)(rect.get_y() + rect.get_h());
	draw_line(color1, x1, y1, x2 - 1, y1);
	draw_line(color1, x1, y1, x1, y2 - 1);
	draw_line(color1, x1 + 1, y1 + 1, x2 - 2, y1 + 1);
	draw_line(color1, x1 + 1, y1 + 1, x1 + 1, y2 - 2);
	draw_line(color2, x1, y2 - 1, x2 - 1, y2 - 1);
	draw_line(color2, x2 - 1, y1, x2 - 1, y2 - 1);
	draw_line(color2, x1 + 1, y2 - 2, x2 - 2, y2 - 2);
	draw_line(color2, x2 - 2, y1 + 1, x2 - 2, y2 - 2);
}

void Surface::circle_points(Uint32 color, int cx, int cy,
                            int x, int y)
{
	if (x == 0)
	{
		draw_point(color, cx, cy + y);
		draw_point(color, cx, cy - y);
		draw_point(color, cx + y, cy);
		draw_point(color, cx - y, cy);
	}
	else if (x == y)
	{
		draw_point(color, cx + x, cy + y);
		draw_point(color, cx - x, cy + y);
		draw_point(color, cx + x, cy - y);
		draw_point(color, cx - x, cy - y);
	}
	else if (x < y)
	{
		draw_point(color, cx + x, cy + y);
		draw_point(color, cx - x, cy + y);
		draw_point(color, cx + x, cy - y);
		draw_point(color, cx - x, cy - y);
		draw_point(color, cx + y, cy + x);
		draw_point(color, cx - y, cy + x);
		draw_point(color, cx + y, cy - x);
		draw_point(color, cx - y, cy - x);
	}
}

void Surface::draw_circle(Uint32 color, int cx, int cy,
                          int radius, int width)
{
	if (radius <= 0)
		return;
	if (width == 0)
	{
		int x0 = cx - radius;
		int y0 = cy - radius;
		int max_x = x0 + 2 * radius + 1;
		int max_y = y0 + 2 * radius + 1;
		bool entered;
		int dx, dy, dx_2;
		int r_2 = radius * radius;
		for (int x = x0; x <= max_x; ++x)
		{
			entered = false;
			dx = abs(cx - x);
			dx_2 = dx * dx;
			for (int y = y0; y <= max_y; ++y)
			{
				dy = abs(cy - y);
				if (dx_2 + dy * dy <= r_2)
				{
					draw_point(color, x, y);
					entered = true;
				}
				else if (entered)
					y = max_y;
			}

		}
		return;
	}

	int x = 0;
	int y = radius;
	int p = (int)((5 - 4 * radius) / 4);
	circle_points(color, cx, cy, x, y);
	while (x < y)
	{
		++x;
		if (p < 0) p += 2 * x + 1;
		else
		{
			--y;
			p += 2 * (x - y) + 1;
		}
		circle_points(color, cx, cy, x, y);
	}
}

void Surface::draw_lightning(Uint32 color, int x1, int y1,
                             int x2, int y2, int width)
{
	int dist = 20;
	vector2d dv(x2 - x1, y2 - y1);
	float d = dv.norm();
	int count = (int)(d / dist);

	dv = dist * (dv / d);

	vector2d p[count + 2];
	p[0] = vector2d(x1, y1);
	for (int i = 1; i < count + 1; ++i)
		p[i] = p[0] + i * dv;
	p[count + 1] = vector2d(x2, y2);

	vector2d p0 = p[0];
	vector2d plot;
	int r = 0;
	float theta = 0;
	for (int i = 1; i < count + 2; ++i)
	{
		plot = p[i];
		if (i <= count)
		{
			r = randint(0, width);
			theta = randint(1, 360) * M_PI / 180.0;
			plot[0] += r * cos(theta);
			plot[1] += r * sin(theta);
		}
		draw_line(color, p0[0], p0[1], plot[0], plot[1]);
		p0 = plot;
	}
}

void Surface::draw_lightning_circle(Uint32 color, int cx, int cy,
                                    int radius, int width)
{
	float D = 50;
	float r = radius * sqrt(2);
	int dt = randint(1, 360);
	
	int n = 3;
	while (r * sqrt(1.0 - cos(2.0 * M_PI / n)) > D && n < 180)
		n++;
	
	for (int i = 0; i < n; ++i)
	{
		float theta1 = (360.0 / n) * i + dt;
		float theta2 = (360.0 / n) * ((i + 1) % n) + dt;
		theta1 *= M_PI / 180.0;
		theta2 *= M_PI / 180.0;
		float x1 = radius * cos(theta1) + cx;
		float y1 = radius * sin(theta1) + cy;
		float x2 = radius * cos(theta2) + cx;
		float y2 = radius * sin(theta2) + cy;
		draw_lightning(color, x1, y1, x2, y2, width);
	}
}
