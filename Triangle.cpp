#include <iostream>
#include <cmath>

#include "Triangle.h"



Triangle::Triangle()
{
	ind_triangle = -1;
	nb_pixels = 0;
	perimeter = 0.0;
	area = 0.0;
	energy_color = 0.0;
	energy_shape = 0.0;
}


Triangle::~Triangle()
{
}

int Triangle::get_ind_triangle() const
{
	return ind_triangle;
}

int Triangle::get_nb_pixels() const
{
	return nb_pixels;
}

double Triangle::get_perimeter() const
{
	return perimeter;
}

double Triangle::get_area() const
{
	return area;
}

double Triangle::get_energy_color() const
{
	return energy_color;
}

double Triangle::get_energy_shape() const
{
	return energy_shape;
}

std::vector<double> Triangle::get_color_mean() const
{
	return color_mean;
}

Vertex * Triangle::get_vertex_0() const
{
	return v_triangle_vertices[0];
}

Vertex * Triangle::get_vertex_1() const
{
	return v_triangle_vertices[1];
}

Vertex * Triangle::get_vertex_2() const
{
	return v_triangle_vertices[2];
}

std::vector<Vertex*> Triangle::get_triangle_vertices() const
{
	return v_triangle_vertices;
}

Edge * Triangle::get_edge_0() const
{
	return v_triangle_edges[0];
}

Edge * Triangle::get_edge_1() const
{
	return v_triangle_edges[1];
}

Edge * Triangle::get_edge_2() const
{
	return v_triangle_edges[2];
}

std::vector<Edge*> Triangle::get_triangle_edges() const
{
	return v_triangle_edges;
}

void Triangle::set_ind_triangle(int _ind_triangle)
{
	ind_triangle = _ind_triangle;
}

void Triangle::set_nb_pixels(int _nb_pixels)
{
	nb_pixels = _nb_pixels;
}

void Triangle::set_energy_color(double _energy_color)
{
	energy_color = _energy_color;
}

void Triangle::set_energy_shape(double _energy_shape)
{
	energy_shape = _energy_shape;
}

void Triangle::set_color_mean(std::vector<double> _color_mean)
{
	color_mean = _color_mean;
}

void Triangle::set_edges(Edge * _edge_0, Edge * _edge_1, Edge * _edge_2)
{
	int ind_vertex_0, ind_vertex_1, ind_vertex_2;

	v_triangle_edges.clear();

	v_triangle_edges.push_back(_edge_0);
	v_triangle_edges.push_back(_edge_1);
	v_triangle_edges.push_back(_edge_2);

	v_triangle_vertices.clear();

	v_triangle_vertices.push_back(_edge_0->get_vertex_0());
	v_triangle_vertices.push_back(_edge_0->get_vertex_1());

	ind_vertex_0 = (_edge_0->get_vertex_0())->get_ind_vertex();
	ind_vertex_1 = (_edge_0->get_vertex_1())->get_ind_vertex();
	ind_vertex_2 = (_edge_1->get_vertex_0())->get_ind_vertex();

	if ((ind_vertex_2 == ind_vertex_0) || (ind_vertex_2 == ind_vertex_1))
		v_triangle_vertices.push_back(_edge_1->get_vertex_1());
	else
		v_triangle_vertices.push_back(_edge_1->get_vertex_0());

	this->update_perimeter();
	this->update_area();

	energy_color = 0.0;
	energy_shape = 0.0;
}

void Triangle::set_edges(std::vector<Edge*> _v_triangle_edges)
{
	this->set_edges(_v_triangle_edges[0], _v_triangle_edges[1], _v_triangle_edges[2]);
}

void Triangle::update_perimeter()
{
	perimeter = v_triangle_edges[0]->get_length() + v_triangle_edges[1]->get_length() + v_triangle_edges[2]->get_length();
}

void Triangle::update_area()
{
	double a, b, c, p;
	
	a = v_triangle_edges[0]->get_length();
	b = v_triangle_edges[1]->get_length();
	c = v_triangle_edges[2]->get_length();

	p = (a + b + c) / 2.0;

	// Heron's formula
	area = sqrt(p*(p - a)*(p - b)*(p - c));
}

void Triangle::display_parameters() const
{
	printf("Triangle %d \t - \t perimeter = %f \t ; \t area = %f\n", ind_triangle, perimeter, area);
	printf("\t\tenergy_shape = %f \t ; \t energy_color = %f \n", energy_shape, energy_color);
	if (color_mean.empty())
		printf("\t\tcolor_mean : empty\n");
	else
		for (int i = 0; i < color_mean.size(); i++)
			printf("\t\tcolor_mean[%d] = %f \n", i, color_mean[i]);
	printf("\t\t");
	v_triangle_vertices[0]->display_parameters();
	printf("\t\t");
	v_triangle_vertices[1]->display_parameters();
	printf("\t\t");
	v_triangle_vertices[2]->display_parameters();
}
