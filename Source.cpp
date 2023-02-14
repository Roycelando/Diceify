// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

// Source.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define FILENAME "SmileHikaru.jpg"

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <freeglut.h>
#include <FreeImage.h>
#include <iostream>
void drawDiceFace(int value, int i, int j);
void drawDice(int i, int j, GLubyte colour);

//the pixel structure
typedef struct {
	GLubyte r, g, b;
} pixel;

//the global structure
typedef struct {
	pixel* data;
	int w, h;
} glob;

typedef struct {
	int number;
	int s;
	int w, h;
} dice;

glob global;
const float PI = 3.14159265358979323846;

enum { MENU_FILTER, MENU_SAVE, MENU_TRIANGLE, MENU_QUIT };
dice die;
double darkest = 255;
double brightest = 0;
double colourInc = 0;

//read image
pixel* read_img(char* name, int* width, int* height) {
	FIBITMAP* image;
	int i, j, pnum;
	RGBQUAD aPixel;
	pixel* data;

	if ((image = FreeImage_Load(FIF_JPEG, name, JPEG_DEFAULT)) != NULL) {
		*width = FreeImage_GetWidth(image);
		*height = FreeImage_GetHeight(image);

		std::cout << "width: " << *width << " height: " << *height << " BPP: " << FreeImage_GetBPP(image) << std::endl;
		data = (pixel*)malloc((*height) * (*width) * sizeof(pixel*));
		if (data == NULL) {
			return NULL;
		}
		pnum = 0;
		for (i = 0; i < (*height); i++) {
			for (j = 0; j < (*width); j++) {
				FreeImage_GetPixelColor(image, j, i, &aPixel);
				GLubyte avg = ((aPixel.rgbRed) + (aPixel.rgbGreen) + (aPixel.rgbRed)) / (GLubyte)3.0;

				if (avg > brightest) {
					brightest = avg;
				}

				if (avg < darkest) {
					darkest = avg;
				}


				data[pnum].r = avg;
				data[pnum].g = avg;
				data[pnum++].b = avg;


				/*
				data[pnum].r = aPixel.rgbRed;
				data[pnum].g = aPixel.rgbGreen;
				data[pnum++].b = aPixel.rgbBlue;

				*/
				
							}
		}
		std::cout << "Darkest: " << darkest << " Brightest: " << brightest << std::endl;
		FreeImage_Unload(image);
		return data;
	}
	return NULL;
}//read_img


 //write_img
void write_img(char* name, pixel* data, int width, int height) {
	FIBITMAP* image;
	RGBQUAD aPixel;
	int i, j;

	image = FreeImage_Allocate(width, height, 24, 0, 0, 0);
	if (!image) {
		perror("FreeImage_Allocate");
		return;
	}
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			aPixel.rgbRed = data[i * width + j].r;
			aPixel.rgbGreen = data[i * width + j].g;
			aPixel.rgbBlue = data[i * width + j].b;

			FreeImage_SetPixelColor(image, j, i, &aPixel);
		}
	}
	if (!FreeImage_Save(FIF_TIFF, image, name, 0)) {
		perror("FreeImage_Save");
	}
	FreeImage_Unload(image);
}//write_img


 /*draw the image - it is already in the format openGL requires for glDrawPixels*/
void display_image(void)

