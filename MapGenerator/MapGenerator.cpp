#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>

#include "GL/glui.h"
#include "GL/glut.h"

#include "Draw.h"
#include "Averaging.h"
#include "Hill_algorithm.h"
#include "Diamond_square.h"
#include "Perlin_map.h"
#include "Water_er.h"
#include "Water_erosion_XPB.h"
#include "Thermal_er.h"
#include "Perlin_noise.h"
#include "Perlin_map.h"

std::string text = "Function: ";

int
	main_window,
	ZZZoom,
	Grid,
	***Cubes,
	chosen = 0, chosen_gen = -1, chosen_proc = -1, chosen_func = -1,
	size = -1,
	Hill_size = -1, Hill_scale = -1,
	Diamond_size = 0, Diamond_iter = -1,
	Perlin_zoom = 0, Perlin_size = 0, Perlin_average = 0,
	Fwater_iter = 1, Pwater_iter = 1,
	Thermal_iter = 1,
	Perlin_un = 1,
	Average_iter = 1;

float
	***Map,
	***Random,
	x = 50, y = 20, z = 200,
	red, green, blue,
	Perlin_persistence = 0;

void InitOpenGl(GLvoid) {
	glShadeModel(GL_SMOOTH);
	glClearColor(0, 0, 0, 0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void ReSizeWindow(GLsizei width, GLsizei height) {
	if (!height)
		height = 1;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 2000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void DrawScene() {
	float Real_Zoom = ZZZoom / 100.0;

	DrawMap(Map, size, 0, 0, x, y, z, red, green, blue, Random, Real_Zoom, Grid);
}

void keyboard(unsigned char key, int a, int b) {
	switch (key) {
	case 'd':
		x += 3;
		glutPostRedisplay();
		break;
	case 'w':
		z -= 3;
		glutPostRedisplay();
		break;
	case 'a':
		x -= 3;
		glutPostRedisplay();
		break;
	case 's':
		z += 3;
		glutPostRedisplay();
		break;
	case ',':
		y += 3;
		glutPostRedisplay();
		break;
	case '.':
		y -= 3;
		glutPostRedisplay();
		break;
	}
}

void myGlutIdle(void) {
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}

const char text1[] = "Project 'GENERATION OF THREE-DIMENSIONAL LANDSCAPES'.";
const char text2[] = "Use keys:  w  a  s  d  ,  .  in landscape's window to move ";

void make_new_random() {
	Random = new float**[size];
	for (int i = 0; i < size; i++)
		Random[i] = new float *[size];

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			Random[i][j] = new float[3];

	const int RAND_RANGE = 20;
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++) {
			Random[i][j][0] = 2.0 * 0.4 / RAND_RANGE * (rand() % RAND_RANGE);
			Random[i][j][1] = 3.0 * 0.4 / RAND_RANGE * (rand() % RAND_RANGE);
			Random[i][j][2] = 1.0 * 0.4 / RAND_RANGE * (rand() % RAND_RANGE);
		}
	for (int x = 0; x < 2; x++)
		for (int k = 0; k < 3; k++)
			for (int i = 1; i < size - 1; i++)
				for (int j = 1; j < size - 1; j++) {
					float Sum = Random[i][j][k] + Random[i - 1][j][k] + Random[i + 1][j][k] + Random[i][j - 1][k] + Random[i][j + 1][k];
					Random[i][j][k] -= (5 * Random[i][j][k] - Sum) / 5;
					Random[i - 1][j][k] -= (5 * Random[i - 1][j][k] - Sum) / 5;
					Random[i + 1][j][k] -= (5 * Random[i + 1][j][k] - Sum) / 5;
					Random[i][j - 1][k] -= (5 * Random[i][j - 1][k] - Sum) / 5;
					Random[i][j + 1][k] -= (5 * Random[i][j + 1][k] - Sum) / 5;
				}
}

GLUI_Spinner *live_zoom;

void control_file_open(int control) {
	std::ifstream Read("Gen_Map.txt");
	Read >> size; size += 5;

	Map = new float**[3];
	for (int i = 0; i < 3; i++)
		Map[i] = new float *[size + 5];

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < size + 5; j++)
			Map[i][j] = new float[size + 5];

	for (int i = 2; i < size - 2; i++)
		for (int j = 2; j < size - 2; j++)
			Read >> Map[0][i][j];

	size -= 2;

	x = size / 2; z = 2 * size; y = 20; ZZZoom = 100; live_zoom->set_int_val(100);

	make_new_random();

	glutPostRedisplay();

	MessageBoxA(NULL, "File was successfully opened!", "Success", MB_OK);

	glutPostRedisplay();
}


void control_file_write(int control) {
	if (size <= 10) {
		MessageBoxA(NULL, "You cannot save an empty map!", "Error", MB_OK);
		return;
	}
	std::ofstream Write("Gen_Map.txt");
	Write << size - 5 << std::endl;
	for (int i = 2; i < size - 2; i++)
	{
		for (int j = 2; j < size - 2; j++)
			Write << Map[0][i][j] << " ";
		Write << std::endl;
	}
	MessageBoxA(NULL, "Save completed successfully!", "Success", MB_OK);
}

void control_gnlwithoutp(int control) {
	chosen = chosen_gen;
	if (chosen_gen == 0) {
		size = Hill_size;
		HILL_ResizeMap(Map, Hill_size);
		HILL_CreateMap(Hill_size, Map, 1, Hill_size / 15, Hill_scale);
		x = Hill_size / 2; z = 2 * Hill_size; y = 20; ZZZoom = 100; live_zoom->set_int_val(100);
		make_new_random();
	}
	else if (chosen_gen == 1) {
		Diamond_size = DS_Func(Diamond_iter);
		size = Diamond_size;
		DS_Create_Mass_Map(Map, Cubes, Diamond_iter);
		DS_CreateMap(Map, Cubes, Diamond_iter, 0, 0, -100, 200, 0, 100);

		float *** Map1;
		Map1 = new float **[3];

		for (int i = 0; i < 3; i++)
			Map1[i] = new float *[size + 5];

		for (int i = 0; i < 3; i++)
			for (int j = 0; j < size + 5; j++)
				Map1[i][j] = new float[size + 5];

		for (int i = 0; i < size + 5; i++)
			for (int j = 0; j < size + 5; j++)
				Map1[0][i][j] = 2 * Map[i][j][1];

		Map = Map1;

		Averaging(Map, size - 2);

		x = Diamond_size / 2; z = 2 * Diamond_size; y = 20; ZZZoom = 100; live_zoom->set_int_val(100);
		make_new_random();
	}
	else if (chosen_gen == 2) {
		size = Perlin_size;

		PERLIN_ResizeMap(Map, Perlin_size);
		PERLIN_CreateMap(Map, Perlin_size, Perlin_persistence, Perlin_average ? true : false);

		Averaging(Map, size - 3, 4);
		x = Perlin_size / 2; z = 2 * Perlin_size; y = 20; ZZZoom = 100; live_zoom->set_int_val(100);
		make_new_random();
	}
	else if (chosen_gen == 3) {
		size = Hill_size;
		HILL_ResizeMap(Map, Hill_size);
		CreateFunc(Hill_size, Map, text);
		x = Hill_size / 2; z = 2 * Hill_size; y = 20; ZZZoom = 100; live_zoom->set_int_val(100);
		make_new_random();
	}

	glutPostRedisplay();

}

void control_proc(int control) {
	if (chosen_proc == 0) {
		float ** Water = new float *[size + 2], ** Sediment = new float *[size + 2];

		for (int i = 0; i < size + 2; i++) {
			Water[i] = new float[size + 2];
			Sediment[i] = new float[size + 2];
		}

		for (int i = 0; i < size + 2; i++)
			for (int j = 0; j < size + 2; j++)
				Water[i][j] = Sediment[i][j] = 0;

		for (int i = 0; i < Fwater_iter; i++)
			Erosion_Water(Map, Water, Sediment, size);
	}
	else if (chosen_proc == 1) {
		float *** Water;		XPB_Resize(size + 5, Water);
		float *** Sediment;		XPB_Resize(size + 5, Sediment);
		float **** Flux;
		float *** Vector;		XPB_Resize(size + 5, Vector);
		Flux = new float ***[4];

		for (int i = 0; i < 4; i++)
			Flux[i] = new float **[size + 5];

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < size + 5; j++)
				Flux[i][j] = new float *[size + 5];

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < size + 5; j++)
				for (int l = 0; l < size + 5; l++)
					Flux[i][j][l] = new float[6];

		XPB_Water_erosion_main(Pwater_iter, size, Map, Water, Sediment, Flux, Vector, 0.5, 1, 1, 9.8, 0.01, 0.00000000001, 0.0000000001, 0.0000000001, 0.8, -100);
	}
	else if (chosen_proc == 2) {
		for (int i = 0; i < Thermal_iter; i++)
			Erosion_Thermal(size, Map);
	}
	else if (chosen_proc == 3) {
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++){
				int randi = rand() % size, randj = rand() % size;
				if (Perlin_un)
					Map[0][i][j] += PERLIN_Noise2D(i + randi, j + randj);
				else
					Map[0][i][j] += PERLIN_CentralizeNoise2D(i + randi, j + randj);
			}
	}
	else if (chosen_proc == 4) {
		for (int i = 0; i < Average_iter; i++)
			Averaging(Map, size);
	}
	glutPostRedisplay();
}

