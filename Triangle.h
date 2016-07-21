#pragma once

#include <vector>
#include "Edge.h"
#include "Vertex.h"

class Triangle
{
public:
	Triangle();
	~Triangle();
	int get_ind_triangle() const;
	int get_nb_pixels() const;
	double get_perimeter() const;
	double get_area() const;
	double get_energy_color() const;
	double get_energy_shape() const;
	std::vector<double> get_color_mean() const;
	Vertex * get_vertex_0() const;
	Vertex * get_vertex_1() const;
	Vertex * get_vertex_2() const;
	std::vector<Vertex*> get_triangle_vertices() const;
	Edge * get_edge_0() const;
	Edge * get_edge_1() const;
	Edge * get_edge_2() const;
	std::vector<Edge*> get_triangle_edges() const;
	void set_ind_triangle(int _ind_triangle);
	void set_nb_pixels(int _nb_pixels);
	void set_energy_color(double _energy_color);
	void set_energy_shape(double _energy_shape);
	void set_color_mean(std::vector<double> _color_mean);
	void set_edges(Edge * _edge_0, Edge * _edge_1, Edge * _edge_2);
	void set_edges(std::vector<Edge*> _v_triangle_edges);
	void update_perimeter();
	void update_area();
	void display_parameters() const;
private:
	int ind_triangle;
	int nb_pixels;
	double perimeter;
	double area;
	double energy_color;
	double energy_shape;
	std::vector<double> color_mean;
	std::vector<Vertex*> v_triangle_vertices;
	std::vector<Edge*> v_triangle_edges;
};