{
	glClear(GL_COLOR_BUFFER_BIT);
	//glDrawPixels(global.w, global.h, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*)global.data);

	die.s = 30;
	die.w = global.w / die.s;
	die.h = global.h / die.s;

	global.w = global.w - (global.w % die.s);
	global.h = global.h - (global.h % die.s);


	std::cout << "dice side: " << die.s << " dice per row: " << die.h << " diece per row: " << die.w << std::endl;


	double avgColour = 0;

	/*
		pixel test;
		test.r = 0;
		test.g = 255;
		test.b = 0;

	*/





	for (int i = 0; i <= global.h - die.s; i += die.s) {
		for (int j = 0; j <= global.w - die.s; j += die.s) {

			int red = rand() % 255;
			int green = rand() % 255;
			int blue = rand() % 255;
			/* testing to see if  I can colour the verticies of each quadrant
					  global.data[((i * global.w)) + j].r =(GLubyte)255;
					  global.data[((i * global.w)) + j].g =(GLubyte)0;
					  global.data[((i * global.w)) + j].b =(GLubyte)0;

			*/


			for (int y = 0; y <= die.s; y++) {
				for (int x = 0; x <= die.s; x++) {

					avgColour += global.data[(((i + y) * global.w)) + (j + x)].r;

					/* testing to see if I can colour each qudarant
					global.data[(((i + y) * global.w)) + (j + x)].r = test.r;
					global.data[(((i + y) * global.w)) + (j + x)].g = test.g;
					global.data[(((i + y) * global.w)) + (j + x)].b = test.b;

					*/



				}

			}
			avgColour /= (double)(die.s * die.s);

			std::cout << "average colour: " << (double)avgColour << std::endl;
			glColor3f(avgColour / 255.0, avgColour / 255.0, avgColour / 255.0);
			drawDice(i, j, avgColour);




			avgColour = 0;
		}

	}


	std::cout << "Done" << std::endl;
	glFlush();
}//display_image()

void drawDice(int i, int j, GLubyte colour) {



	int value = 5;
	

	glColor3f(0, 0, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_POLYGON);
	glVertex2i(j, i);
	glVertex2i(j + die.s, i);
	glVertex2i(j + die.s, i + die.s);
	glVertex2i(j, i + die.s);
	glEnd();

	switch (value) {
	case 1: {
		drawDiceFace(1,i,j);
		break;
	}
   case 2: {
		drawDiceFace(2,i,j);

		break;
	}case 3: {
		drawDiceFace(1,i,j);
		drawDiceFace(2,i,j);

		break;
	}case 4: {
      drawDiceFace(3,i,j);

		break;
	}case 5: {
		drawDiceFace(1,i,j);
		drawDiceFace(3,i,j);

		break;
	}case 6: {
		drawDiceFace(4, i, j);
		break;
	}
	}



}

void drawDiceFace(int number, int i, int j) {
	double scale = die.s * 0.15;
	switch (number) {

	case 1: {
		glColor3f(0,0,0);
		glPointSize(scale);
		glBegin(GL_POINTS);
			glVertex2i(j+(die.s/2.0),i+(die.s/2.0));
		glEnd();

		break;
	}
	case 2: {
	glColor3f(0,0,0);
		glPointSize(scale);
		glBegin(GL_POINTS);
			glVertex2i(j+(die.s/4.0),i+((die.s*3)/4.0));
			glVertex2i(j+((die.s*3)/4.0),i+(die.s/4.0));
		glEnd();

		break;

	}case 3: {

	glColor3f(0,0,0);
		glPointSize(scale);
		glBegin(GL_POINTS);
			glVertex2i(j+(die.s/4.0),i+(die.s/4.0));
			glVertex2i(j+(die.s/4.0),i+((die.s*3)/4.0));
			glVertex2i(j+((die.s*3)/4.0),i+(die.s/4.0));
			glVertex2i(j+((die.s*3)/4.0),i+((die.s*3)/4.0));

	glEnd();

		break;

	}case 4: {
		glColor3f(0,0,0);
		glPointSize(scale);
		glBegin(GL_POINTS);

			glVertex2i(j+(die.s/4.0),i+(die.s/4.0));
			glVertex2i(j+(die.s/4.0),i+((die.s*3)/4.0));
			glVertex2i(j+((die.s*3)/4.0),i+(die.s/4.0));
			glVertex2i(j+((die.s*3)/4.0),i+((die.s*3)/4.0));
			glVertex2i(j+(die.s/4.0),i+(die.s/2.0));
			glVertex2i(j+((die.s*3)/4.0),i+(die.s/2.0));

		glEnd();

		break;

	}

	}

}

