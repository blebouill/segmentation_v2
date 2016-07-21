#include <iostream>

#include "Vertex.h"



Vertex::Vertex()
{
	x = -1.0;
	y = -1.0;
	ind_vertex = -1;
}


Vertex::Vertex(double _x, double _y, int _ind_vertex)
{
	this->set_vertex(_x, _y, _ind_vertex);
}


Vertex::~Vertex()
{
}


double Vertex::get_x() const
{
	return x;
}


double Vertex::get_y() const
{
	return y;
}


int Vertex::get_ind_vertex() const
{
	return ind_vertex;
}


void Vertex::set_x(double _x)
{
	x = _x;
}


void Vertex::set_y(double _y)
{
	y = _y;
}


void Vertex::set_ind_vertex(int _ind_vertex)
{
	ind_vertex = _ind_vertex;
}


void Vertex::set_vertex(double _x, double _y, int _ind_vertex)
{
	ind_vertex = _ind_vertex;
	x = _x;
	y = _y;
}


void Vertex::display_parameters() const
{
	printf("Vertex %d \t - \t x = %f    \t ; \t y = %f\n", ind_vertex, x, y);
}
