#pragma once

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <numeric>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

class ppm {
	void init();
	//info about the PPM file (height and width)
	unsigned int nr_lines;
	unsigned int nr_columns;

public:
	//arrays for storing the R,G,B values
	std::vector<unsigned char> r;
	std::vector<unsigned char> g;
	std::vector<unsigned char> b;

	unsigned int height;
	unsigned int width;
	unsigned int max_col_val;
	//total number of elements (pixels)
	unsigned int size;

	ppm();
	ppm(const std::string& fname);
	ppm(const unsigned int _width, const unsigned int _height);
	~ppm();

	void read(const std::string& fname);
	void write(const std::string& fname);
};