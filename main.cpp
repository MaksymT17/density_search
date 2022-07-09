/**
 *
 * This is the main source file for checking most best density regions in pictures.
 *
 * @author Maksym V
 */

#include <iostream>
#include <cmath>
#include "Matrix.hpp"
#include "BmpDrawer.hpp"
#include <deque>
#include <iterator>

//#define ENABLE_CACHING
using namespace std;

//desired parameters for search
constexpr const int CIRCLE_RADIUS = 15;
constexpr const int FIELD_MAX_X = 500;
constexpr const int FIELD_MAX_Y = 500;
constexpr const int CIRCLES_COUNT = 64;
constexpr const int SEARCH_STEP = 1;

struct PlacementInfo {
	int x, y;
	int value;
};


Matrix matrix(FIELD_MAX_X, FIELD_MAX_Y);

#ifdef ENABLE_CACHING
std::deque<PlacementInfo> cache;
int CHOSEN_COUNT = 0;
#endif // ENABLE_CACHING

static int getCircleY(const int r, const int x) {
	return static_cast<int>(round(sqrt((r*r - x * x))));
}

//sectors
// /3 4\
// \2 1/

static int getValue(const int x, const int y) {
	if (x < 0 || y < 0 || x >= FIELD_MAX_X || y >= FIELD_MAX_Y)
		return 0;
	
	return matrix(x, y);
}

static void clearPosition(const int x, const int y) {
	if (x < 0 || y < 0 || x >= FIELD_MAX_X || y >= FIELD_MAX_Y)
		return;

	//drawer.drawPixel(x, y);
	matrix(x, y) = 0;
}

static const int getCircledAreaValue(const int center_x, const int center_y) {
	int circled_area_sum = getValue(center_x, center_y);

	for (int r_incr = 1; r_incr < CIRCLE_RADIUS; ++r_incr) {
		circled_area_sum += getValue(center_x + r_incr, center_y);
		circled_area_sum += getValue(center_x - r_incr, center_y);
		circled_area_sum += getValue(center_x, center_y + r_incr);
		circled_area_sum += getValue(center_x, center_y - r_incr);
	}

	for (int x = 1; x < CIRCLE_RADIUS; ++x) {
		const int y_limit = getCircleY(CIRCLE_RADIUS, x);
		for (int y = 1; y < y_limit; ++y) {
			//sector 1
			circled_area_sum += getValue(center_x + x, center_y + y);
			//sector 2
			circled_area_sum += getValue(center_x - x, center_y + y);
			//sector 3
			circled_area_sum += getValue(center_x - x, center_y - y);
			//sector 4
			circled_area_sum += getValue(center_x + x, center_y - y);
		}
	}

	return circled_area_sum;
}

static void clearCircledAreaValue(const int center_x, const int center_y) {
	clearPosition(center_x, center_y);

	for (int r_incr = 1; r_incr < CIRCLE_RADIUS; ++r_incr) {
		clearPosition(center_x + r_incr, center_y);
		clearPosition(center_x - r_incr, center_y);
		clearPosition(center_x, center_y + r_incr);
		clearPosition(center_x, center_y - r_incr);
	}

	for (int x = 1; x < CIRCLE_RADIUS; ++x) {
		const int y_limit = getCircleY(CIRCLE_RADIUS, x);
		for (int y = 1; y < y_limit; ++y) {
			clearPosition(center_x + x, center_y + y);
			clearPosition(center_x - x, center_y + y);
			clearPosition(center_x - x, center_y - y);
			clearPosition(center_x + x, center_y - y);
		}
	}
}

int main(int argc, char **argv)
{
	std::string file("../input/oil3.BMP");
	std::string output_file("oil3_out.BMP");
	
	if (argc = 2) {
		file = argv[1];
		output_file = "result.BMP";
	}

	BmpDrawer drawer(file);
	
	//setup map
	bitmap_image image(file);
	for (int x = 0; x < FIELD_MAX_X; ++x) {
		for (int y = 0; y < FIELD_MAX_Y; ++y) {
			///note: as the oil_map image is gray scaled, we can get only the one channel value(all r g b are equal in GS)
			matrix(x, y) = image.get_pixel(x, y).red;
		}
	}

	//make the iteration, where optimal position for the cirle has to be chosen
	//with cached data in single iteration mutiple positions could be chosen
	for (int i = 0; i < CIRCLES_COUNT; ++i) {
#ifdef ENABLE_CACHING
		if (CHOSEN_COUNT == CIRCLES_COUNT) {
			break;
		}
#endif
		PlacementInfo pi{ 0,0,0 };
		for (int x = 0; x < FIELD_MAX_X; x += SEARCH_STEP) {
			for (int y = 0; y < FIELD_MAX_Y; y += SEARCH_STEP) {
				int val = getCircledAreaValue(x, y);

				if (val > pi.value) {
					pi.x = x;
					pi.y = y;
					pi.value = val;
#ifdef ENABLE_CACHING
					cache.push_back(pi);
					if (cache.size() == CIRCLES_COUNT - CHOSEN_COUNT) {
						cache.pop_front();
					}
#endif
				}
			}
		}
#ifdef ENABLE_CACHING
		std::cout << CHOSEN_COUNT << " " << pi.x << " " << pi.y << endl;

		drawer.drawCircle(cache.back().x, cache.back().y, CIRCLE_RADIUS);
		clearCircledAreaValue(cache.back().x, cache.back().y);
		++CHOSEN_COUNT;

		if (CHOSEN_COUNT == CIRCLES_COUNT)
			break;

		for (auto ii = cache.size() - 2; ii >= CHOSEN_COUNT; --ii) {
			if (cache.begin()->value < getCircledAreaValue(cache[ii].x, cache[ii].y)) {
				std::cout << CHOSEN_COUNT << " " << cache[ii].x << " " << cache[ii].y << endl;
				drawer.drawCircle(cache[ii].x, cache[ii].y, CIRCLE_RADIUS);
				clearCircledAreaValue(cache[ii].x, cache[ii].y);
				++CHOSEN_COUNT;

				if (CHOSEN_COUNT == CIRCLES_COUNT)
					break;
			}
		}
#endif
#ifndef ENABLE_CACHING
		// when results ready print them in console and draw circles on pictures
		std::cout << pi.x << " " << pi.y << endl;
		drawer.drawCircle(pi.x, pi.y, CIRCLE_RADIUS);
		clearCircledAreaValue(pi.x, pi.y);
#endif // !ENABLE_CACHING
	}

	drawer.save(output_file);
}
