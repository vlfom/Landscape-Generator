#include "stdafx.h" 
#include <windows.h>
#include <stdio.h>
#include <iostream>

#include "Water_er.h"

void Erosion_Water(float***& HeightMap, float**& Water, float**& Sediment, int size) {
	float K[4] = { 0.01f, 0.01f, 0.1f, 0.001f };
	int i, j;

	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++){
			Water[i][j] += K[0];
			HeightMap[0][i][j] -= Water[i][j] * K[1];
			Sediment[i][j] += Water[i][j] * K[1];
		}

	for (i = 1; i<size - 1; i++)
		for (j = 1; j<size - 1; j++)
			if ((HeightMap[0][i][j] + Water[i][j]>HeightMap[0][i - 1][j] + Water[i - 1][j]) && (HeightMap[0][i][j] + Water[i][j]>HeightMap[0][i + 1][j] + Water[i + 1][j]) &&
				(HeightMap[0][i][j] + Water[i][j] > HeightMap[0][i][j - 1] + Water[i][j - 1]) && (HeightMap[0][i][j] + Water[i][j] > HeightMap[0][i][j + 1] + Water[i][j + 1])) {
				float 
					d_w[4], d_a, d_i[4], d_total, 
					average_height, 
					m_max, d_m;

				average_height = (HeightMap[0][i - 1][j] + Water[i - 1][j] + HeightMap[0][i + 1][j] + Water[i + 1][j] + HeightMap[0][i][j - 1] + Water[i][j - 1] + HeightMap[0][i][j + 1] + Water[i][j + 1]) / 4;

				d_total = 4 * (HeightMap[0][i][j] + Water[i][j]) - (HeightMap[0][i - 1][j] + Water[i - 1][j]) - (HeightMap[0][i + 1][j] + Water[i + 1][j]) - (HeightMap[0][i][j - 1] + Water[i][j - 1]) - (HeightMap[0][i][j + 1] + Water[i][j + 1]);

				d_a = HeightMap[0][i][j] + Water[i][j] - average_height;

				d_i[0] = HeightMap[0][i][j] + Water[i][j] - (HeightMap[0][i - 1][j] + Water[i - 1][j]);
				d_i[1] = HeightMap[0][i][j] + Water[i][j] - (HeightMap[0][i + 1][j] + Water[i + 1][j]);
				d_i[2] = HeightMap[0][i][j] + Water[i][j] - (HeightMap[0][i][j - 1] + Water[i][j - 1]);
				d_i[3] = HeightMap[0][i][j] + Water[i][j] - (HeightMap[0][i][j + 1] + Water[i][j + 1]);

				d_w[0] = min(Water[i][j], d_a)*d_i[0] / d_total;
				d_w[1] = min(Water[i][j], d_a)*d_i[1] / d_total;
				d_w[2] = min(Water[i][j], d_a)*d_i[2] / d_total;
				d_w[3] = min(Water[i][j], d_a)*d_i[3] / d_total;

				if (Water[i - 1][j] < Water[i][j])Sediment[i - 1][j] += Sediment[i][j] * Water[i - 1][j] / Water[i][j];
				if (Water[i + 1][j] < Water[i][j])Sediment[i + 1][j] += Sediment[i][j] * Water[i + 1][j] / Water[i][j];
				if (Water[i][j - 1] < Water[i][j])Sediment[i][j - 1] += Sediment[i][j] * Water[i][j - 1] / Water[i][j];
				if (Water[i][j + 1] < Water[i][j])Sediment[i][j + 1] += Sediment[i][j] * Water[i][j + 1] / Water[i][j];

				Water[i - 1][j] += d_w[0];
				Water[i + 1][j] += d_w[1];
				Water[i][j - 1] += d_w[2];
				Water[i][j + 1] += d_w[3];

				Water[i][j] -= d_w[0] + d_w[1] + d_w[2] + d_w[3];

				Water[i][j] *= 1 - K[2];

				m_max = Water[i][j] * K[3];

				d_m = max(0, Sediment[i][j] - m_max);

				Sediment[i][j] -= d_m;
				HeightMap[0][i][j] += d_m;
			}
}