// Read the screen image back to the data buffer after drawing to it
void draw_triangle(void)
{
	glDrawPixels(global.w, global.h, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*)global.data);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0, 0);
	glVertex2i(rand() % global.w, rand() % global.h);
	glColor3f(0, 1.0, 0);
	glVertex2i(rand() % global.w, rand() % global.h);
	glColor3f(0, 0, 1.0);
	glVertex2i(rand() % global.w, rand() % global.h);
	glEnd();
	glFlush();
	glReadPixels(0, 0, global.w, global.h, GL_RGB, GL_UNSIGNED_BYTE, (GLubyte*)global.data);
}

/* A simple thresholding filter.
*/
void MyFilter(pixel* Im, int myIm_Width, int myIm_Height) {
	int x, y;

	for (x = 0; x < myIm_Width; x++)
		for (y = 0; y < myIm_Height; y++) {
			if (Im[x + y * myIm_Width].b > 128)
				Im[x + y * myIm_Width].b = 255;
			else
				Im[x + y * myIm_Width].b = 0;

			if (Im[x + y * myIm_Width].g > 128)
				Im[x + y * myIm_Width].g = 255;
			else
				Im[x + y * myIm_Width].g = 0;

			if (Im[x + y * myIm_Width].r > 128)
				Im[x + y * myIm_Width].r = 255;
			else
				Im[x + y * myIm_Width].r = 0;
		}

	glutPostRedisplay();	// Tell glut that the image has been updated and needs to be redrawn
}//My_Filter


 /*glut keyboard function*/
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 0x1B:
	case'q':
	case 'Q':
		exit(0);
		break;
	case's':
	case'S':
		printf("SAVING IMAGE: backup.tif\n");
		write_img((char*)"backup.tif", global.data, global.w, global.h);
		break;
	case 't':
	case 'T':
		draw_triangle();
		break;
	case'f':
	case'F':
		MyFilter(global.data, global.w, global.h);
		break;
	}
}//keyboard


 //Glut menu callback function
void menuFunc(int value)
{
	switch (value) {
	case MENU_QUIT:
		exit(0);
		break;
	case MENU_SAVE:
		printf("SAVING IMAGE: backup.tif\n");
		write_img((char*)"backup.tif", global.data, global.w, global.h);
		break;
	case MENU_TRIANGLE:
		draw_triangle();
		break;
	case MENU_FILTER:
		MyFilter(global.data, global.w, global.h);
		break;
	}
}//menuFunc


void show_keys()
{
	printf("Q:quit\nF:filter\nT:triangle\nS:save\n");
}

//Glut menu set up
void init_menu()
{
	int sub_menu = glutCreateMenu(&menuFunc);
	glutAddMenuEntry("Triangle", MENU_TRIANGLE);
	glutAddMenuEntry("Filter", MENU_FILTER);

	int main_menu = glutCreateMenu(&menuFunc);
	glutAddSubMenu("Modify", sub_menu);
	glutAddMenuEntry("Save", MENU_SAVE);
	glutAddMenuEntry("Quit", MENU_QUIT);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


int main(int argc, char** argv)
{
	global.data = read_img((char*)FILENAME, &global.w, &global.h);
	if (global.data == NULL)
	{
		printf("Error loading image file %s\n", (char*)FILENAME);
		return 1;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

	glutInitWindowSize(global.w, global.h);
	glutCreateWindow("SIMPLE DISPLAY");
	glutDisplayFunc(display_image);
	gluOrtho2D(0, global.w, 0, global.h);

	glClearColor(255.0, 255.0, 255.0, 1.0);
	glutKeyboardFunc(keyboard);
	glMatrixMode(GL_PROJECTION);

	//init_menu();
	show_keys();

	glutMainLoop();

	return 0;
}
