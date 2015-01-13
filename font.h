#ifndef __FONT_H
#define __FONT_H

#include "surface.h"

class Font
{
public:
	Font(const char *filename);
	~Font();

	void draw_string(Surface *dst, int x, int y, const char *str) const;

	constexpr static int get_rows() { return 8; }
	constexpr static int get_cols() { return 16; }
	int get_w() const;
	int get_h() const;
private:
	Surface *m_font;
};

#endif
