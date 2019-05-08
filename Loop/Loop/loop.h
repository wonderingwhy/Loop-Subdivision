#pragma once  
#include <windows.h>          
#include <math.h>          
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
using namespace std;
typedef float REAL;
const REAL PI = acos(-1.0);
ofstream fout;
ifstream fin;
string fin_name = "model.obj";
string fout_name;
int n_vertex = 0;
REAL sqr(REAL x) {
	return x * x;
}
struct vec3f {
	REAL x, y, z;
	vec3f(REAL x = 0, REAL y = 0, REAL z = 0) :x(x), y(y), z(z) {}
	void print() {
		printf("%.2f %.2f %.2f\n", x, y, z);
	}
	//向量数乘
	vec3f operator * (const REAL num) const {
		return vec3f(x * num, y * num, z * num);
	}
	//向量点乘
	REAL operator * (const vec3f &other) const {
		return x * other.x + y * other.y + z * other.z;
	}
	//向量叉乘
	vec3f operator ^ (const vec3f &other) const {
		return vec3f(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
	}
	vec3f operator + (const vec3f &other) const {
		return vec3f(x + other.x, y + other.y, z + other.z);
	}
	vec3f operator - (const vec3f &other) const {
		return vec3f(x - other.x, y - other.y, z - other.z);
	}
	vec3f operator / (const REAL num) const {
		return vec3f(x / num, y / num, z / num);
	}
	//向量单位化
	vec3f normalize() {
		REAL length = sqrt(x * x + y * y + z * z);
		return vec3f(x / length, y / length, z / length);
	}
};
struct vec3i {
	int x, y, z;
	vec3i(int x = -1, int y = -1, int z = -1) :x(x), y(y), z(z) {}
};
struct Face {
	vec3i vertexes_id;
	Face(int x = 0, int y = 0, int z = 0) {
		vertexes_id.x = x;
		vertexes_id.y = y;
		vertexes_id.z = z;
	}
	void print() {
		fout << "f " << vertexes_id.x + 1 << " " << vertexes_id.y + 1 << " " << vertexes_id.z + 1 << endl;
	}
};
struct Vertex {
	vec3f position;
	Vertex(REAL x = 0, REAL y = 0, REAL z = 0) {
		position.x = x;
		position.y = y;
		position.z = z;
	}
	void print() {
		fout << "v " << position.x << " " << position.y << " " << position.z << endl;
	}
};
vector<Face> tot_faces;
vector<Vertex> tot_vertexes;
vector<vector<pair<int, vec3i>>> edge_vertexes;