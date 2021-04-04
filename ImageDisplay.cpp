#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "ImageDisplay.h"

#include <iostream>

ImageDisplay::ImageDisplay(const std::string& windowName, unsigned int width, unsigned int height) :
	image_(3 * width*height, 0), width_(width), height_(height), lastRowWritten_(0)
{

}

ImageDisplay::~ImageDisplay() {
}

void ImageDisplay::set(int x, int y, const Colour& colour) {
	image_[3*width_*y + 3*x + 0] = (unsigned char)(255 * colour.red);
	image_[3*width_*y + 3*x + 1] = (unsigned char)(255 * colour.green);
	image_[3*width_*y + 3*x + 2] = (unsigned char)(255 * colour.blue);
	lastRowWritten_ = y;
}

void ImageDisplay::refresh() {
	std::cout << "Rendered row " << lastRowWritten_ << " of " << height_ << "\r";
}

void ImageDisplay::save(const std::string& filename) const {
	stbi_write_png(filename.c_str(), int(width_), int(height_), 3, &image_[0], 3 * int(width_));
}

void ImageDisplay::pause(double seconds) {

}
