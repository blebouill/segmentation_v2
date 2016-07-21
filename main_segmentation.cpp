#include <iostream>
#include <cmath>
#include <vector>

#include "Vertex.cpp"
#include "Edge.cpp"
#include "Triangle.cpp"
#include "fn_init_segmentation.cpp"

using namespace std;

int main_segmentation_triangles(int * img, int height_img, int width_img, int nb_color, int N)
{

	// ________________________ Variables & Calculs préliminaires ________________________ //

	int i, j, k, ind;
	double perimeter_ref, area_ref;

	vector<unsigned long int> summed_line_table, summed_squared_line_table;

	vector<Vertex> v_vertex;
	vector<Edge> v_edge;
	vector<Triangle> v_triangle;

	vector<vector<int> > v_ind_edges_using_vertex, v_ind_triangles_using_vertex, v_ind_triangles_using_edge;


	// Images intégrales par ligne
	compute_summed_line_tables(img, width_img, height_img, nb_color, &summed_line_table, &summed_squared_line_table);


	// _________________________________ Initialisation __________________________________ //
	
	//	Initialisation du maillage
	init_triangular_mesh(&v_vertex, &v_edge, &v_triangle, &v_ind_edges_using_vertex, &v_ind_triangles_using_vertex, &v_ind_triangles_using_edge, width_img, height_img, N);
	
	N = (int)v_triangle.size();

	// Aire de référence
	area_ref = ((double)width_img - 1.0)*((double)height_img - 1.0) / (double)N;

	// Périmetre de référence
	perimeter_ref = 3.0*sqrt(4.0 / (sqrt(3.0) * (double)N) * ((double)width_img - 1.0) * ((double)height_img - 1.0));
	
	// Calcul des sommes des arêtes
	for (i = 0; i < v_edge.size(); i++)
		v_edge[i].update_sums(summed_line_table, summed_squared_line_table, width_img, height_img, nb_color);
	
	// for (i = 0; i < v_edge.size(); i++)
	// 	v_edge[i].display_parameters();
	



	return 1;
}