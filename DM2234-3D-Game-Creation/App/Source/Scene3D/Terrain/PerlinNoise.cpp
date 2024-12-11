#include "PerlinNoise.h"

//init with reference values for permutation vector
PerlinNoise::PerlinNoise() {
	//use for time for seeding instead
	seed = time(NULL);
	PerlinNoise(seed);
}
//generate a new permutation vector based on the value of the seed
PerlinNoise::PerlinNoise(unsigned int randseed) {
	seed = randseed;
	//seeding the random engine
	std::default_random_engine engine(seed);
	//make vector size of 256
	p.resize(256);
	//fill vector p with values from 0-255
	iota(p.begin(), p.end(), 0);
	//shuffle vector randomly
	shuffle(p.begin(), p.end(), engine);
	//duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());
}
PerlinNoise::~PerlinNoise() {
	p.clear();
}

//get a noise value, for 2d images, z can be any value
double PerlinNoise::noise(double x, double y, double z) {
	//find the unit cube that contains the point
	int X = (int)floor(x);
	int Y = (int)floor(y);
	int Z = (int)floor(z);

	//find relative x,y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	//compute fade curves for each x,y,z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);
	//std::cout << u << ", " << v << ", " << w << std::endl;

	//hash coords of the 8 cube corners
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;
	//std::cout << A << ", " << AA << ", " << AB << ", " << B << ", " << BA << ", " << BB << std::endl;

	//add blended results from 8 corners of cube
	double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
		grad(p[BA], x - 1, y, z)),
		lerp(u, grad(p[AB], x, y - 1, z),
			grad(p[BB], x - 1, y - 1, z))),
		lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
			grad(p[BA + 1], x - 1, y, z - 1)),
			lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
				grad(p[BB + 1], x - 1, y - 1, z - 1))));
	return (res + 1.0) / 2.0;		//binding the value to 0-1, theoritical min/max is -1, 1
}

double PerlinNoise::fade(double t) {		//ease curve
	return t * t * t * (t * (t * 6 - 15) + 10);
	//quintic interpolation instead of smoothstep, reduces artefacts
	//old fade curve: 6t^5-15t^4-10t^3
}
double PerlinNoise::lerp(double t, double a, double b) {
	return a * (1 - t) + b * t;
}
double PerlinNoise::grad(int hash, double x, double y, double z) {
	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y;
	// if h is smaller than 8, x, else y
	double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	// if h is smaller than 4, y, else (if h is 12 or 14, x, else z)
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	//
}