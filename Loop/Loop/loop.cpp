#include "loop.h"

int add_edge_vertexes(int vertex0_id, int vertex1_id, int vertex2_id) {
	int ret = -1;
	if (vertex0_id > vertex1_id) {
		swap(vertex0_id, vertex1_id);
	}
	int index = -1;
	for (int i = 0; i < edge_vertexes[vertex0_id].size(); ++i) {
		if (edge_vertexes[vertex0_id][i].first == vertex1_id) {
			index = i;
			break;
		}
	}
	if (index == -1) {
		ret = tot_vertexes.size();
		edge_vertexes[vertex0_id].push_back(make_pair(vertex1_id, vec3i(ret, vertex2_id, -1)));
		tot_vertexes.push_back(Vertex());
	}
	else {
		ret = edge_vertexes[vertex0_id][index].second.x;
		edge_vertexes[vertex0_id][index].second.z = vertex2_id;
	}
	return ret;
}
void divide_face() {
	string filename;
	vector<Face> new_faces;
	for (Face face : tot_faces)
	{
		int vertex0_id = face.vertexes_id.x;
		int vertex1_id = face.vertexes_id.y;
		int vertex2_id = face.vertexes_id.z;

		int vertex3_id = add_edge_vertexes(vertex0_id, vertex1_id, vertex2_id);
		int vertex4_id = add_edge_vertexes(vertex1_id, vertex2_id, vertex0_id);
		int vertex5_id = add_edge_vertexes(vertex0_id, vertex2_id, vertex1_id);

		new_faces.push_back(Face(vertex0_id, vertex3_id, vertex5_id));
		new_faces.push_back(Face(vertex3_id, vertex1_id, vertex4_id));
		new_faces.push_back(Face(vertex5_id, vertex4_id, vertex2_id));
		new_faces.push_back(Face(vertex5_id, vertex3_id, vertex4_id));
	}
	tot_faces = new_faces;
}
void new_vertexes_position() {
	for (int i = 0; i < n_vertex; ++i) {
		for (int j = 0; j < edge_vertexes[i].size(); ++j) {
			int vertex_id = edge_vertexes[i][j].first;
			int new_vertex0_id = edge_vertexes[i][j].second.x;
			int new_vertex1_id = edge_vertexes[i][j].second.y;
			int new_vertex2_id = edge_vertexes[i][j].second.z;
			if (new_vertex2_id == -1) {
				tot_vertexes[new_vertex0_id].position =
					(tot_vertexes[i].position + tot_vertexes[vertex_id].position) / 2;
			}
			else {
				tot_vertexes[new_vertex0_id].position =
					(tot_vertexes[i].position + tot_vertexes[vertex_id].position) * 3 / 8 +
					(tot_vertexes[new_vertex1_id].position + tot_vertexes[new_vertex2_id].position) / 8;
			}
		}
	}
}
void old_vertexes_position() {
	for (int i = 0; i < n_vertex; ++i) {
		for (int j = 0; j < edge_vertexes[i].size() && edge_vertexes[i][j].first > i; ++j) {
			edge_vertexes[edge_vertexes[i][j].first].push_back(
				make_pair(i, edge_vertexes[i][j].second));
		}
	}
	vector<vec3f> temp_positions;
	for (int i = 0; i < n_vertex; ++i) {
		int k = edge_vertexes[i].size();
		vector<int> adj_boundary;
		vec3f v3f;
		for (int j = 0; j < k; ++j) {
			int vertex_id = edge_vertexes[i][j].first;
			if (edge_vertexes[i][j].second.z == -1) {
				adj_boundary.push_back(vertex_id);
			}
			v3f = v3f + tot_vertexes[vertex_id].position;
		}
		if (adj_boundary.size() == 2) {
			temp_positions.push_back(tot_vertexes[i].position * 3 / 4 +
				(tot_vertexes[adj_boundary[0]].position + tot_vertexes[adj_boundary[1]].position) / 8);
		}
		else {
			REAL beta = (5.0 / 8 - sqr(3.0 / 8 + cos(2 * PI / k) / 4)) / k;
			temp_positions.push_back(tot_vertexes[i].position * (1 - k * beta) + v3f * beta);
		}
	}
	for (int i = 0; i < n_vertex; ++i) {
		tot_vertexes[i].position = temp_positions[i];
	}
}
void init() {
	edge_vertexes.clear();
	for (int i = 0; i < n_vertex; ++i) {
		vector<pair<int, vec3i>> vec;
		edge_vertexes.push_back(vec);
	}
}
void subdivision_loop() {
	init();
	divide_face();
	new_vertexes_position();
	old_vertexes_position();
	n_vertex = tot_vertexes.size();
}
void read(string fin_name) {
	string str;
	tot_faces.clear();
	fin.open(fin_name);
	while (getline(fin, str)) {
		istringstream is(str);
		string type;
		is >> type;
		if (type.size() == 1) {
			if (type[0] == 'v') {
				REAL x, y, z;
				is >> x >> y >> z;
				tot_vertexes.push_back(Vertex(x, y, z));
			}
			else if (type[0] == 'f') {
				vector<int> temp_points;
				string temp;
				while (is >> temp) {
					istringstream iss(temp);
					int x;
					iss >> x;
					temp_points.push_back(x - 1);
				}
				if (temp_points.size() >= 3) {
					tot_faces.push_back(Face(temp_points[0], temp_points[1], temp_points[2]));
				}
			}
		}
	}
	n_vertex = tot_vertexes.size();
	fin.close();
}
void write(string fout_name) {
	cout << endl;
	cout << fout_name << " Writing..." << endl;
	fout.open(fout_name);
	for (Vertex vertex : tot_vertexes) {
		vertex.print();
	}
	for (Face face : tot_faces) {
		face.print();
	}
	fout.close();
	cout << fout_name << " Writing Done!" << endl;
	cout << endl;
}
int main(int argc, char *argv[])
{
	read(fin_name);
	for (int i = 0; i < 8; ++i) {
		time_t begin = clock();
		subdivision_loop();
		time_t end = clock();
		cout << "*******************************" << endl;
		cout << "Vertexes: " << tot_vertexes.size() << " " << "Faces: " << tot_faces.size() << endl;
		cout << "Time Used: " << end - begin << "ms" << endl;
		write("model_div" + to_string(i + 1) + ".obj");
		if (tot_vertexes.size() > 4e5 || tot_faces.size() > 8e5) {
			break;
		}
		Sleep(1000);
	}
	system("pause");
	return 0;
}