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
* 深度计算：平面方程代入x和扫描线对应的y可O(1)获得深度z
* 与2d的扫描线算法相似，APT动态维护要渲染的多边形对象，
* 每个多边形对象对应一个AET
*/

// tuple
struct tuple {
	int a, b, c;
};

struct point {
	int x, y;
};

struct edge {
	double x, dx, y_max;
	bool operator < (const edge& a) {
		return x < a.x;
	}
};

struct poly {
	int a, b, c, d;	// 多边形所在平面方程
	std::vector<point> p;	// 顶点投影坐标（x，y）
	int Ymin, Ymax;
	std::list<edge> AET;					  // 活化边表
	std::vector<edge> NET[window_height + 5]; // 新边表
	tuple color;	// 填充颜色

	void get_NET();
} polygon[15];

int cnt_poly = 0;	// 多边形的个数

void poly::get_NET() {
	for (int i = 0; i < window_height + 5; i++) { this->NET[i].clear(); }
	this->Ymin = 1e9, this->Ymax = 0;
	int n = p.size();
	if (!n) return;
	point e0 = p[0];
	p.push_back(e0);	// 将第1个点作为第（n+1）个点加入顶点集合，以便构造边
	for (int i = 0; i < n; i++) {
		double x1 = p[i].x, y1 = p[i].y;
		double x2 = p[i + 1].x, y2 = p[i + 1].y;
		if (y1 > y2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}
		// 多边形在Y方向的分布区间
		this->Ymin = std::min(this->Ymin, int(y1));
		this->Ymax = std::max(this->Ymax, int(y2));

		double _k;
		if (fabs(x1 - x2) < 1e-2) { _k = 0; }
		else if (fabs(y1 - y2) < 1e-2) { _k = 1; }
		else { _k = (x2 - x1) / (y2 - y1); }
		this->NET[int(y1)].push_back({ x1, _k, y2 });
	}
	std::cout << "got NET\n";
}


std::vector<poly> Y_table[window_height + 5];	// 多边形Y表
std::list<poly> APT;	// 活化多边形表
int Y_min = 1e9, Y_max = 0;					// 扫描区间

void myInit();
void myReshape(int w, int h);
void myDisplay();

void set_polygon();
void get_Y_table();
void Z_Buffer();
bool check(edge, edge, int);

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

	glutInitWindowSize(window_width, window_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Ex3: Z-buffering Algorithm");

	myInit();
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);

	set_polygon();	// 设置各个多边形的相关信息


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

void myDisplay() {

	glClear(GL_COLOR_BUFFER_BIT);

	Z_Buffer();

	glutSwapBuffers();
	glFlush();
}

void set_polygon() {
	// 设置平面方程、顶点坐标、填充颜色
	polygon[0].p.push_back({ 335, 389 });
	polygon[0].p.push_back({ 335, 119 });
	polygon[0].p.push_back({ 405, 153 });
	polygon[0].p.push_back({ 403, 355 });
	polygon[0].c = 1;
	polygon[0].d = 1;
	polygon[0].color = { 0, 204, 255 };

	polygon[1].p.push_back({ 133, 208 });
	polygon[1].p.push_back({ 159, 184 });
	polygon[1].p.push_back({ 403, 355 });
	polygon[1].p.push_back({ 334, 389 });
	polygon[1].c = 1;
	polygon[1].d = 3;
	polygon[1].color = { 51, 143, 204 };

	polygon[2].p.push_back({ 155, 324 });
	polygon[2].p.push_back({ 128, 297 });
	polygon[2].p.push_back({ 334, 120 });
	polygon[2].p.push_back({ 405, 153 });
	polygon[2].c = 1;
	polygon[2].d = 2;
	polygon[2].color = { 17, 150, 238 };

	cnt_poly = 3;

	for (int i = 0; i < cnt_poly; i++) {
		polygon[i].get_NET();
	}
}

void get_Y_table() {
	for (int i = 0; i < window_height + 5; i++) { Y_table[i].clear(); }
	Y_min = 1e9, Y_max = 0;
	for (int i = 0; i < cnt_poly; i++) {
		Y_table[polygon[i].Ymin].push_back(polygon[i]);
		// 确定扫描区间
		Y_min = std::min(Y_min, polygon[i].Ymin);
		Y_max = std::max(Y_max, polygon[i].Ymax);
	}
	std::cout << "got Y_table\n";
}

