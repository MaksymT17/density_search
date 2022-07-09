#pragma once
#include "thirdparty/bitmap_image.hpp"

// Image Bmp drawer for represantation of Objects

class BmpDrawer {
public:
	BmpDrawer(std::string &file)
		: mImage(bitmap_image(file.c_str())), drawer(mImage) {}
	~BmpDrawer() = default;

	inline void drawPixel(int x, int y) {
		drawer.pen_width(1);
		drawer.pen_color(palette_colormap[e_amber]);
		drawer.plot_pixel(x, y);
	}

	inline void drawCircle(int x, int y, int r) {
		drawer.pen_width(1);
		drawer.pen_color(palette_colormap[e_scarlet]);
		drawer.circle(x, y, r);
	}
	inline void drawRectangle(int minW, int minH, int maxW, int maxH) {
		drawer.pen_width(1);
		drawer.pen_color(palette_colormap[e_scarlet]);
		drawer.rectangle(minW, minH, maxW, maxH);
	}


	inline void save(const std::string &file) { mImage.save_image(file); }

private:
	bitmap_image mImage;
	image_drawer drawer;
};