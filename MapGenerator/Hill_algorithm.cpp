#include "stdafx.h" 
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <cmath>
#include <vector>

#include "Hill_algorithm.h"
#include "Random.h"

int convert(char x)
{
	if (x == 'x') return -1;
	if (x == 'y') return -2;
	if (x == '+') return -3;
	if (x == '-') return -4;
	if (x == '*') return -5;
	if (x == '/') return -6;
	if (x == 's') return -7;
	if (x == 'c') return -8;
	if (x == 'q') return -9;
	if (x == '^') return -10;
	if (x == 'l') return -11;
	if (x == 'e') return -12;
	if (x == '(') return -13;
	return -100;
}

int prior(char x)
{
	if (x == -13) return 0;
	if (x == -3 || x == -4) return 1;
	if (x == -5 || x == -6) return 2;
	return 3;
}

std::vector < int > opn(std::string s)
{

	std::vector < int > res, tmp;
	int i, j, tmp_prior = 0;

	for (i = 0; i < (int)s.length(); i++)
	{
		if (s[i] == 'x' || s[i] == 'y')
		{
			res.push_back(convert(s[i]));
		}
		else if (s[i] >= '0' && s[i] <= '9')
		{
			int num = s[i] - '0'; j = i + 1;
			while (j < (int)s.length() && s[j] >= '0' && s[j] <= '9')
				num = num * 10 + s[j] - '0', j++;
			i = j - 1;
			res.push_back(num);
		}
		else if (s[i] == '(')
			tmp.push_back(convert(s[i]));
		else if (s[i] == ')')
		{
			while (tmp.back() != convert('('))
				res.push_back(tmp.back()), tmp.pop_back();
			tmp.pop_back();
			tmp_prior = 0; for (j = 0; j < (int)tmp.size(); j++)
				if (prior(tmp[j]) > tmp_prior) tmp_prior = prior(tmp[j]);
		}
		else if (s[i] == 's')
		{
			if (s[i + 1] == 'i')
			{
				//sin 's'
				std::string to_calc; int num = 0;
				for (j = i + 4; j < (int)s.length(); j++)
				{
					if (s[j] == ')') num--; else if (s[j] == '(') num++;
					if (num < 0) break;
					to_calc.push_back(s[j]);
				}
				std::vector < int > got = opn(to_calc);
				i = j;
				for (j = 0; j < (int)got.size(); j++)
					res.push_back(got[j]);
				res.push_back(convert('s'));
			}
			else
			{
				//sqrt 'q'                     
				std::string to_calc; int num = 0;
				for (j = i + 5; j < (int)s.length(); j++)
				{
					if (s[j] == ')') num--; else if (s[j] == '(') num++;
					if (num < 0) break;
					to_calc.push_back(s[j]);
				}
				std::vector < int > got = opn(to_calc);
				i = j;
				for (j = 0; j < (int)got.size(); j++)
					res.push_back(got[j]);
				res.push_back(convert('q'));
			}
		}
		else if (s[i] == 'c')
		{
			//cos 'c'
			std::string to_calc; int num = 0;
			for (j = i + 4; j < (int)s.length(); j++)
			{
				if (s[j] == ')') num--; else if (s[j] == '(') num++;
				if (num < 0) break;
				to_calc.push_back(s[j]);
			}
			std::vector < int > got = opn(to_calc);
			i = j;
			for (j = 0; j < (int)got.size(); j++)
				res.push_back(got[j]);
			res.push_back(convert('c'));
		}
		else if (s[i] == 'l')
		{
			//ln 'l'
			std::string to_calc; int num = 0;
			for (j = i + 3; j < (int)s.length(); j++)
			{
				if (s[j] == ')') num--; else if (s[j] == '(') num++;
				if (num < 0) break;
				to_calc.push_back(s[j]);
			}
			std::vector < int > got = opn(to_calc);
			i = j;
			for (j = 0; j < (int)got.size(); j++)
				res.push_back(got[j]);
			res.push_back(convert('l'));
		}
		else if (s[i] == 'e')
		{
			//exp 'e'
			std::string to_calc; int num = 0;
			for (j = i + 4; j < (int)s.length(); j++)
			{
				if (s[j] == ')') num--; else if (s[j] == '(') num++;
				if (num < 0) break;
				to_calc.push_back(s[j]);
			}
			std::vector < int > got = opn(to_calc);
			i = j;
			for (j = 0; j < (int)got.size(); j++)
				res.push_back(got[j]);
			res.push_back(convert('e'));
		}
		else if (s[i] == '^')
		{
			//^ '^'
			std::string to_calc; int num = 0;
			for (j = i + 2; j < (int)s.length(); j++)
			{
				if (s[j] == ')') num--; else if (s[j] == '(') num++;
				if (num < 0) break;
				to_calc.push_back(s[j]);
			}
			std::vector < int > got = opn(to_calc);
			i = j;
			for (j = 0; j < (int)got.size(); j++)
				res.push_back(got[j]);
			res.push_back(convert('^'));
		}
		else
		{
			if (tmp_prior < prior(convert(s[i])))
				tmp.push_back(convert(s[i])), tmp_prior = prior(convert(s[i]));
			else
			{
				while (!tmp.empty() && prior(tmp.back()) >= prior(convert(s[i])))
					res.push_back(tmp.back()), tmp.pop_back();
				tmp.push_back(convert(s[i])); tmp_prior = prior(convert(s[i]));
			}
		}
	}

	if (!tmp.empty()) for (i = (int)tmp.size() - 1; i >= 0; i--)
		res.push_back(tmp[i]);
	return res;
}

