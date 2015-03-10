#include "stdafx.h" 
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <string>

#include "Water_erosion_XPB.h"

void XPB_Resize(int a, float***& Mas){
	int i, j;
	Mas = new float **[10];
	for (i = 0; i < 10; i++)
		Mas[i] = new float *[a + 2];
	for (i = 0; i < 10; i++)
		for (j = 0; j < a + 2; j++)
			Mas[i][j] = new float[a + 2];
}

void XPB_Copy(int a, float***& HeightMap, float***& Water, float***& Sediment, float****& Flux){
	int i, j, k;
	for (i = 2; i < a - 1; i++)
		for (j = 2; j < a - 1; j++){
			HeightMap[0][i][j] = HeightMap[1][i][j];
			Water[0][i][j] = Water[3][i][j];
			Sediment[0][i][j] = Sediment[2][i][j];
			for (k = 0; k < 4; k++)
				Flux[0][i][j][k] = Flux[1][i][j][k];

		}
}

void XPB_Water_Increment_random(int a, float***& Water, float K_water, float Interval){
	int i, j, 
		number;
	number = a*a / 2;
	while (number){
		number--;
		i = 2 + rand() % (a - 3);
		j = 2 + rand() % (a - 3);
		Water[1][i][j] = Water[0][i][j] + K_water*Interval;
	}

}
void XPB_Water_Increment(int a, float***& Water, float K_water, float Interval){
	int i, j;
	for (i = 2; i < a - 2; i++)
		for (j = 2; j < a - 2; j++)
			Water[1][i][j] = Water[0][i][j] + K_water*Interval;
}

void XPB_Water_outflow(int a, float***& HeightMap, float***& Water, float****& Flux, float Interval, float S, float g, float l_X, float l_Y){
	int i, j;
	for (i = 2; i < a - 1; i++)
		for (j = 2; j < a - 1; j++){
			float d_h, l, K;

			d_h = Water[0][i][j] + HeightMap[0][i][j] - Water[0][i - 1][j] - HeightMap[0][i - 1][j]; //L
			if (!d_h)d_h = 1;
			l = sqrt(d_h*d_h + 1);
			Flux[1][i][j][0] = max(0, Flux[0][i][j][0] + Interval*S*g*d_h / l);

			d_h = Water[0][i][j] + HeightMap[0][i][j] - Water[0][i + 1][j] - HeightMap[0][i + 1][j]; //R
			if (!d_h)d_h = 1;
			l = sqrt(d_h*d_h + 1);
			Flux[1][i][j][1] = max(0, Flux[0][i][j][1] + Interval*S*g*d_h / l);

			d_h = Water[0][i][j] + HeightMap[0][i][j] - Water[0][i][j - 1] - HeightMap[0][i][j - 1]; //B
			if (!d_h)d_h = 1;
			l = sqrt(d_h*d_h + 1);
			Flux[1][i][j][2] = max(0, Flux[0][i][j][2] + Interval*S*g*d_h / l);

			d_h = Water[0][i][j] + HeightMap[0][i][j] - Water[0][i][j + 1] - HeightMap[0][i][j + 1]; //T
			if (!d_h)d_h = 1;
			l = sqrt(d_h*d_h + 1);
			Flux[1][i][j][3] = max(0, Flux[0][i][j][3] + Interval*S*g*d_h / l);

			float znam = (Flux[0][i][j][0] + Flux[0][i][j][1] + Flux[0][i][j][2] + Flux[0][i][j][3])*Interval;
			if (!znam)znam = 1;

			K = min(1, Water[1][i][j] * l_X * l_Y / znam);

			Flux[1][i][j][0] *= K;
			Flux[1][i][j][1] *= K;
			Flux[1][i][j][2] *= K;
			Flux[1][i][j][3] *= K;

		}
}

