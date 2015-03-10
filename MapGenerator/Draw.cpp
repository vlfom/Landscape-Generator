#include "stdafx.h" 
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <GL/glut.h>

#include "Draw.h"

void SetColor(float h, float r, float g, float b) {
	glColor3f(r + (3 * r / (r + g + b)) *(h / 90), g + (3 * g / (r + g + b)) *(h / 90), b + (3 * b / (r + g + b)) *(h / 150));
}

void DrawMap(float*** Map, int length, int x_start, int z_start, float x, float y, float z, float red, float green, float blue, float ***Random, float Zoom, int grid){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	int i, j,
		zoom = 1, zoom_y = 15,
		LENGTH_draw_x = 300, LENGTH_draw_z = 2000;

	float heightMan = 10;

	int x1 = int(x), z1 = int(z);
	if (x1 < 0)
		x1 = 0;
	if (z1 < 0)
		z1 = 0;
	if (x1 > 1000)
		x1 = 1000;
	if (z1 > 1000)
		z1 = 1000;

	glTranslatef(-(x), -(y), -(z));

	for (i = 2; i < length - 2; i++)
		for (j = 2; j < length - 2; j++)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glBegin(GL_POLYGON);

			SetColor(Map[0][i][j] * Zoom, (red + Random[i][j][0]) / 2, (green + Random[i][j][1]) / 2, (blue + Random[i][j][2]) / 2);
			glVertex3f(x_start + i, Map[0][i][j] * Zoom, z_start + j);

			SetColor(Map[0][i + 1][j] * Zoom, (red + Random[i][j][0]) / 2, (green + Random[i][j][1]) / 2, (blue + Random[i][j][2]) / 2);
			glVertex3f(x_start + i + 1, Map[0][i + 1][j] * Zoom, z_start + j);

			SetColor(Map[0][i + 1][j + 1] * Zoom, (red + Random[i][j][0]) / 2, (green + Random[i][j][1]) / 2, (blue + Random[i][j][2]) / 2);
			glVertex3f(x_start + i + 1, Map[0][i + 1][j + 1] * Zoom, z_start + j + 1);

			SetColor(Map[0][i][j + 1] * Zoom, (red + Random[i][j][0]) / 2, (green + Random[i][j][1]) / 2, (blue + Random[i][j][2]) / 2);
			glVertex3f(x_start + i, Map[0][i][j + 1] * Zoom, z_start + j + 1);

			glEnd();
		}

	if ( !grid )
		for (i = 2; i < length - 2; i++)
			for (j = 2; j < length - 2; j++)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

				glBegin(GL_POLYGON);

				glColor3f(0, 0, 0);

				glVertex3f(x_start + i, Map[0][i][j] * Zoom, z_start + j);

				glVertex3f(x_start + i + 1, Map[0][i + 1][j] * Zoom, z_start + j);

				glVertex3f(x_start + i + 1, Map[0][i + 1][j + 1] * Zoom, z_start + j + 1);

				glVertex3f(x_start + i, Map[0][i][j + 1] * Zoom, z_start + j + 1);

				glEnd();

			}
	else if (grid == 1)
	{
	}

	glPopMatrix();
	glutSwapBuffers();
}