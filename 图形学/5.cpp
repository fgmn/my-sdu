#include <GL\glut.h>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <list>
#include <vector>
#include <iomanip>
#define window_width 512
#define window_height 512

#define max_err 10		// ���ѡ����������ƫ��
#define delta_t 5e-5	// bezier���߻��ƾ���
#define c_color {1, 0, 0}
#define l_color {0, 1, 0}
#define b_color {0, 0, 1}

/*
���ƿ��ƶ����->����bezier����
*/

int mode, flag;

struct tuple {
	float a, b, c;
	tuple operator + (const tuple& x) {
		return { a + x.a, b + x.b, c + x.c };
	}
	tuple operator * (const int x) {
		return { a * x, b * x, c * x };
	}
	tuple operator / (const int x) {
		return { a / x, b / x, c / x };
	}
};

struct point { int x, y; };

std::vector<point> ctrl_point;	// ���Ƶ�
std::vector<point> bezier_point;	// bezier����
std::vector<point> bspline_point;	// B��������
int k = 3;	// ����


// �ص���������Start
void myInit();
void myReshape(int w, int h);
void myDisplay();
void myKeyboard(unsigned char key, int x, int y);
void myMouse(int button, int state, int x, int y);
void myMotion(int x, int y);
// �ص���������End

void de_Casteljau();
void de_Boor();
int get_point(int x, int y);	// ��ȡ����
void draw_point(int x, int y, tuple color);	// ���Ƶ�
void draw_line(std::vector<point>& draw_obj, tuple color);	// ������


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ex5: Bezier Curves and B-splines");

	myInit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);	// ֧�� insert/delete/move ���Ƶ�

	glutMainLoop();
	return 0;
}

void myInit() {
	glClearColor(0.5, 0.5, 0.5, 0);	// ����ɫ�趨
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
	switch (key)	// ģʽ�л�
	{
	case 'i':	// insert
		mode = 0;
		std::cout << "switch mode to: " << "insert\n";
		break;
	case 'd':	// delete
		mode = 1;
		std::cout << "switch mode to: " << "delete\n";
		break;
	case 'm':	// move
		mode = 2;
		std::cout << "switch mode to: " << "move\n";
		break;
	case 'b':	// bezier���� <-> B��������
		flag = flag ^ 1;
		if (flag) { std::cout << "apply de_Boor\n"; }
		else { std::cout << "apply de_Casteljau\n"; }
		glutPostRedisplay();	// ����glutDisplayFunc
		break;
	}
}

void myMouse(int button, int state, int x, int y) {
	y = window_height - y;
	if (mode == 0) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			int ret = get_point(x, y);
			if (ret == -1) {
				draw_point(x, y, c_color);
				ctrl_point.push_back({ x, y });
				std::cout << "get (" << x << "," << y << ")\n";
			}
		}
	}
	else if (mode == 1) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			int ret = get_point(x, y);
			if (ret != -1) {
				auto it = ctrl_point.begin();
				std::cout << "erase (" << (*(it + ret)).x << "," << (*(it + ret)).y << ")\n";
				ctrl_point.erase(it + ret);
			}
		}
	}
	glutPostRedisplay();	// ����glutDisplayFunc
}

void myMotion(int x, int y) {
	// ����갴�²��ƶ�ʱ����
	y = window_height - y;
	if (mode == 2) {
		int ret = get_point(x, y);
		if (ret != -1) {
			std::cout << "move (" << ctrl_point[ret].x << "," << ctrl_point[ret].y << ")";
			std::cout << " to (" << x << "," << y << ")\n";
			ctrl_point[ret].x = x;
			ctrl_point[ret].y = y;
		}
	}
	glutPostRedisplay();
}

void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);	// �ñ���ɫˢ��
	// ���ƿ��ƶ����
	draw_line(ctrl_point, l_color);

	// ���Ƶ���>=2������bezier����
	if (ctrl_point.size() >= 2) {
		if (flag) de_Boor();
		else de_Casteljau();
	}
	glutSwapBuffers();	// ����������
	glFlush();	// ��ջ�����
}

void de_Casteljau() {
	bezier_point.clear();
	std::vector<point> tmp;

	tmp = ctrl_point;
	int n = tmp.size();

	for (double t = 0; t <= 1; t += (delta_t / n)) {
		tmp = ctrl_point;
		for (int i = 1; i < n; i++) {	// ����i
			for (int j = 0; j < n - i; j++) {	// ��i�ֿ��Ƶ���Ϊ(n-i+1)
				tmp[j].x = (1 - t) * tmp[j].x + t * tmp[j + 1].x;
				tmp[j].y = (1 - t) * tmp[j].y + t * tmp[j + 1].y;
			}
		}
		bezier_point.push_back(tmp[0]);
	}

	draw_line(bezier_point, b_color);
}

#define DEBUG
void de_Boor() {
	bspline_point.clear();
	std::vector<point> tmp;

	tmp = ctrl_point;
	int n = tmp.size() - 1;	// ���ﶨ��nΪ���Ƶ������

	std::vector<double> c(1e5);	// �ڵ�ʸ��
	// �ǵݼ��������У�t0,t1,...,t(n+k)
	
	// ׼����B����
	//for (int i = 0; i < k; i++) c[i] = 0;
	//for (int i = k; i <= n; i++) c[i] = c[i - 1] + 1.0 / (n - k + 2);
	//for (int i = n + 1; i <= n + k; i++) c[i] = 1;

	// ����B����
	for (int i = 0; i <= n + k; i++) c[i] = i + 1;

	for (int j = k - 1; j <= n; j++) {
		for (double t = c[j]; t <= c[j + 1]; t += (delta_t / n)) {	// ��t�̶�������[c[j],c[j+1]]
			tmp = ctrl_point;
			for (int r = 1; r < k; r++) {	// (k-1)����
				for (int i = j; i > j - k + r; i--) {	// ���Ƶ����䣺[(j-k+1)+r,j]
					double x1, x2, y;
					x1 = t - c[i];
					x2 = c[i + k - r] - t;
					y = c[i + k - r] - c[i];	// x1/y��Pi + x2/y��P(i+1)

					double c1, c2;
					if (abs(x1) < 1e-9 && abs(y) < 1e-9) { c1 = 0; }
					else { c1 = x1 / y; }
					if (abs(x2) < 1e-9 && abs(y) < 1e-9) { c2 = 0; }
					else { c2 = x2 / y; }	// 0/0=0

					tmp[i].x = c1 * tmp[i].x + c2 * tmp[i - 1].x;
					tmp[i].y = c1 * tmp[i].y + c2 * tmp[i - 1].y;
				}
			}
			bspline_point.push_back(tmp[j]);
		}
	}

	draw_line(bspline_point, b_color);
}

int get_point(int x, int y) {
	int ret = -1, n = ctrl_point.size();
	double min_d = 1e10, d;
	for (int i = 0; i < n; i++) {
		int cx = ctrl_point[i].x, cy = ctrl_point[i].y;
		d = sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));
		if (d < min_d && d <= max_err) { min_d = d; ret = i; }
	}
	return ret;
}

void draw_point(int x, int y, tuple color) {
	glBegin(GL_POINTS);
	glColor3f(color.a, color.b, color.c);
	glVertex2d(x, y);
	glEnd();
}

void draw_line(std::vector<point> &draw_obj, tuple color) {
	glBegin(GL_LINE_STRIP);
	glColor3f(color.a, color.b, color.c);
	for (point &i : draw_obj) {	// C++11
		glVertex2f(i.x, i.y);
	}
	glEnd();
}


