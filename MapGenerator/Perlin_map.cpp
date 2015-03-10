#include "stdafx.h" 
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "Perlin_map.h"
#include "Perlin_noise.h"

void PERLIN_ResizeMap(float***& Map, int size){
	int i, j, l;
	Map = new float**[3];
	for (l = 0; l < 3; l++)
		Map[l] = new float*[size + 5];

	for (l = 0; l < 3; l++)
		for (i = 0; i < size + 5; i++)
			Map[l][i] = new float[size + 5];

	for (l = 0; l < 3; l++)
		for (i = 0; i < size + 5; i++)
			for (j = 0; j < size + 5; j++)
				Map[l][i][j] = 0;
}

void PERLIN_CreateMap(float***& Map, int size, float persistence, bool Centrilize){
	int i, j,
		I, max_I = (int)(log(size) / log(2)),
		frequency;
	float amplitude;
	for (I = 0; I <= max_I; I++){
		frequency = (int)pow((double)2, I);
		amplitude = (float)pow((double)persistence, I);
		for (i = 0; i < size + 2; i++)
			for (j = 0; j < size + 2; j++)
			{
				int randi = rand() % size, randj = rand() % size;
				if (!Centrilize)
					Map[0][i][j] += amplitude*PERLIN_Noise2D(i + randi, j + randj);
				else
					Map[0][i][j] += amplitude*PERLIN_CentralizeNoise2D(i + randi, j + randj);
			}
	}
	PERLIN_Zoom(Map, size, 100);
}

void PERLIN_Zoom(float***& Map, int size, int zoom){
	int i, j;
	for (i = 0; i < size + 2; i++)
	{
		for (j = 0; j < size + 2; j++)
		{
			Map[0][i][j] *= zoom;
		}
	}
}