tuple CB[window_width + 5];		// 帧缓存
int ZB[window_width + 5];	// 深度缓存

void Z_Buffer() {
	std::cout << "Z_Buffer start\n";
	get_Y_table();
	APT.clear();

	glBegin(GL_POINTS);
	//std::cout << "[" << Y_min << "," << Y_max << "]\n";
	for (int Y = Y_min; Y <= Y_max; Y++) {
		//std::cout << "Y=" << Y << "\n";
		// to init
		// 定义Z轴方向垂直于屏幕向外
		for (int i = 1; i <= window_width; i++) {
			CB[i] = { 127, 127, 127 };
			ZB[i] = -1e10;	// 负无穷大
		}

		for (poly mpoly : Y_table[Y]) {
			APT.push_back(mpoly);
		}	// 直接插入不考虑有序性
		// 对每个多边形更新其AET，并比较深度考虑渲染
		for (poly &mpoly : APT) {	// 取引用，对mpoly存在修改
			for (edge e : mpoly.NET[Y]) {
				mpoly.AET.push_back(e);
			}
			mpoly.AET.sort();

			std::vector<int> to_fill; to_fill.clear();
			int x_old = -1, x_new;
			edge _e;
			for (edge& e : mpoly.AET) {
				x_new = int(e.x + 0.5);
				if (x_old != x_new) to_fill.push_back(x_new);
				else if (check(e, _e, Y)) {
					to_fill.push_back(x_new);
				}
				_e = e;
				x_old = x_new;
				//std::cout << e.x << ' ';
			}

			int n_fill = to_fill.size();
			for (int i = 0; i < n_fill; i += 2) {
				for (int j = to_fill[i]; j <= to_fill[i + 1]; j++) {
					// ax+by+cz+d=0,默认c=1
					int z = -mpoly.d - mpoly.a * j - mpoly.b * Y;	// O(1)，无需增量式优化 
					// ？和扫描线求交也是O(1)
					// 但是需要确定和多边形的哪些边相交O(num(edge))
					if (z > ZB[j]) {
						ZB[j] = z;
						CB[j] = mpoly.color;
					}
				}
			}
			
			// 更新当前多边形的AET
			for (auto it = mpoly.AET.begin(); it != mpoly.AET.end();) {
				if (int(it->y_max) == Y) {
					it = mpoly.AET.erase(it);
				}
				else if (int(it->y_max) > Y) {
					it->x += it->dx;
					it++;
				}
			}
		}

		// 输出帧缓存
		for (int i = 1; i <= window_width; i++) {
			glColor3ub(CB[i].a, CB[i].b, CB[i].c);
			glVertex2d(i, Y);
		} 

		// 更新APT
		for (auto it = APT.begin(); it != APT.end();) {
			if (int(it->Ymax) == Y) {
				it = APT.erase(it);
			}
			else if (int(it->Ymax) > Y) {
				it++;
			}
		}
	}
	glEnd();
}

bool check(edge a, edge b, int Y) {
	bool ret = true;
	int cnt = 0;
	if (a.y_max == Y) cnt++;
	if (b.y_max == Y) cnt++;
	if (cnt == 1) ret = false;
	return ret;
}

/* 伪代码
Z_Buffer() {
	建立多边形Y表，初始化APT
	for (自下而上扫描，对于每条扫描线i) {
		初始化帧缓存以及深度缓存
		将对应扫描线i的，多边形Y表中的多边形加入到APT中
		for (对每个多边形) {
			1.依据NET更新其AET
			2.对（单个多边形的边与扫描线的）交点集合排序并进行配对，存储至to_fill之中（vector）
			3.根据to_fill更新帧缓存以及Z-Buffer，根据平面方程ax+by+cz+d=0（默认c=1）计算深度多边形在该点处的深度z，若z>ZB[j]则更新
			4.更新当前多边形的AET，删除y_max=Y的活化边
		}
		输出帧缓存
		更新APT，删除y_max=Y的活化多边形
	}
}
*/