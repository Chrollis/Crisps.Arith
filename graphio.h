#pragma once
#include "utils.h"

#define SIDE 50
#define WIDTH 16 * SIDE
#define HEIGHT 8 * SIDE

#define MIDNIGHTBLUE 0x701919
#define FIREBRICK3 0x2626CD
#define DARKVIOLET 0xD30094
#define LIMEGREEN 0x32CD32
#define GOLDENROD1 0x25C1FF

class Graphio {
private:
	static std::vector<std::vector<double>> generate_gaussian_kernel(int radius, double sigma);
public:
	static void set_text_style(LPCWSTR name, long height = SIDE, unsigned char quality = ANTIALIASED_QUALITY);
	static void solid_rectangle(COLORREF fill_color, int left, int top, int right, int bottom);
	static void fill_round_rectangle(COLORREF fill_color, COLORREF line_color, int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight);
	static void output_line(COLORREF line_color, const std::vector<int>& position);
	static void output_text(COLORREF text_color, int x, int y, const std::string& str);
	static void draw_text(COLORREF text_color, const std::string& str, RECT* pRect, UINT uFormat);
	static void input_text(const ExMessage& msg);
	static void gaussian_blur(IMAGE* srcImg, IMAGE* dstImg, int radius, double sigma);
public:
	static std::string input;
public:
	Graphio() = default;
	~Graphio() = default;
};
