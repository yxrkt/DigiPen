///////////////////////////////////////////////////////////////////////
// $Id: framework.cpp 1210 2008-02-05 17:45:17Z gherron $
//
// Provides the framework for graphics projects. Opens a window and
// hooks up all GUI callbacks.
//
// Gary Herron
//
// Copyright © 2007 DigiPen Institute of Technology
////////////////////////////////////////////////////////////////////////

#include "framework.h"
#include "geomlib.h"
#include "transformation.h"
#include "scenelib.h"


// Some globals used by the GUI framework.
int mouseX, mouseY;
bool leftDown = false;
bool middleDown = false;
bool rightDown = false;

float spin = -135.0;
float tilt = -60.0;
float translatex = 0.0;
float translatey = 0.0;
float zoom = 0.25;

Scene scene;

////////////////////////////////////////////////////////////////////////
// Sets the scene's viewing and projection matrices AND their inverses
// according to the parameters controlled by mouse interactions.
void BuildProjection(Scene& scene, int width, int height)
{
	double sx, sy;
	if(width < height) {
		sx = 0.4f;
		sy = sx * height / width; }
	else {
		sy = 0.4f;
		sx = sy * width / height; }

	scene.projection.Identity();
	scene.projection.Perspective(sx, sy, 0.1, 100.0);
	scene.projection.ComputeInverse();

	scene.viewing.Identity();
	scene.viewing.Translate(0.0, 0.0, -3.0);
	scene.viewing.Translate(0.01*translatex, -0.01*translatey, 0.25*zoom);
	scene.viewing.RotateX(tilt);
	scene.viewing.RotateZ(spin);
	scene.viewing.ComputeInverse();

}

////////////////////////////////////////////////////////////////////////
// Displays the current scene as described by the VertexTable vertices
// and the PolygonTable polygons and transformed by values spin and
// tilt.
void ReDrawScene()
{
	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	int width = viewport[2];
	int height = viewport[3];

	glClearColor(1.0,1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, 0, -1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	BuildProjection(scene, width, height);
	glDisable(GL_DEPTH_TEST);
	DrawScene(scene, width, height);

	glutSwapBuffers();
}

void Update()
{
        ReDrawScene();
		glutIdleFunc(NULL);
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when the window size is changed.
void ReshapeWindow(int w, int h)
{
	if (w && h) {
		glViewport(0, 0, w, h); }
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when there are keyboard messages.
void KeyboardInput(unsigned char key, int, int)
{

	switch(key) {
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		sceneID = key - '0';
		scene.Clear();
	    CreateScene(scene);
	    PreprocessScene(scene);
		glutIdleFunc(&Update);
	    break;
	case 'g':
		scene.UseOpenGLRendering = !scene.UseOpenGLRendering;
		glutIdleFunc(&Update);
		break;
	case 'a':
		scene.UseAntiAliasing = !scene.UseAntiAliasing;
		glutIdleFunc(&Update);
		break;
	case 27:					// Escape key
	case 'q':
		exit(EXIT_SUCCESS);
	}
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when a mouse button changes state.
// button:	0 (GL_LEFT_BUTTON)	 for a left button click
//			1 (GL_MIDDLE_BUTTON) for a wheel click
//			2 (GL_RIGHT_BUTTON)	 for a right button click
//			3					 for a wheel roll up
//			4					 for a wheel roll down
//
// state: 0 button went down
//		  1 button went up
//
// x,y: The position of the mouse.
//
// Note: a scroll wheel action will generate two calls here, the first
// with state == 0 and the second with state == 1, and both with
// button equal to 3 or 4 depending on the direction of scroll.
void MouseButton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		leftDown = (state == GLUT_DOWN);

	if (button == GLUT_MIDDLE_BUTTON)
		middleDown = (state == GLUT_DOWN);

	if (button == GLUT_RIGHT_BUTTON)
		rightDown = (state == GLUT_DOWN);

	if (button == 3)
		zoom += 1;

	if (button == 4)
		zoom -= 1;

	mouseX = x;
	mouseY = y;

	scene.UseFastRendering = leftDown || middleDown || rightDown;

	glutIdleFunc(&Update);
}

////////////////////////////////////////////////////////////////////////
// Called by GLut when a mouse moves (while a button is doen)
void MouseMotion(int x, int y)
{
	// This uses changes in x mouse position to spin the model around
	// its z axis, and changes in the y axis to tilt the model forward
	// or backward.
	if(leftDown) {
		spin += (x - mouseX);
		tilt += (y - mouseY); }

	if(rightDown) {
		translatex += (x - mouseX);
		translatey += (y - mouseY); }
	// Record this position 
	mouseX = x;
	mouseY = y;

	// Draw the scene, transformed by the new spin and tilt values.
	glutIdleFunc(&Update);

}

////////////////////////////////////////////////////////////////////////
// Called once by the main program to do the OpenGL/GLut setup and
// then enter the interactive loop.
int main(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(100, 100);//480 480
    glutInit(&argc, argv);
	glutCreateWindow("Class Framework");

	glutDisplayFunc(&ReDrawScene);
	glutReshapeFunc(&ReshapeWindow);
	glutKeyboardFunc(&KeyboardInput);
	glutMouseFunc(&MouseButton);
	glutMotionFunc(&MouseMotion);
	glutIdleFunc(&Update);

	CreateScene(scene);
	PreprocessScene(scene);

	// This function enters an event loop, and never returns.
	glutMainLoop();
}

