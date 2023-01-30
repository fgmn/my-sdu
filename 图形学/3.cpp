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
* ��ȼ��㣺ƽ�淽�̴���x��ɨ���߶�Ӧ��y��O(1)������z
* ��2d��ɨ�����㷨���ƣ�APT��̬ά��Ҫ��Ⱦ�Ķ���ζ���
* ÿ������ζ����Ӧһ��AET
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
	int a, b, c, d;	// ���������ƽ�淽��
	std::vector<point> p;	// ����ͶӰ���꣨x��y��
	int Ymin, Ymax;
	std::list<edge> AET;					  // ��߱�
	std::vector<edge> NET[window_height + 5]; // �±߱�
	tuple color;	// �����ɫ

	void get_NET();
} polygon[15];

int cnt_poly = 0;	// ����εĸ���

void poly::get_NET() {
	for (int i = 0; i < window_height + 5; i++) { this->NET[i].clear(); }
	this->Ymin = 1e9, this->Ymax = 0;
	int n = p.size();
	if (!n) return;
	point e0 = p[0];
	p.push_back(e0);	// ����1������Ϊ�ڣ�n+1��������붥�㼯�ϣ��Ա㹹���
	for (int i = 0; i < n; i++) {
		double x1 = p[i].x, y1 = p[i].y;
		double x2 = p[i + 1].x, y2 = p[i + 1].y;
		if (y1 > y2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}
		// �������Y����ķֲ�����
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


std::vector<poly> Y_table[window_height + 5];	// �����Y��
std::list<poly> APT;	// �����α�
int Y_min = 1e9, Y_max = 0;					// ɨ������

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

	set_polygon();	// ���ø�������ε������Ϣ


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
	// ����ƽ�淽�̡��������ꡢ�����ɫ
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
		// ȷ��ɨ������
		Y_min = std::min(Y_min, polygon[i].Ymin);
		Y_max = std::max(Y_max, polygon[i].Ymax);
	}
	std::cout << "got Y_table\n";
}

tuple CB[window_width + 5];		// ֡����
int ZB[window_width + 5];	// ��Ȼ���

void Z_Buffer() {
	std::cout << "Z_Buffer start\n";
	get_Y_table();
	APT.clear();

	glBegin(GL_POINTS);
	//std::cout << "[" << Y_min << "," << Y_max << "]\n";
	for (int Y = Y_min; Y <= Y_max; Y++) {
		//std::cout << "Y=" << Y << "\n";
		// to init
		// ����Z�᷽��ֱ����Ļ����
		for (int i = 1; i <= window_width; i++) {
			CB[i] = { 127, 127, 127 };
			ZB[i] = -1e10;	// �������
		}

		for (poly mpoly : Y_table[Y]) {
			APT.push_back(mpoly);
		}	// ֱ�Ӳ��벻����������
		// ��ÿ������θ�����AET�����Ƚ���ȿ�����Ⱦ
		for (poly &mpoly : APT) {	// ȡ���ã���mpoly�����޸�
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
					// ax+by+cz+d=0,Ĭ��c=1
					int z = -mpoly.d - mpoly.a * j - mpoly.b * Y;	// O(1)����������ʽ�Ż� 
					// ����ɨ������Ҳ��O(1)
					// ������Ҫȷ���Ͷ���ε���Щ���ཻO(num(edge))
					if (z > ZB[j]) {
						ZB[j] = z;
						CB[j] = mpoly.color;
					}
				}
			}
			
			// ���µ�ǰ����ε�AET
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

		// ���֡����
		for (int i = 1; i <= window_width; i++) {
			glColor3ub(CB[i].a, CB[i].b, CB[i].c);
			glVertex2d(i, Y);
		} 

		// ����APT
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

/* α����
Z_Buffer() {
	���������Y����ʼ��APT
	for (���¶���ɨ�裬����ÿ��ɨ����i) {
		��ʼ��֡�����Լ���Ȼ���
		����Ӧɨ����i�ģ������Y���еĶ���μ��뵽APT��
		for (��ÿ�������) {
			1.����NET������AET
			2.�ԣ���������εı���ɨ���ߵģ����㼯�����򲢽�����ԣ��洢��to_fill֮�У�vector��
			3.����to_fill����֡�����Լ�Z-Buffer������ƽ�淽��ax+by+cz+d=0��Ĭ��c=1��������ȶ�����ڸõ㴦�����z����z>ZB[j]�����
			4.���µ�ǰ����ε�AET��ɾ��y_max=Y�Ļ��
		}
		���֡����
		����APT��ɾ��y_max=Y�Ļ�����
	}
}
*/