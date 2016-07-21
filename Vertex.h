#pragma once

class Vertex
{
public:
	Vertex();
	Vertex(double _x, double _y, int _ind_vertex = -1);
	~Vertex();
	double get_x() const;
	double get_y() const;
	int get_ind_vertex() const;
	void set_x(double _x);
	void set_y(double _y);
	void set_ind_vertex(int _ind_vertex);
	void set_vertex(double _x, double _y, int _ind_vertex = -1);
	void display_parameters() const;
private:
	double x;
	double y;
	int ind_vertex;
};

