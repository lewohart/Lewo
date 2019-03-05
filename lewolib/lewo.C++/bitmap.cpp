#include "stdafx.h"
#include "bitmap.h"

#include <windows.h>
#include <stack>
#include <algorithm>

BITMAPINFO bitmap::setup_bitmapinfo() {
	ZeroMemory(&info, sizeof(BITMAPINFO));
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biSizeImage = get_width() * sizeof(COLORREF) * -info.bmiHeader.biHeight;
	info.bmiHeader.biClrUsed = 0;
	info.bmiHeader.biClrImportant = 0;
	return info;
}

void bitmap::setup_size() {
	BITMAP bm;
	GetObject(handle, sizeof(bm), &bm);
	info.bmiHeader.biWidth = bm.bmWidth;
	info.bmiHeader.biHeight = -bm.bmHeight;
}

void bitmap::setup_pixels() {
	pixels = new COLORREF[get_width() * get_height()];
	HDC dc = CreateCompatibleDC(0);
	GetDIBits(dc, handle, 0, get_height(), pixels, &info, DIB_RGB_COLORS);
	DeleteDC(dc);
}

BITMAPFILEHEADER bitmap::get_bitmap_file_header() const {
	const BITMAPINFOHEADER* ih = &info.bmiHeader;
	BITMAPFILEHEADER hdr;
	hdr.bfType = ((WORD)('M' << 8) | 'B');
	hdr.bfSize = sizeof(BITMAPFILEHEADER) + ih->biSize + ih->biClrUsed * sizeof(RGBQUAD) + ih->biSizeImage;
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + ih->biSize + ih->biClrUsed * sizeof(RGBQUAD);
	return hdr;
}

void bitmap::load_file(LPCTSTR path) {
	handle = (HBITMAP)LoadImage(GetModuleHandle(NULL), path, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	setup_bitmapinfo();
	setup_size();
	setup_pixels();
}

void bitmap::save_file(LPCTSTR path) {
	BITMAPINFOHEADER* ih = &info.bmiHeader;
	BITMAPFILEHEADER hdr = get_bitmap_file_header();
	unsigned long written, palette_length = ih->biClrUsed * sizeof(RGBQUAD);
	HANDLE file = CreateFile(path, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	WriteFile(file, &hdr, sizeof(BITMAPFILEHEADER), &written, 0);
	WriteFile(file, ih, sizeof(BITMAPINFOHEADER) + palette_length, &written, 0);
	WriteFile(file, pixels, ih->biSizeImage, &written, 0);
	CloseHandle(file);
}

inline LPCOLORREF bitmap::get_pixels() const {
	return pixels;
}

inline int bitmap::get_width() const {
	return info.bmiHeader.biWidth;
}

inline int bitmap::get_height() const {
	return -info.bmiHeader.biHeight;
}

inline LPCOLORREF bitmap::pixel(const int &x, const int &y) {
	return &pixels[get_width() * y + x];
}

inline bool bitmap::in_bounds(const int &x, const int &y) const {
	return 0 <= x && x < get_width() && 0 <= y && y < get_height();
}

bitmap::~bitmap() {
	delete[] pixels;
	DeleteObject(handle);
}
