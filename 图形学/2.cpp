#include <GL\glut.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>
#include <cmath>
#define window_width 512
#define window_height 512

/*
(51, 143, 204)
(17, 150, 238)
(0, 204, 255)
*/

/*
todo list:
1. 同时绘制多个多边形；
2. 交互式绘制
*/

char ctrl;
/*
'n'：绘制下一个多边形
'r'：重新绘制当前多边形
'b'：回退一步
'd': 开始绘制
's'：暂停绘制
*/
int cnt_poly = 0;

struct point {
	int x, y;
};

struct edge {
	double x, dx, y_max;
	bool operator < (const edge& a) {
		return x < a.x;
	}
};

std::vector<point> polygon[15];					// 顺/逆时针存储多边形顶点
std::list<edge> AET;						// 活性边表
std::vector<edge> NET[window_height + 5];	// 新边表
int Y_min = 1e9, Y_max = 0;					// 扫描区间

void myInit();
void myReshape(int w, int h);
void myDisplay();
void myKeyboard(unsigned char key, int x, int y);
void myMouse(int button, int state, int x, int y);
void myMotion(int x, int y);

void set_polygon();
void get_NET(std::vector<point>);		// polygon扫描转换得到新边表
void polygon_fill();	// 自下而上进行扫描，填充多边形内部
bool check(edge, edge, int);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ex2: Polygon Scan Transform and Fill");

	myInit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	set_polygon();
	glutMainLoop();
	return 0;
}

void myInit() {
	glClearColor(0.5, 0.5, 0.5, 0);
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
	std::cout << key << '\n';
	ctrl = key; 
	switch (ctrl)
	{
	case 'n':
		std::cout << "n\n";
		cnt_poly++;
		//std::cout << "polygon nums: " << cnt_poly << '\n';
		break;
	case 'r':
		polygon[cnt_poly].clear();
		break;
	case 'b':
		polygon[cnt_poly].pop_back();
		break;
	case 'd':
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

void myMouse(int button, int state, int x, int y) { 
	y = window_height - y; 
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		polygon[cnt_poly].push_back({ x, y });
		std::cout << "get (" << x << "," << y << ")\n";
	}
}
void myMotion(int x, int y) { glutPostRedisplay(); }

void myDisplay() {
	//set_polygon();
	//get_NET();
	//polygon_fill();
	std::cout << "display\n";
	if (ctrl != 'd') return;
	std::cout << "draw\n";
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < cnt_poly; i++) {
		get_NET(polygon[i]);
		polygon_fill();
	}

	glutSwapBuffers();
	glFlush();
}

void set_polygon() {
	//polygon.clear();
	//polygon.push_back({ 255, 400 });
	//polygon.push_back({ 50, 250 });
	//polygon.push_back({ 150, 50 });
	polygon[0].push_back({ 335, 389 });
	polygon[0].push_back({ 335, 119 });
	polygon[0].push_back({ 405, 153 });
	polygon[0].push_back({ 403, 355 });

	polygon[1].push_back({ 133, 208 });
	polygon[1].push_back({ 159, 184 });
	polygon[1].push_back({ 335, 321 });
	polygon[1].push_back({ 334, 389 });

	polygon[2].push_back({ 155, 324 });
	polygon[2].push_back({ 128, 297 });
	polygon[2].push_back({ 334, 120 });
	polygon[2].push_back({ 334, 193 });
	cnt_poly = 3;
}

void get_NET(std::vector<point> poly) {
	for (int i = 0; i < window_height + 5; i++) { NET[i].clear(); }
	Y_min = 1e9, Y_max = 0;
	int n = poly.size();
	if (!n) return;
	point e0 = poly[0];
	poly.push_back(e0);
	//std::cout << "vertexs: " << n << '\n';
	for (int i = 0; i < n; i++) {
		double x1 = poly[i].x, y1 = poly[i].y;
		double x2 = poly[i + 1].x, y2 = poly[i + 1].y;
		if (y1 > y2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}
		Y_min = std::min(Y_min, int(y1));
		Y_max = std::max(Y_max, int(y2));

		double _k;
		if (fabs(x1 - x2) < 1e-2) { _k = 0; }
		else if (fabs(y1 - y2) < 1e-2) { _k = 1; }
		else { _k = (x2 - x1) / (y2 - y1); }
		NET[int(y1)].push_back({ x1, _k, y2 });
		//std::cout << "(" << x1 << "," << _k << "," << y2 << ")\n";
	}
	std::cout << "got NET\n";
}

void polygon_fill() {
	AET.clear();	// 初始化活性边表为空
	glBegin(GL_POINTS);
	//std::cout << "[" << Y_min << ',' << Y_max << "]\n";
	for (int Y = Y_min; Y <= Y_max; Y++) {	// 各条扫描线
		for (edge e : NET[Y]) {	// 将新边有序插入活性表
			bool is_inserted = false;
			for (auto it = AET.begin(); it != AET.end(); it++) {
				if (it->x > e.x) {
					AET.insert(it, e);
					is_inserted = true;
					break;
				}
			}
			if (!is_inserted) AET.push_back(e);
		}
		AET.sort();	// 允许自相交

		// 填充（需考虑扫描线与多边形顶点相交的情形）
		std::vector<int> to_fill; to_fill.clear();
		int x_old = -1, x_new;
		edge _e;
		for (edge &e : AET) {
			x_new = int(e.x + 0.5);
			if (x_old != x_new) to_fill.push_back(x_new);
			else if (check(e, _e, Y)) {
				to_fill.push_back(x_new);
			}
			_e = e;
			x_old = x_new;
			//std::cout << e.x << ' ';
		}
		//std::cout << '\n';
		//std::cout << "to fill: ";
		//for (int x : to_fill) {
		//	std::cout << x << ' ';
		//}
		//std::cout << '\n';
		int n_fill = to_fill.size();
		for (int i = 0; i < n_fill; i += 2) {
			for (int j = to_fill[i]; j <= to_fill[i + 1]; j++) {
				glColor3ub(17, 150, 238);
				glVertex2d(j, Y);
			}
		}
		// 更新AET
		for (auto it = AET.begin(); it != AET.end();) {
			if (int(it->y_max) == Y) {
				it = AET.erase(it);
			}
			else if (int(it->y_max) > Y) {
				it->x += it->dx;
				it++;
			}
		}
	}
	glEnd();
	std::cout << "filled\n";
}

bool check(edge a, edge b, int Y) {
	bool ret = true;
	int cnt = 0;
	if (a.y_max == Y) cnt++;
	if (b.y_max == Y) cnt++;
	if (cnt == 1) ret = false;
	return ret;
}