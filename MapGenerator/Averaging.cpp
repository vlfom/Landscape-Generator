#include "stdafx.h" 
#include <windows.h>
#include <stdio.h>

#include "Averaging.h"

void Averaging(float***& Map, int size, int num){
	int i, j, k;
	for (k = 0; k < num; k++)
		for (i = 1; i < size + 4; i++)
			for (j = 1; j < size + 4; j++){
				float sum = Map[0][i][j] + Map[0][i - 1][j] + Map[0][i][j - 1] + Map[0][i + 1][j] + Map[0][i][j + 1];
				Map[0][i][j] -= (5 * Map[0][i][j] - sum) / 5;
				Map[0][i - 1][j] -= (5 * Map[0][i - 1][j] - sum) / 5;
				Map[0][i][j - 1] -= (5 * Map[0][i][j - 1] - sum) / 5;
				Map[0][i + 1][j] -= (5 * Map[0][i + 1][j] - sum) / 5;
				Map[0][i][j + 1] -= (5 * Map[0][i][j + 1] - sum) / 5;
			}
}