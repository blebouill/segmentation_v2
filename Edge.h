#pragma once

#include <vector>
#include "Vertex.h"

class Edge
{
public:
	Edge();
	Edge(Vertex * _vertex_0, Vertex * _vertex_1, int _ind_edge = -1);
	~Edge();
	double get_length() const;
	Vertex * get_vertex_0() const;
	Vertex * get_vertex_1() const;
	std::vector<Vertex*> get_vertices() const;
	std::vector<unsigned long int> get_sum_left() const;
	std::vector<unsigned long int> get_sum_of_sqares_left() const;
	int get_pixels_sum_left() const;
	std::vector<unsigned long int> get_sum_right() const;
	std::vector<unsigned long int> get_sum_of_sqares_right() const;
	int get_pixels_sum_right() const;
	int get_ind_edge() const;
	void set_vertices(Vertex * _vertex_0, Vertex * _vertex_1);
	void set_vertices(std::vector<Vertex*> _v_edge_vertices);
	void set_sum_left(std::vector<unsigned long int> _v_sum_left);
	void set_sum_of_sqares_left(std::vector<unsigned long int> _v_sum_of_sqares_left);
	void set_pixels_sum_left(int _pixels_sum_left);
	void set_sum_right(std::vector<unsigned long int> _v_sum_right);
	void set_sum_of_sqares_right(std::vector<unsigned long int> _v_sum_of_sqares_right);
	void set_pixels_sum_right(int _pixels_sum_right);
	void set_ind_edge(int _ind_edge);
	void update_length();
	bool update_sums(std::vector<unsigned long int> summed_line_table, std::vector<unsigned long int> summed_squared_line_table, int width_img, int height_img, int nb_color);
	void display_parameters() const;
private:
	std::vector<Vertex*> v_edge_vertices;
	double length;
	std::vector<unsigned long int> v_sum_right;	// Somme des images intégrales par ligne sur le côté du triangle (un par couleur)
	std::vector<unsigned long int> v_sum_of_sqares_right;
	std::vector<unsigned long int> v_sum_left;	// Somme des images intégrales par ligne sur les voisins extérieurs (gauche) du côté du triangle (un par couleur)
	std::vector<unsigned long int> v_sum_of_sqares_left;
	int pixels_sum_left;
	int pixels_sum_right;
	int ind_edge;
};

