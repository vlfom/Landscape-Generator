#include "stdafx.h" 
#include <iostream>
#include <cmath>

#include "Diamond_square.h"
#include "Random.h"

void DS_Create_Mass_Map(float***& Map, int***& Cubes, int devisions){
	int i, j;
	int size_Map = DS_Func(devisions) + 8, size_Cubes = (int)pow((float)4, devisions) + 8;
	Map = new float**[size_Map];
	Cubes = new int**[size_Cubes];

	for (i = 0; i < size_Map; i++)Map[i] = new float*[size_Map];
	for (i = 0; i < size_Map; i++)
		for (j = 0; j < size_Map; j++){
			Map[i][j] = new float[3];
			Map[i][j][0] = 0;
			Map[i][j][1] = 0;
			Map[i][j][2] = 0;
		}

	for (i = 0; i < size_Cubes; i++)Cubes[i] = new int*[2];
	for (i = 0; i < size_Cubes; i++)
		for (j = 0; j < 2; j++){
			Cubes[i][j] = new int[2];
			Cubes[i][j][0] = 0;
			Cubes[i][j][1] = 0;
		}
}

int DS_Func(int a){
	int e = 1, i;
	if (a >= 1)e = 3;
	for (i = 2; i <= a; i++)
		e = 2 * e - 1;
	return e;
}

void DS_MakeBorders(float***& Map, int num){
	int i;
	for (i = 0; i <= num + 1; i++){
		Map[0][i][1] = 0;
		Map[i][0][1] = 0;
		Map[num + 1][i][1] = 0;
		Map[i][num + 1][1] = 0;
	}
}

void DS_Square(int s_i, int s_j, int f_i, int f_j, int kof, float***& Map, int rand_answer){

	//Center point
	Map[(s_i + f_i) / 2][(s_j + f_j) / 2][0] = (Map[s_i][s_j][0] + Map[f_i][f_j][0]) / 2;

	float random = Rand_generate(rand_answer);

	Map[(s_i + f_i) / 2][(s_j + f_j) / 2][1] = (Map[s_i][s_j][1] + Map[s_i][f_j][1] + Map[f_i][s_j][1] + Map[f_i][f_j][1]) / 4 + random*kof;
	Map[(s_i + f_i) / 2][(s_j + f_j) / 2][2] = (Map[s_i][s_j][2] + Map[f_i][f_j][2]) / 2;

}

void DS_Diamond(int s_i, int s_j, int f_i, int f_j, int num, int kof, float***& Map, int rand_answer){
	int p;

	//Left
	Map[s_i][(s_j + f_j) / 2][0] = Map[s_i][s_j][0];
	p = (3 * s_i - f_i) / 2;
	if (p < 0)
		p = 0;
	float random = Rand_generate(rand_answer);
	Map[s_i][(s_j + f_j) / 2][1] = (Map[p][(s_j + f_j) / 2][1] + Map[s_i][s_j][1] + Map[(s_i + f_i) / 2][(s_j + f_j) / 2][1] + Map[s_i][f_j][1]) / 4 + random*kof / 2;
	Map[s_i][(s_j + f_j) / 2][2] = (Map[s_i][s_j][2] + Map[s_i][f_j][2]) / 2;

	//Top
	Map[(s_i + f_i) / 2][s_j][0] = (Map[s_i][s_j][0] + Map[f_i][s_j][0]) / 2;
	p = (3 * s_j - f_j) / 2;
	if (p < 0)
		p = 0;
	random = Rand_generate(rand_answer);
	Map[(s_i + f_i) / 2][s_j][1] = (Map[s_i][s_j][1] + Map[(s_i + f_i) / 2][p][1] + Map[f_i][s_j][1] + Map[(s_i + f_i) / 2][(s_j + f_j) / 2][1]) / 4 + random*kof / 2;
	Map[(s_i + f_i) / 2][s_j][2] = Map[s_i][s_j][2];

	//Right
	Map[f_i][(s_j + f_j) / 2][0] = Map[f_i][s_j][0];
	p = (3 * f_i - s_i) / 2;
	if (p > num + 1)
		p = num + 1;
	random = Rand_generate(rand_answer);
	Map[f_i][(s_j + f_j) / 2][1] = (Map[(s_i + f_i) / 2][(s_j + f_j) / 2][1] + Map[f_i][s_j][1] + Map[p][(s_j + f_j) / 2][1] + Map[f_i][f_j][1]) / 4 + random*kof / 2;
	Map[f_i][(s_j + f_j) / 2][2] = (Map[f_i][s_j][2] + Map[f_i][f_j][2]) / 2;

	//Bottom
	Map[(s_i + f_i) / 2][f_j][0] = (Map[s_i][f_j][0] + Map[f_i][f_j][0]) / 2;
	p = (3 * f_j - s_j) / 2;
	if (p > num + 1)
		p = num + 1;
	random = Rand_generate(rand_answer);
	Map[(s_i + f_i) / 2][f_j][1] = (Map[s_i][f_j][1] + Map[(s_i + f_i) / 2][(s_j + f_j) / 2][1] + Map[f_i][f_j][1] + Map[(s_i + f_i) / 2][p][1]) / 4 + random*kof / 2;
	Map[(s_i + f_i) / 2][f_j][2] = Map[f_i][f_j][2];

}

