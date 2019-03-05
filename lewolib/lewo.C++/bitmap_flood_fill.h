#pragma once

#include "bitmap.h"

class bitmap_flood_fill {
	bitmap &b;
	std::stack<std::pair<int, int>> stack;

	inline void pushSeed(const int &x, const int &y);
	inline bool popSeed(int &x, int &y);

	inline COLORREF GetPixel(const int &x, const int &y);
	inline void SetPixel(const int &x, const int &y, COLORREF fill);

public:
	bitmap_flood_fill(bitmap &bmp);

	// the set it up and kick it off routine.
	void seed_fill(COLORREF fill, int x, int y);

	// the main routine
	void fill_seeds_on_stack(COLORREF fill);

	// fill pixels to the left and right of the seed pixel until you hit
	// boundary pixels.  Return the locations of the leftmost and rightmost
	// filled pixels.
	void fill_contiguous_span(int x, int y, COLORREF fill, int &xLeft, int &xRight);
};