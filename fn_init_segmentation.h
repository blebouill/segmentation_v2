#pragma once

#include <vector>
#include "Triangle.h"
#include "Edge.h"
#include "Vertex.h"

void init_triangular_mesh(std::vector<Vertex> * v_vertex, std::vector<Edge> * v_edge, std::vector<Triangle> * v_triangle,
						  std::vector<std::vector<int> > * v_ind_edges_using_vertex, std::vector<std::vector<int> > * v_ind_triangles_using_vertex,
						  std::vector<std::vector<int> > * v_ind_triangles_using_edge, int width_img, int height_img, int N);

int find_first_equal_element(std::vector<int> v1, std::vector<int> v2);

void compute_summed_line_tables(int * img, int width_img, int height_img, int nb_color, std::vector<unsigned long int> * summed_line_table, std::vector<unsigned long int> * summed_squared_line_table);