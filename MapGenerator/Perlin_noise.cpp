#include "stdafx.h" 
#include <windows.h>
#include <stdio.h>

#include "Perlin_noise.h"

float PERLIN_Noise(int k) {
	int x = (k << 13) ^ k;
	return (1.0f - ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float PERLIN_Noise2D(int x, int y) {
	int n = x + y * 57;
	n = (n << 13) ^ n;
	float xxx = (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float PERLIN_CentralizeNoise2D(int x, int y) {
	float corners, sides, center;
	corners = (PERLIN_Noise2D(x - 1, y - 1) + PERLIN_Noise2D(x - 1, y + 1) + PERLIN_Noise2D(x + 1, y - 1) + PERLIN_Noise2D(x + 1, y + 1)) / 16;
	sides = (PERLIN_Noise2D(x - 1, y) + PERLIN_Noise2D(x + 1, y) + PERLIN_Noise2D(x, y - 1) + PERLIN_Noise2D(x, y + 1)) / 8;
	center = PERLIN_Noise2D(x, y) / 4;
	return corners + sides + center;

}