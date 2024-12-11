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

class PerlinNoise {
public:
	PerlinNoise();
	PerlinNoise(unsigned int randseed);
	~PerlinNoise();

	double noise(double x, double y, double z);
private:
	int seed;			//seed
	vector<int> p;		//permutation vector

	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
};