double calc(std::vector < int > c, int x, int y)
{
	std::vector < double > to_calc; int i, sz = 0;
	for (i = 0; i < (int)c.size(); i++)
	{
		if (c[i] == convert('x'))
			to_calc.push_back(x), sz++;
		else if (c[i] == convert('y'))
			to_calc.push_back(y), sz++;
		else if (c[i] == convert('+'))
		{
			to_calc[sz - 2] = to_calc[sz - 2] + to_calc[sz - 1]; to_calc.pop_back(); sz--;
		}
		else if (c[i] == convert('-'))
		{
			to_calc[sz - 2] = to_calc[sz - 2] - to_calc[sz - 1]; to_calc.pop_back(); sz--;
		}
		else if (c[i] == convert('*'))
		{
			to_calc[sz - 2] = to_calc[sz - 2] * to_calc[sz - 1]; to_calc.pop_back(); sz--;
		}
		else if (c[i] == convert('/'))
		{
			to_calc[sz - 2] = to_calc[sz - 2] / to_calc[sz - 1]; to_calc.pop_back(); sz--;
		}
		else if (c[i] == convert('^'))
		{
			to_calc[sz - 2] = pow(to_calc[sz - 2], to_calc[sz - 1]); to_calc.pop_back(); sz--;
		}
		else if (c[i] == convert('s'))
		{
			to_calc[sz - 1] = sin(to_calc[sz - 1]);
		}
		else if (c[i] == convert('c'))
		{
			to_calc[sz - 1] = cos(to_calc[sz - 1]);
		}
		else if (c[i] == convert('q'))
		{
			to_calc[sz - 1] = sqrt(to_calc[sz - 1]);
		}
		else if (c[i] == convert('l'))
		{
			to_calc[sz - 1] = log(to_calc[sz - 1]);
		}
		else if (c[i] == convert('e'))
		{
			to_calc[sz - 1] = exp(to_calc[sz - 1]);
		}
		else if (c[i] >= 0)
			to_calc.push_back(c[i]), sz++;
	}
	return to_calc.back();
}

void HILL_ResizeMap(float***& Map, int size){
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


void HILL_zoom_Height_Map(int a, float***& Map, float zoom_y)
{
	int i, j;
	for (i = 0; i < a + 5; i++)
		for (j = 0; j < a + 5; j++)
			Map[0][i][j] *= zoom_y;
}


void HILL_Centralize(int a, float***& Map){
	int i, j;
	for (i = 0; i < a + 5; i++)
		for (j = 0; j < a + 5; j++)
			Map[0][i][j] = (Map[0][i][j] - minH) / (maxH - minH);
}

void HILL_GenerateHolme(int a, float***& Map, int radius){

	int i, j;
	int xpoint, zpoint;
	xpoint = rand() % (a + 5);
	zpoint = rand() % (a + 5);
	for (i = max(1, xpoint - radius); i <= min(xpoint + radius, a + 4); i++)
		for (j = max(1, zpoint - radius); j <= min(zpoint + radius, a + 4); j++){
			int value;
			value = radius*radius - ((xpoint - i)*(xpoint - i) + (zpoint - j)*(zpoint - j));
			if (value>0)Map[0][i][j] += value;
			if (Map[0][i][j] > maxH)maxH = Map[0][i][j];
		}

}

void HILL_CreateMap(int a, float***& Map, int random_const, int random, int zoom){
	int i, j;
	long long number;
	minH = 0;
	maxH = 0;

	for (i = 0; i < a + 5; i++)
		for (j = 0; j < a + 5; j++)
		{
			Map[0][i][j] = 0;
		}

	int answer = 0;

	number = a*a;

	number *= 30000;

	number /= 1000000 - (random + random_const)*(random + random_const);

	while (number){

		number--;
		HILL_GenerateHolme(a, Map, (int)(abs(Rand_generate(answer)) * 2 * (random - random_const) + random_const));

	}

	HILL_Centralize(a, Map);
	HILL_zoom_Height_Map(a, Map, 20);
}


void CreateFunc(int a, float***& Map, std::string s){
	int i, j;

	s.erase(s.begin(), s.begin() + 9);

	std::vector< int > ss = opn(s);

	minH = 0;
	maxH = 0;

	for (i = 0; i < a + 5; i++)
		for (j = 0; j < a + 5; j++)
			Map[0][i][j] = 0;

	for (i = -a / 2; i < a / 2 + 5; i++)
		for (j = -a / 2; j < a / 2 + 5; j++)
		{
			double xx = calc(ss, i, j);
			Map[0][i + a / 2][j + a / 2] = (float) calc(ss, i, j);
			if (Map[0][i + a / 2][j + a / 2] > maxH) maxH = Map[0][i + a / 2][j + a / 2];
			if (Map[0][i + a / 2][j + a / 2] < minH) minH = Map[0][i + a / 2][j + a / 2];
		}

	HILL_Centralize(a, Map);
	HILL_zoom_Height_Map(a, Map, 20);
}