void XPB_Water_change(int a, float***& HeightMap, float***& Water, float****& Flux, float***& Vector, float Interval, float l_X, float l_Y){
	int i, j;
	for (i = 2; i < a - 1; i++)
		for (j = 2; j < a - 1; j++){
			float znam;
			Water[4][i][j] = Interval*((Flux[1][i - 1][j][1] + Flux[1][i][j - 1][3] + Flux[1][i + 1][j][0] + Flux[1][i][j + 1][2]) - (Flux[1][i][j][0] + Flux[1][i][j][1] + Flux[1][i][j][2] + Flux[1][i][j][3]));

			Water[2][i][j] = Water[1][i][j] + Water[4][i][j] / (l_X * l_Y);

			Water[5][i][j] = (Flux[1][i - 1][j][1] - Flux[1][i][j][0] + Flux[1][i][j][1] - Flux[1][i + 1][j][0]) / 2;
			znam = l_Y * (Water[1][i][j] + Water[2][i][j]) / 2;
			if (!znam)znam = 1;
			Vector[0][i][j] = Water[5][i][j] / znam;

			Water[6][i][j] = (Flux[1][i][j - 1][3] - Flux[1][i][j][2] + Flux[1][i][j][3] - Flux[1][i][j + 1][2]) / 2;
			znam = l_X * (Water[1][i][j] + Water[2][i][j]) / 2;
			if (!znam)znam = 1;
			Vector[1][i][j] = Water[5][i][j] / znam;

			Vector[2][i][j] = sqrt(Vector[0][i][j] * Vector[0][i][j] + Vector[1][i][j] * Vector[1][i][j]);

			float sredn_d_h = (abs(HeightMap[0][i - 1][j] - HeightMap[0][i][j]) + abs(HeightMap[0][i + 1][j] - HeightMap[0][i][j]) + abs(HeightMap[0][i][j - 1] - HeightMap[0][i][j]) + abs(HeightMap[0][i][j] - HeightMap[0][i][j + 1])) / 4;
			Vector[3][i][j] = sredn_d_h / sqrt(sredn_d_h*sredn_d_h + ((l_X + l_Y) / 2)*((l_X + l_Y) / 2));
		}
}

void XPB_Water_erosion(int a, float***& HeightMap, float***& Sediment, float***& Vector, float critical_height, float K_sediment, float K_depositing, float K_dissolving){
	int i, j;
	for (i = 2; i < a - 1; i++)
		for (j = 2; j < a - 1; j++){

			Sediment[3][i][j] = K_sediment * Vector[3][i][j] * Vector[2][i][j];
			if (Sediment[3][i][j] > Sediment[0][i][j]){
				HeightMap[1][i][j] = HeightMap[0][i][j] - K_dissolving*(Sediment[3][i][j] - Sediment[0][i][j]);

				if (HeightMap[1][i][j] < critical_height){
					HeightMap[1][i][j] = critical_height;
					Sediment[1][i][j] = Sediment[0][i][j] + (HeightMap[0][i][j] - HeightMap[1][i][j]);
				}
				else
					Sediment[1][i][j] = Sediment[0][i][j] + K_dissolving*(Sediment[3][i][j] - Sediment[0][i][j]);

			}
			else{
				HeightMap[1][i][j] = HeightMap[0][i][j] - K_depositing*(Sediment[0][i][j] - Sediment[3][i][j]);

				if (HeightMap[1][i][j] < critical_height){
					HeightMap[1][i][j] = critical_height;
					Sediment[1][i][j] = Sediment[0][i][j] + (HeightMap[0][i][j] - HeightMap[1][i][j]);
				}
				else
					Sediment[1][i][j] = Sediment[0][i][j] + K_depositing*(Sediment[0][i][j] - Sediment[3][i][j]);

			}
		}
}

void XPB_Water_sediment_transportation(int a, float***& Sediment, float***& Vector, float Interval){
	int i, j;
	for (i = 2; i < a - 1; i++)
		for (j = 2; j < a - 1; j++){
			int x, y;
			float value;
			x = (int)(i - Vector[0][i][j] * Interval);
			y = (int)(j - Vector[1][i][j] * Interval);
			if (x<2 || y<2 || x>a - 2 || y>a - 2)
				value = (Sediment[1][i - 1][j] + Sediment[1][i + 1][j] + Sediment[1][i][j - 1] + Sediment[1][i][j + 1]) / 4;
			else
				value = Sediment[1][x][y];

			Sediment[2][i][j] = value;
		}
}

void XPB_Water_evaporation(int a, float***& Water, float K_evaporation, float Interval){
	int i, j;
	for (i = 2; i < a - 1; i++)
		for (j = 2; j < a - 1; j++)
			Water[3][i][j] = Water[2][i][j] * (1 - K_evaporation*Interval);
}


void XPB_Water_erosion_main(int iterations, int a, float***& HeightMap, float***& Water, float***& Sediment, float****& Flux, float***& Vector, float Interval, float l, float S, float g, float K_water, float K_sediment, float K_depositing, float K_dissolving, float K_evaporation, float critical_height){
	int i;
	float l_X = 0.5, l_Y = 0.5;

	int ANS = iterations;

	for (i = 1; i <= ANS; i++){

		XPB_Water_Increment(a, Water, K_water, Interval);

		XPB_Water_outflow(a, HeightMap, Water, Flux, Interval, S, g, l_X, l_Y);

		XPB_Water_change(a, HeightMap, Water, Flux, Vector, Interval, l_X, l_Y);

		XPB_Water_erosion(a, HeightMap, Sediment, Vector, critical_height, K_sediment, K_depositing, K_dissolving);

		XPB_Water_sediment_transportation(a, Sediment, Vector, Interval);

		XPB_Water_evaporation(a, Water, K_evaporation, Interval);

		XPB_Copy(a, HeightMap, Water, Sediment, Flux);

	}
}