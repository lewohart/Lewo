#pragma once
#include <windows.h>
#include <stack>
#include <algorithm>

class bitmap
{
	HBITMAP handle;
	BITMAPINFO info;
	LPCOLORREF pixels;

	BITMAPINFO setup_bitmapinfo();
	void setup_size();
	void setup_pixels();
	BITMAPFILEHEADER get_bitmap_file_header() const;

public:
	void load_file(LPCTSTR path);
	void save_file(LPCTSTR path);

	inline LPCOLORREF get_pixels() const;
	
	inline int get_width() const;
	inline int get_height() const;
	
	inline LPCOLORREF pixel(const int &x, const int &y);
	inline bool in_bounds(const int &x, const int &y) const;

	~bitmap();
};
