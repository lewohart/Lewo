#include "stdafx.h"
#include "bitmap_flood_fill.h"


inline void bitmap_flood_fill::pushSeed(const int &x, const int &y) {
	stack.push(std::make_pair(x, y));
}

inline bool bitmap_flood_fill::popSeed(int &x, int &y) {
	if (stack.empty())
		return false;

	std::pair<int, int> value = stack.top();
	x = value.first, y = value.second;
	stack.pop();
	return true;
}

inline COLORREF bitmap_flood_fill::GetPixel(const int &x, const int &y) {
	return *b.pixel(x, y);
}

inline void bitmap_flood_fill::SetPixel(const int &x, const int &y, COLORREF fill) {
	*b.pixel(x, y) = fill;
}

bitmap_flood_fill::bitmap_flood_fill(bitmap &bmp) : b(bmp) {
}

// the set it up and kick it off routine.
void bitmap_flood_fill::seed_fill(COLORREF fill, int x, int y)
{
	// we assume the stack is created empty, and that no other
	// routines or threads are using this stack for anything
	pushSeed(x, y);
	fill_seeds_on_stack(fill);
}

// the main routine
void bitmap_flood_fill::fill_seeds_on_stack(COLORREF fill)
{
	COLORREF col1, col2;
	int x, y, xLeft, xRight, i;

	while (popSeed(x, y)) {
		if (*b.pixel(x, y) != fill) {
			fill_contiguous_span(x, y, fill, xLeft, xRight);

			// single pixel spans handled as a special case in the else clause
			if (xLeft != xRight) {
				// handle the row above you
				y++;
				for (i = xLeft + 1; i <= xRight; i++) {
					col1 = GetPixel(i - 1, y);
					col2 = GetPixel(i, y);
					if (col1 != fill && col2 == fill)
						pushSeed(i - 1, y);
				}
				if (col2 != fill && col2 != fill)
					pushSeed(xRight, y);

				// handle the row below you
				y -= 2;
				for (i = xLeft + 1; i <= xRight; i++) {
					col1 = GetPixel(i - 1, y);
					col2 = GetPixel(i, y);
					if (col1 != fill && col2 == fill)
						pushSeed(i - 1, y);
				}
				if (col2 != fill && col2 != fill)
					pushSeed(xRight, y);
			}
			else {
				col1 = GetPixel(xLeft, y + 1);
				col2 = GetPixel(xLeft, y - 1);
				if (col1 != fill)
					pushSeed(xLeft, y + 1);
				if (col2 != fill)
					pushSeed(xLeft, y - 1);
			}

		}
	}
}

// fill pixels to the left and right of the seed pixel until you hit
// boundary pixels.  Return the locations of the leftmost and rightmost
// filled pixels.
void bitmap_flood_fill::fill_contiguous_span(int x, int y, COLORREF fill, int &xLeft, int &xRight)
{
	COLORREF col;
	int i;

	// fill pixels to the right until you reach a boundary pixel
	i = x;
	col = GetPixel(i, y);
	while (col != fill) {
		SetPixel(i, y, fill);
		i++;
		col = GetPixel(i, y);
	}
	xRight = i - 1;

	// fill pixels to the left until you reach a boundary pixel
	i = x - 1;
	col = GetPixel(i, y);
	while (col != fill) {
		SetPixel(i, y, fill);
		i--;
		col = GetPixel(i, y);
	}
	xLeft = i + 1;
}