void DS_AddCubes(int s_i, int s_j, int f_i, int f_j, int index, int length, int***& Cubes){

	//Second
	Cubes[length + 1][0][0] = (s_i + f_i) / 2; Cubes[length + 1][0][1] = s_j;
	Cubes[length + 1][1][0] = f_i; Cubes[length + 1][1][1] = (s_j + f_j) / 2;

	//Third
	Cubes[length + 2][0][0] = (s_i + f_i) / 2; Cubes[length + 2][0][1] = (s_j + f_j) / 2;
	Cubes[length + 2][1][0] = f_i; Cubes[length + 2][1][1] = f_j;

	//Fourth
	Cubes[length + 3][0][0] = s_i; Cubes[length + 3][0][1] = (s_j + f_j) / 2;
	Cubes[length + 3][1][0] = (s_i + f_i) / 2; Cubes[length + 3][1][1] = f_j;

	int S_I = s_i, S_J = s_j, F_I = f_i, F_J = f_j;

	//First
	Cubes[index][0][0] = S_I; Cubes[index][0][1] = S_J;
	Cubes[index][1][0] = (S_I + F_I) / 2; Cubes[index][1][1] = (S_J + F_J) / 2;

}

void DS_CreateMap(float***& Map, int***& Cubes, int devisions, float x_start, float y_start, float z_start, float x_finish, float y_finish, float z_finish){
	int i, j, num = DS_Func(devisions);
	DS_MakeBorders(Map, num);

	Map[1][1][0] = x_start;
	Map[1][1][1] = y_start;
	Map[1][1][2] = z_start;
	Map[num][1][0] = x_finish;
	Map[num][1][1] = (y_start + y_finish) / 2;
	Map[num][1][2] = z_start;
	Map[num][num][0] = x_finish;
	Map[num][num][1] = y_finish;
	Map[num][num][2] = z_finish;
	Map[1][num][0] = x_start;
	Map[1][num][1] = (y_start + y_finish) / 2;
	Map[1][num][2] = z_finish;

	Cubes[1][0][0] = Cubes[1][0][1] = 1;

	Cubes[1][1][0] = Cubes[1][1][1] = num;

	int l = 1, answer = 0;

	for (i = 1; i <= devisions; i++){

		for (j = 1; j <= l; j++){
			int value = 7 - (5 + devisions - i)*(5 + devisions - i) / 2;
			DS_Square(Cubes[j][0][0], Cubes[j][0][1], Cubes[j][1][0], Cubes[j][1][1], value, Map, answer);
		}

		for (j = 1; j <= l; j++){
			int value = 8 - (5 + devisions - i)*(5 + devisions - i) / 2;
			DS_Diamond(Cubes[j][0][0], Cubes[j][0][1], Cubes[j][1][0], Cubes[j][1][1], num, value, Map, answer);
		}

		int l_to_use = l;//size is changing

		for (j = 1; j <= l; j++){
			DS_AddCubes(Cubes[j][0][0], Cubes[j][0][1], Cubes[j][1][0], Cubes[j][1][1], j, l_to_use, Cubes);
			l_to_use += 3;
		}
		l = (int)pow((float)4, i);

	}


}