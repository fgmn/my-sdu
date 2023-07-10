#include <GL\glut.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#define window_width 500
#define window_height 500

int mode;
int line[100][4], cnt_line = 0;
int x_centre, y_centre, r;

void myInit();
void myReshape(int w, int h);
void myDisplay();
void myKeyboard(unsigned char key, int x, int y);
void myMouse(int button, int state, int x, int y);
void myMotion(int x, int y);

void line_DDA();
void line_Bresenham();
void circle();
void circle_point(int x, int y);

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ex1: Line and Circle Algorithm");

	myInit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);

	glutMainLoop();
	return 0;
}

void myInit() {
	glClearColor(0.1, 0.1, 0.4, 0);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1, -1, -1, -1);
}

void myReshape(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_MODELVIEW);
}

void myKeyboard(unsigned char key, int x, int y) {
	mode = key - '0';
}

void myMouse(int button, int state, int x, int y) {
	y = window_height - y;
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		line[cnt_line][0] = x;
		line[cnt_line][1] = y;
		x_centre = x; y_centre = y;
	}
	if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
		line[cnt_line][2] = x;
		line[cnt_line][3] = y;
		cnt_line++;
		r = sqrt((x - x_centre) * (x - x_centre) + (y - y_centre) * (y - y_centre));
		glutPostRedisplay();
	}
	if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON) {
		r = sqrt((x - x_centre) * (x - x_centre) + (y - y_centre) * (y - y_centre));
		glutPostRedisplay();
	}
}

void myMotion(int x, int y) {
	// 在鼠标按下并移动时调用
	y = window_height - y;
	line[cnt_line][2] = x;
	line[cnt_line][3] = y;
	//r = sqrt((x - x_centre) ^ 2 + (y - y_centre) ^ 2);	// err!
	glutPostRedisplay();
}

void myDisplay() {
	switch (mode)
	{
	case 1: line_DDA();
		break;
	case 2: line_Bresenham();
		break;
	case 3: circle();
		cnt_line = 0;
		break;
	default:
		break;
	}
	glFlush();	// 清空缓冲区
}

void line_DDA() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < cnt_line; i++) {
		glBegin(GL_POINTS);
		glColor3f(1, 0, 0);
		int x1 = line[i][0], y1 = line[i][1];
		int x2 = line[i][2], y2 = line[i][3];

		// 选择采样精度更高的微分方向
		int step = std::max(abs(x1 - x2), abs(y1 - y2));
		float dx = 1.0 * (x2 - x1) / step;
		float dy = 1.0 * (y2 - y1) / step;

		float x = x1, y = y1;
		for (int j = 0; j <= step; j++) {
			glVertex2d(x, int(y + 0.5));
			x += dx, y += dy;
		}
		glEnd();
	}
	glutSwapBuffers();	// 交换缓冲区（双缓冲区技术）
}

void line_Bresenham() {
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < cnt_line; i++) {
		glBegin(GL_POINTS);
		glColor3f(0, 1, 0);
		int x1 = line[i][0], y1 = line[i][1];
		int x2 = line[i][2], y2 = line[i][3];

		int dx = x2 - x1, dy = y2 - y1;
		int e = -dx, x = x1, y = y1;
		int ux = dx > 0 ? 1 : -1;
		int uy = dy > 0 ? 1 : -1;
		// 仅有整数运算
		dx = abs(dx); dy = abs(dy);
		if (dx > dy) {
			while (x != x2) {
				glVertex2d(x, y);
				e += 2 * dy;
				if (e > 0) {
					y += uy;
					e -= 2 * dx;
				}
				x += ux;
			}
		}
		else if (dx <= dy) {
			e = -dy;
			while (y != y2) {
				glVertex2d(x, y);
				e += 2 * dx;
				if (e > 0) {
					x += ux;
					e -= 2 * dy;
				}
				y += uy;
			}
		}
		glVertex2d(x, y);
		glEnd();
	}
	glutSwapBuffers();
}

void circle() {
	//std::cout << "(x,y,r) = (" << x_centre << ',' << y_centre << ',' << r << ")\n";
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_POINTS);
	glColor3f(0, 1, 0);
	if (r < 0) return;
	int x = 0, y = r;
	float d = 1.25 - r;
	do {
		circle_point(x, y);
		if (d < 0) {
			d += 2 * x + 3;
		}
		else {
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
	} while (x <= y);
	circle_point(x, y);
	glEnd();
	glutSwapBuffers();
}

void circle_point(int x, int y) {
	glVertex2d(x_centre + x, y_centre + y); glVertex2d(x_centre + y, y_centre + x);
	glVertex2d(x_centre - x, y_centre + y); glVertex2d(x_centre + y, y_centre - x);
	glVertex2d(x_centre + x, y_centre - y); glVertex2d(x_centre - y, y_centre + x);
	glVertex2d(x_centre - x, y_centre - y); glVertex2d(x_centre - y, y_centre - x);
}