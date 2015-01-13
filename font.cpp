#include "font.h"

#include "resourcemanager.h"

Font::Font(const char *filename)
	:m_font(NULL)
{
	ResourceManager *rm = ResourceManager::get_instance();
	m_font = rm->get_image(filename);
}

Font::~Font()
{
}

int Font::get_w() const
{
	return m_font->get_w() / get_cols();
}

int Font::get_h() const
{
	return m_font->get_h() / get_rows();
}

void Font::draw_string(Surface *dst, int x, int y, const char *str) const
{
	int len = strlen(str);
	Rect dims;
	int tw = get_w();
	int th = get_h();
	dims[2] = tw - 1;
	dims[3] = th - 1;
	for (int i = 0; i < len; ++i)
	{
		dims[0] = tw * (str[i] % get_cols());
		dims[1] = th * (str[i] / get_cols());
		dst->blit(m_font, x + tw * i, y, dims);
	}
}
