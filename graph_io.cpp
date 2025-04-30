#include "graph_io.h"

std::string Graph_IO::input;

std::vector<std::vector<double>> Graph_IO::generate_gaussian_kernel(int radius, double sigma) {
	int size = 2 * radius + 1;
	std::vector<std::vector<double>> kernel(size, std::vector<double>(size));
	double sum = 0;
	for (int x = -radius; x <= radius; x++) {
		for (int y = -radius; y <= radius; y++) {
			double exponent = -(x * x + y * y) / (2 * sigma * sigma);
			sum += (kernel[x + radius][y + radius] =
				exp(exponent) / (2 * M_PI * sigma * sigma));
		}
	}
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			kernel[i][j] /= sum;
		}
	}
	return kernel;
}
void Graph_IO::gaussian_blur(IMAGE* srcImg, IMAGE* dstImg, int radius, double sigma) {
	auto kernel = generate_gaussian_kernel(radius, sigma);
	int width = srcImg->getwidth();
	int height = srcImg->getheight();
	DWORD* srcBuf = GetImageBuffer(srcImg);
	DWORD* dstBuf = GetImageBuffer(dstImg);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			double r = 0, g = 0, b = 0;
			for (int i = -radius; i <= radius; i++) {
				for (int j = -radius; j <= radius; j++) {
					int nx = x + i;
					int ny = y + j;

					if (nx < 0) nx = -nx;
					if (ny < 0) ny = -ny;
					if (nx >= width) nx = 2 * width - nx - 1;
					if (ny >= height) ny = 2 * height - ny - 1;

					COLORREF color = srcBuf[ny * width + nx];
					double weight = kernel[i + radius][j + radius];

					r += GetRValue(color) * weight;
					g += GetGValue(color) * weight;
					b += GetBValue(color) * weight;
				}
			}
			dstBuf[y * width + x] = RGB(
				(int)(r),
				(int)(g),
				(int)(b)
			);
		}
	}
}

void Graph_IO::set_text_style(LPCWSTR name, long height, unsigned char quality) {
	LOGFONT font;
	gettextstyle(&font);
	wcscpy_s(font.lfFaceName, name);
	font.lfHeight = height;
	font.lfQuality = quality;
	settextstyle(&font);
}
void Graph_IO::solid_rectangle(COLORREF fill_color, int left, int top, int right, int bottom) {
	setfillcolor(fill_color);
	solidrectangle(left, top, right, bottom);
}
void Graph_IO::fill_round_rectangle(COLORREF fill_color, COLORREF line_color, int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight) {
	setfillcolor(fill_color);
	setlinecolor(line_color);
	fillroundrect(left, top, right, bottom, ellipsewidth, ellipseheight);
}
void Graph_IO::output_line(COLORREF line_color, const std::vector<int>& position) {
	if (position.size() != 4) {
		return;
	}
	setlinecolor(line_color);
	line(position[0], position[1], position[2], position[3]);
}
void Graph_IO::output_text(COLORREF text_color, int x, int y, const std::string& str) {
	settextcolor(text_color);
	outtextxy(x, y, string_to_lpcwstr(str));
}
void Graph_IO::draw_text(COLORREF text_color, const std::string& str, RECT* pRect, UINT uFormat) {
	settextcolor(text_color);
	drawtext(string_to_lpcwstr(str), pRect, uFormat);
}
void Graph_IO::input_text(const ExMessage& msg) {
	const BYTE& key = msg.vkcode;
	static bool is_shift = 0;
	std::string num_signal = "*+\r-./";
	std::string shift_num = ")!@#$%^&*(";
	std::string oem = ";=,-./`[\\]\'";
	std::string shift_oem = ":+<_>?~{|}\"";

	if (msg.message == WM_KEYDOWN) {
		if (key == VK_BACK || key == VK_DELETE) {
			if (!input.empty()) {
				input.pop_back();
			}
		}
		else if (key == VK_SHIFT) {
			is_shift = 1;
		}
		else if (key == VK_TAB) {
			do {
				input += ' ';
			} while (input.length() % 4);
		}
		else if (key == VK_RETURN || key == VK_SPACE) {
			input += key;
		}
		else if (key >= '0' && key <= '9') {
			input += is_shift ? shift_num[size_t(key) - '0'] : key;
		}
		else if (key >= 'A' && key <= 'Z') {
			input += is_shift ? key : key - 'A' + 'a';
		}
		else if (key >= VK_NUMPAD0 && key <= VK_NUMPAD9) {
			input += key - VK_NUMPAD0 + '0';
		}
		else if (key >= VK_MULTIPLY && key <= VK_DIVIDE) {
			input += num_signal[size_t(key - VK_MULTIPLY)];
		}
		else if (key >= VK_OEM_1 && key <= VK_OEM_3) {
			input += is_shift ? shift_oem[size_t(key) - VK_OEM_1] : oem[size_t(key) - VK_OEM_1];
		}
		else if (key >= VK_OEM_4 && key <= VK_OEM_7) {
			input += is_shift ? shift_oem[size_t(key) - VK_OEM_4 + VK_OEM_3 - VK_OEM_1 + 1 ] : 
				oem[size_t(key) - VK_OEM_4 + VK_OEM_3 - VK_OEM_1 + 1];
		}
	}
	else if (msg.message == WM_KEYUP && key == VK_SHIFT) {
		is_shift = 0;
	}
}