void control_gnlwithp(int control) {
	control_gnlwithoutp(control);
	control_proc(control);
}

void control_grid(int control) {
	Grid = true;
}


int main(int argc, char ** argv) {
	FreeConsole(); //hide the console

	setlocale(LC_ALL, "");
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);
	main_window = glutCreateWindow("Landscape Generation");
	InitOpenGl();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(ReSizeWindow);
	glutKeyboardFunc(keyboard);

	// GLUI code

	GLUI *WINDOW = GLUI_Master.create_glui("Generation of three-dimensional landscapes", 0, GetSystemMetrics(SM_CXSCREEN) - 850, 0);

	GLUI_StaticText *Text1 = new GLUI_StaticText(WINDOW, text1);
	GLUI_StaticText *Text2 = new GLUI_StaticText(WINDOW, text2);
	Text1->set_alignment(GLUI_ALIGN_CENTER);
	Text2->set_alignment(GLUI_ALIGN_CENTER);

	GLUI_Panel *MAIN = new GLUI_Panel(WINDOW, "", GLUI_PANEL_NONE);
	MAIN->set_alignment(GLUI_ALIGN_LEFT);

	/* Generation */
	GLUI_Panel *panel_gen = new GLUI_Panel(MAIN, "Algorithm of generation");
	chosen_gen = -1;
	GLUI_RadioGroup *WayGen = new GLUI_RadioGroup(panel_gen, &chosen_gen);
	new GLUI_RadioButton(WayGen, "Hill algorithm");
	new GLUI_RadioButton(WayGen, "Diamond - square algorithm");
	new GLUI_RadioButton(WayGen, "Perlin Noise");
	new GLUI_RadioButton(WayGen, "Function");
	panel_gen->set_alignment(GLUI_ALIGN_CENTER);

	/* Processing */
	GLUI_Panel *panel_proc = new GLUI_Panel(MAIN, "Algorithm of processing");
	chosen_proc = -1;
	GLUI_RadioGroup *WayProc = new GLUI_RadioGroup(panel_proc, &chosen_proc);
	new GLUI_RadioButton(WayProc, "Fast water erosion");
	new GLUI_RadioButton(WayProc, "Physic-used water erosion");
	new GLUI_RadioButton(WayProc, "Thermal erosion");
	new GLUI_RadioButton(WayProc, "Perlin noise");
	new GLUI_RadioButton(WayProc, "Uniforming");
	WayProc->set_alignment(GLUI_ALIGN_CENTER);

	GLUI_Panel *panel_draw = new GLUI_Panel(MAIN, "Way of drawing");
	Grid = 0;
	GLUI_RadioGroup *WayDraw = new GLUI_RadioGroup(panel_draw, &Grid);
	new GLUI_RadioButton(WayDraw, "With Grid");
	new GLUI_RadioButton(WayDraw, "Without Grid");
	WayDraw->set_alignment(GLUI_ALIGN_CENTER);

	GLUI_Panel *panel_color = new GLUI_Panel(MAIN, "Color");
	red = green = blue = 0;
	GLUI_Spinner *SpinRed = new GLUI_Spinner(panel_color, "Red", &red); SpinRed->set_float_limits(0, 1);
	GLUI_Spinner *SpinGreen = new GLUI_Spinner(panel_color, "Green", &green); SpinGreen->set_float_limits(0, 1);
	GLUI_Spinner *SpinBlue = new GLUI_Spinner(panel_color, "Blue", &blue); SpinBlue->set_float_limits(0, 1);
	panel_color->set_alignment(GLUI_ALIGN_CENTER);

	live_zoom = new GLUI_Spinner(MAIN, "Live zoom (in %)", &ZZZoom);
	live_zoom->set_int_limits(10, 999);
	live_zoom->set_speed(5);
	live_zoom->set_int_val(100);

	/* Properties */

	new GLUI_Column(MAIN, false);

	/* Hill */
	GLUI_Panel *hill_gen = new GLUI_Panel(MAIN, "Hill algorithm properties");
	GLUI_Spinner *hill_size = new GLUI_Spinner(hill_gen, "Size:", &Hill_size);
	hill_size->set_int_limits(200, 500);
	hill_gen->set_alignment(GLUI_ALIGN_CENTER);

	/* Diamond */
	GLUI_Panel *diamond_gen = new GLUI_Panel(MAIN, "Diamond-square algorithm properties");
	GLUI_Spinner *diamond_iter = new GLUI_Spinner(diamond_gen, "Size (map will have 2^n size)", &Diamond_iter);
	diamond_iter->set_int_limits(7, 10);
	diamond_gen->set_alignment(GLUI_ALIGN_CENTER);

	/* Perlin */
	GLUI_Panel *perlin_gen = new GLUI_Panel(MAIN, "Perlin's noise algorithm properties");
	GLUI_Spinner *perlin_size = new GLUI_Spinner(perlin_gen, "Size:", &Perlin_size);
	GLUI_Spinner *perlin_pers = new GLUI_Spinner(perlin_gen, "Persistence:", &Perlin_persistence);
	perlin_size->set_int_limits(128, 1024);
	perlin_pers->set_float_limits(0.7, 0.99);
	perlin_gen->set_alignment(GLUI_ALIGN_CENTER);

	/* Function */

	GLUI_StaticText *stattxt = new GLUI_StaticText(MAIN, "Type function here (don't erase the text 'Function'):");
	GLUI_EditText *edtxt = new GLUI_EditText(MAIN, "", text);
	edtxt->set_alignment(GLUI_ALIGN_CENTER);
	edtxt->set_w(400);

	GLUI_Panel *panel_create = new GLUI_Panel(MAIN, "Changing");
	new GLUI_Button(panel_create, "Generate new landscape without processing", 0, control_gnlwithoutp);
	new GLUI_Button(panel_create, "Process current landscape", 0, control_proc);
	GLUI_Panel *panel_file = new GLUI_Panel(MAIN, "File operations");
	new GLUI_Button(panel_file, "Save Map to File", 0, control_file_write);
	new GLUI_Button(panel_file, "Open Map from File", 0, control_file_open);

	new GLUI_Button(MAIN, "Quit", 0, (GLUI_Update_CB)exit);

	new GLUI_Column(MAIN, false);

	/* Fast Water */
	GLUI_Panel *fast_water = new GLUI_Panel(MAIN, "Fast water erosion properties");
	GLUI_Spinner *fwater_iter = new GLUI_Spinner(fast_water, "Iterations:", &Fwater_iter);
	fwater_iter->set_int_limits(1, 1000);
	fast_water->set_alignment(GLUI_ALIGN_CENTER);

	/* Physic Water */
	GLUI_Panel *physic_water = new GLUI_Panel(MAIN, "Physic water erosion properties");
	GLUI_Spinner *pwater_iter = new GLUI_Spinner(physic_water, "Iterations:", &Pwater_iter);
	pwater_iter->set_int_limits(1, 1000);
	physic_water->set_alignment(GLUI_ALIGN_CENTER);

	/* Thermal */
	GLUI_Panel *thermal_er = new GLUI_Panel(MAIN, "Thermal erosion properties");
	GLUI_Spinner *thermal_iter = new GLUI_Spinner(thermal_er, "Iterations:", &Thermal_iter);
	thermal_iter->set_int_limits(1, 1000);
	thermal_er->set_alignment(GLUI_ALIGN_CENTER);

	/* Perlin */
	GLUI_Panel *perlin_er = new GLUI_Panel(MAIN, "Perlin noise properties");
	GLUI_Checkbox *perlin_un = new GLUI_Checkbox(perlin_er, "Uniform noise", &Perlin_un);
	perlin_er->set_alignment(GLUI_ALIGN_CENTER);

	/* Uniforming */
	GLUI_Panel *averaging = new GLUI_Panel(MAIN, "Average properties");
	GLUI_Spinner *average_iter = new GLUI_Spinner(averaging, "Iterations:", &Average_iter);
	averaging->set_alignment(GLUI_ALIGN_CENTER);

	/*  */

	WINDOW->set_main_gfx_window(main_window);

	GLUI_Master.set_glutIdleFunc(NULL);

	glutMainLoop();

	return 0;
}