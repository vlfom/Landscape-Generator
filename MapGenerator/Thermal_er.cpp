#include "stdafx.h" 
#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "Thermal_er.h"

void Erosion_Thermal(int size, float***& HeightMap){
	int x, z;
	float d_max;

	for (x = 1; x < size; x++)
		for (z = 1; z < size; z++){
			int _x_ = x, _z_ = z;
			float T = 32;
			d_max = 0;

			if (HeightMap[0][x - 1][z] - HeightMap[0][x][z] > d_max){
				d_max = HeightMap[0][x - 1][z] - HeightMap[0][x][z];
				_x_ = x - 1;
				_z_ = z;
			}

			if (HeightMap[0][x + 1][z] - HeightMap[0][x][z] > d_max){
				d_max = HeightMap[0][x + 1][z] - HeightMap[0][x][z];
				_x_ = x + 1;
				_z_ = z;
			}

			if (HeightMap[0][x][z - 1] - HeightMap[0][x][z] > d_max){
				d_max = HeightMap[0][x][z - 1] - HeightMap[0][x][z];
				_x_ = x;
				_z_ = z - 1;
			}

			if (HeightMap[0][x][z + 1] - HeightMap[0][x][z] > d_max){
				d_max = HeightMap[0][x][z + 1] - HeightMap[0][x][z];
				_x_ = x;
				_z_ = z + 1;
			}


			if (d_max < 1){
				float delta = 100.0f + rand() % 50;
				HeightMap[0][x][z] -= d_max / delta;
				HeightMap[0][_x_][_z_] += d_max / delta;
			}
		}
}