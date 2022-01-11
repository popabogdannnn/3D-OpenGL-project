#include<windows.h>
#include <GL/glew.h>
#include <gl/freeglut.h>
#include <cstdio>
#include "render.h"




int main(int argc, char** argv)
{
	for (int x = -4; x <= 4; x++) {
		for (int z = -4; z <= 4; z++) {
			blocks.push_back(Block(x, 0, z, 0));
		}
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
	//SetCursorPos(100 + WIN_WIDTH / 2, 100 + WIN_HEIGHT / 2);
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("Maincraft");
	glewInit();
	initiliaze_opengl();
	glutDisplayFunc(render_function);
	glutIdleFunc(render_function);
	glutKeyboardFunc(handle_keyboard);
	glutMouseFunc(handle_mouse);
	glutCloseFunc(cleanup);
	glutMainLoop();
	return 0;
}