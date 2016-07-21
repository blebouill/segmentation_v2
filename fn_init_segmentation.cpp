#include <iostream>
#include <cmath>

#include "fn_init_segmentation.h"

using namespace std;


/*******************************************************************
						init_triangular_mesh
********************************************************************/

void init_triangular_mesh(vector<Vertex> * v_vertex, vector<Edge> * v_edge, vector<Triangle> * v_triangle,
						  vector<vector<int> > * v_ind_edges_using_vertex, vector<vector<int> > * v_ind_triangles_using_vertex,
						  vector<vector<int> > * v_ind_triangles_using_edge, int width_img, int height_img, int N)
{
	/* _____________________ Variables _____________________ */

	int i, j, k;
	int nb_vertices, nb_vertices_odd_line, nb_vertices_even_line, nb_odd_line, nb_even_line;
	int ind, nb_line, nb_edges, nb_bloc, nb_edges_odd_line, nb_edges_even_line, nb_triangles;
	int ind_vertex_1, ind_vertex_2, ind_vertex_3, ind_edge, nb_vertices_iter;
	double a, a_x, a_y, h, nb_edge_x, delta_x;

	Vertex * v1 = NULL, * v2 = NULL;
	Edge * e1 = NULL, * e2 = NULL, * e3 = NULL;

	vector<int> v_ind_vertex_1(4), v_ind_vertex_2(4);


	/* _____________________ Calculs préliminaires _____________________ */

	// Longueur du côté des triangles initiaux sur l'axe x (quasi-equilateraux)
	a = sqrt(4.0 / (sqrt(3.0) * (double)N) * ((double)width_img - 1.0) * ((double)height_img - 1.0));

	// Hauteur des triangles équilatéraux de côté a_x
	h = sqrt(3.0) * a / 2.0;

	// Ajustement de la valeur de h pour avoir un nombre de ligne impair
	nb_line = (int)round(((double)height_img - 1.0) / h) + 1;

	if ((nb_line % 2) == 0)
	{
		nb_line++;
		h = ((double)height_img - 1.0) / ((double)nb_line - 1.0);
		a = 2.0 * h / sqrt(3.0);
	}

	// Ajustement de la valeur a_x pour avoir un maillage symétrique sur l'axe x
	nb_edge_x = ceil(((double)width_img - 1.0) / a);
	a_x = ((double)width_img - 1.0) / nb_edge_x;

	// Ajustement de la valeur a_y pour avoir un maillage régulier sur l'axe y
	h = ((double)height_img - 1.0) / ((double)nb_line - 1.0);
	a_y = h;

	// Nombre de sommets par ligne paire ou impaire
	nb_vertices_odd_line = (int)nb_edge_x + 1;
	nb_vertices_even_line = (int)nb_edge_x;

	// Nombre de lignes paires et impaires (début à 1 donc impair)
	nb_odd_line = (int)round(nb_line / 2.0);	// Lignes impaires
	nb_even_line = nb_line - nb_odd_line;		// Lignes paires

	// Nombre de côtés
	nb_edges_odd_line = nb_vertices_odd_line - 1;
	nb_edges_even_line = nb_vertices_even_line - 1;

	// Nombre de blocs
	nb_bloc = nb_odd_line - 1;


	/* _____________________ Vertices _____________________ */

	// Nombre de sommets
	nb_vertices = nb_odd_line*nb_vertices_odd_line + nb_even_line*nb_vertices_even_line;

	// Resize v_vertex
	v_vertex->resize(nb_vertices);


	/* _____________________ Edges _____________________ */

	// Nombre de côtés
	nb_edges = nb_edges_odd_line + nb_bloc*(5 * nb_vertices_even_line + nb_edges_even_line + 2);

	// Resize v_edge
	v_edge->resize(nb_edges);


	/* _____________________ Triangles _____________________ */

	// Nombre de triangles
	nb_triangles = nb_bloc*(2 * nb_edges_even_line + 2 * nb_edges_odd_line + 2);

	// Resize v_triangle
	v_triangle->resize(nb_triangles);


	/* _____________________ Remplissage des classes Vertices _____________________ */

	ind = 0;
	delta_x = a_x / 2.0;

	for (i = 1; i < nb_line; i++)
	{
		// Ligne paire
		if ((i % 2) == 0)
			for (j = 0; j < nb_vertices_even_line; j++)
			{
				(*v_vertex)[ind].set_vertex((double)j*a_x + delta_x, (double)(i - 1)*a_y, ind);
				ind++;
			}

		// Ligne impaire
		else
		{
			for (j = 0; j < nb_vertices_odd_line - 1; j++)
			{
				(*v_vertex)[ind].set_vertex((double)j*a_x, (double)(i - 1)*a_y, ind);
				ind++;
			}
			(*v_vertex)[ind].set_vertex((double)width_img - 1.0, (double)(i - 1)*a_y, ind);
			ind++;
		}
	}

	// Dernière ligne (impaire)
	for (j = 0; j < nb_vertices_odd_line - 1; j++)
	{
		(*v_vertex)[ind].set_vertex((double)j*a_x, (double)height_img - 1.0, ind);
		ind++;
	}
	(*v_vertex)[ind].set_vertex((double)width_img - 1.0, (double)height_img - 1.0, ind);

	/* _____________________ Remplissage des classes Edges _____________________ */

	ind = 0;
	
	// Resize v_ind_edges_using_vertex
	v_ind_edges_using_vertex->resize(nb_vertices);
	
	// Côtés horizontaux
	ind_vertex_1 = 0;

	for (i = 1; i <= nb_line; i++)
	{
		nb_vertices_iter = ((i % 2) == 0) ? nb_vertices_even_line : nb_vertices_odd_line;

		for (j = 0; j < nb_vertices_iter - 1; j++)
		{
			ind_vertex_2 = ind_vertex_1 + 1;

			v1 = &(*v_vertex)[ind_vertex_1];
			v2 = &(*v_vertex)[ind_vertex_2];

			(*v_edge)[ind].set_vertices(v1, v2);
			(*v_edge)[ind].set_ind_edge(ind);

			(*v_ind_edges_using_vertex)[ind_vertex_1].push_back(ind);
			(*v_ind_edges_using_vertex)[ind_vertex_2].push_back(ind);

			ind++;
			ind_vertex_1++;
		}
		ind_vertex_1++;
	}

	// Côtés verticaux [Bord gauche]
	ind_vertex_1 = 0;

	for (i = 0; i < nb_bloc; i++)
	{
		ind_vertex_2 = ind_vertex_1 + nb_vertices_even_line + nb_vertices_odd_line;

		v1 = &(*v_vertex)[ind_vertex_1];
		v2 = &(*v_vertex)[ind_vertex_2];

		(*v_edge)[ind].set_vertices(v1, v2);
		(*v_edge)[ind].set_ind_edge(ind);

		(*v_ind_edges_using_vertex)[ind_vertex_1].push_back(ind);
		(*v_ind_edges_using_vertex)[ind_vertex_2].push_back(ind);

		ind++;
		ind_vertex_1 += nb_vertices_even_line + nb_vertices_odd_line;
	}

	// Côtés verticaux [Bord droit]
	ind_vertex_1 = nb_vertices_odd_line - 1;

	for (i = 0; i < nb_bloc; i++)
	{
		ind_vertex_2 = ind_vertex_1 + nb_vertices_even_line + nb_vertices_odd_line;

		v1 = &(*v_vertex)[ind_vertex_1];
		v2 = &(*v_vertex)[ind_vertex_2];

		(*v_edge)[ind].set_vertices(v1, v2);
		(*v_edge)[ind].set_ind_edge(ind);

		(*v_ind_edges_using_vertex)[ind_vertex_1].push_back(ind);
		(*v_ind_edges_using_vertex)[ind_vertex_2].push_back(ind);

		ind++;
		ind_vertex_1 += nb_vertices_even_line + nb_vertices_odd_line;
	}

	// Côtés restants
	for (i = 0; i < nb_bloc; i++)
	{
		for (j = 0; j < nb_vertices_even_line; j++)
		{
			ind_vertex_1 = nb_vertices_odd_line + j + i*(nb_vertices_odd_line + nb_vertices_even_line);

			v_ind_vertex_2[0] = ind_vertex_1 - nb_vertices_odd_line;		// Haut gauche
			v_ind_vertex_2[1] = ind_vertex_1 - nb_vertices_odd_line + 1;	// Haut droit
			v_ind_vertex_2[2] = ind_vertex_1 + nb_vertices_even_line;		// Bas gauche
			v_ind_vertex_2[3] = ind_vertex_1 + nb_vertices_even_line + 1;	// Bas droit

			for (k = 0; k < 4; k++)
			{
				v1 = &(*v_vertex)[ind_vertex_1];
				v2 = &(*v_vertex)[v_ind_vertex_2[k]];

				(*v_edge)[ind].set_vertices(v1, v2);
				(*v_edge)[ind].set_ind_edge(ind);

				(*v_ind_edges_using_vertex)[ind_vertex_1].push_back(ind);
				(*v_ind_edges_using_vertex)[v_ind_vertex_2[k]].push_back(ind);

				ind++;
			}
		}
	}

	/*
	printf("\nv_ind_edges_using_vertex\n");
	for (int i = 0; i < v_ind_edges_using_vertex->size(); i++)
	{
	for (int j = 0; j < (*v_ind_edges_using_vertex)[i].size(); j++)
	{
	printf("%d\t", (*v_ind_edges_using_vertex)[i][j]);
	}
	printf("\n");
	}
	printf("\n");
	*/
	/*
	for (int i = 0; i < nb_edges; i++)
	(*v_edge)[i].display_edge();
	*/


	/* _____________________ Remplissage des classes Triangles _____________________ */

	// Resize v_ind_triangles_using_vertex
	v_ind_triangles_using_vertex->resize(nb_vertices);

	// Resize v_ind_triangles_using_edge
	v_ind_triangles_using_edge->resize(nb_edges);

	ind = 0;

	for (i = 0; i < nb_bloc; i++)
	{
		for (j = 0; j < nb_vertices_odd_line - 1; j++)
		{
			// Triangle en dessous de la première ligne du bloc
			ind_vertex_1 = i*(nb_vertices_even_line + nb_vertices_odd_line) + j;
			ind_vertex_2 = ind_vertex_1 + 1;
			ind_vertex_3 = ind_vertex_1 + nb_vertices_odd_line;

			(*v_ind_triangles_using_vertex)[ind_vertex_1].push_back(ind);
			(*v_ind_triangles_using_vertex)[ind_vertex_2].push_back(ind);
			(*v_ind_triangles_using_vertex)[ind_vertex_3].push_back(ind);

			// Edge 1 (vertex_1 -- vertex_2)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_2]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e1 = &(*v_edge)[ind_edge];

			// Edge 2 (vertex_2 -- vertex_3)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_2], (*v_ind_edges_using_vertex)[ind_vertex_3]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e2 = &(*v_edge)[ind_edge];

			// Edge 3 (vertex_1 -- vertex_3)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_3]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e3 = &(*v_edge)[ind_edge];

			// Remplissage de v_triangle
			(*v_triangle)[ind].set_edges(e1, e2, e3);
			(*v_triangle)[ind].set_ind_triangle(ind);

			ind++;
		}

		for (j = 0; j < nb_vertices_even_line - 1; j++)
		{
			// Triangle au dessus de la ligne du milieu du bloc
			ind_vertex_1 = nb_vertices_odd_line + i*(nb_vertices_even_line + nb_vertices_odd_line) + j;
			ind_vertex_2 = ind_vertex_1 + 1;
			ind_vertex_3 = ind_vertex_1 - nb_vertices_odd_line + 1;

			(*v_ind_triangles_using_vertex)[ind_vertex_1].push_back(ind);
			(*v_ind_triangles_using_vertex)[ind_vertex_2].push_back(ind);
			(*v_ind_triangles_using_vertex)[ind_vertex_3].push_back(ind);

			// Edge 1 (vertex_1 -- vertex_2)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_2]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e1 = &(*v_edge)[ind_edge];

			// Edge 2 (vertex_2 -- vertex_3)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_2], (*v_ind_edges_using_vertex)[ind_vertex_3]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e2 = &(*v_edge)[ind_edge];

			// Edge 3 (vertex_1 -- vertex_3)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_3]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e3 = &(*v_edge)[ind_edge];

			// Remplissage de v_triangle
			(*v_triangle)[ind].set_edges(e1, e2, e3);
			(*v_triangle)[ind].set_ind_triangle(ind);

			ind++;



			// Triangle en dessous de la ligne du milieu du bloc
			ind_vertex_1 = nb_vertices_odd_line + i*(nb_vertices_even_line + nb_vertices_odd_line) + j;
			ind_vertex_2 = ind_vertex_1 + 1;
			ind_vertex_3 = ind_vertex_1 + nb_vertices_even_line + 1;

			(*v_ind_triangles_using_vertex)[ind_vertex_1].push_back(ind);
			(*v_ind_triangles_using_vertex)[ind_vertex_2].push_back(ind);
			(*v_ind_triangles_using_vertex)[ind_vertex_3].push_back(ind);

			// Edge 1 (vertex_1 -- vertex_2)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_2]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e1 = &(*v_edge)[ind_edge];

			// Edge 2 (vertex_2 -- vertex_3)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_2], (*v_ind_edges_using_vertex)[ind_vertex_3]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e2 = &(*v_edge)[ind_edge];

			// Edge 3 (vertex_1 -- vertex_3)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_3]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e3 = &(*v_edge)[ind_edge];

			// Remplissage de v_triangle
			(*v_triangle)[ind].set_edges(e1, e2, e3);
			(*v_triangle)[ind].set_ind_triangle(ind);

			ind++;
		}

		for (j = 0; j < nb_vertices_odd_line - 1; j++)
		{
			// Triangle au dessus de la dernière ligne du bloc
			ind_vertex_1 = (i + 1)*(nb_vertices_even_line + nb_vertices_odd_line) + j;
			ind_vertex_2 = ind_vertex_1 + 1;
			ind_vertex_3 = ind_vertex_1 - nb_vertices_even_line;

			(*v_ind_triangles_using_vertex)[ind_vertex_1].push_back(ind);
			(*v_ind_triangles_using_vertex)[ind_vertex_2].push_back(ind);
			(*v_ind_triangles_using_vertex)[ind_vertex_3].push_back(ind);

			// Edge 1 (vertex_1 -- vertex_2)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_2]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e1 = &(*v_edge)[ind_edge];

			// Edge 2 (vertex_2 -- vertex_3)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_2], (*v_ind_edges_using_vertex)[ind_vertex_3]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e2 = &(*v_edge)[ind_edge];

			// Edge 3 (vertex_1 -- vertex_3)
			ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_3]);

			(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

			e3 = &(*v_edge)[ind_edge];

			// Remplissage de v_triangle
			(*v_triangle)[ind].set_edges(e1, e2, e3);
			(*v_triangle)[ind].set_ind_triangle(ind);

			ind++;
		}

		// Triangle sur le bord gauche
		ind_vertex_1 = i*(nb_vertices_even_line + nb_vertices_odd_line);
		ind_vertex_2 = ind_vertex_1 + nb_vertices_odd_line;
		ind_vertex_3 = ind_vertex_2 + nb_vertices_even_line;

		(*v_ind_triangles_using_vertex)[ind_vertex_1].push_back(ind);
		(*v_ind_triangles_using_vertex)[ind_vertex_2].push_back(ind);
		(*v_ind_triangles_using_vertex)[ind_vertex_3].push_back(ind);

		// Edge 1 (vertex_1 -- vertex_2)
		ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_2]);

		(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

		e1 = &(*v_edge)[ind_edge];

		// Edge 2 (vertex_2 -- vertex_3)
		ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_2], (*v_ind_edges_using_vertex)[ind_vertex_3]);

		(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

		e2 = &(*v_edge)[ind_edge];

		// Edge 3 (vertex_1 -- vertex_3)
		ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_3]);

		(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

		e3 = &(*v_edge)[ind_edge];

		// Remplissage de v_triangle
		(*v_triangle)[ind].set_edges(e1, e2, e3);
		(*v_triangle)[ind].set_ind_triangle(ind);

		ind++;


		// Triangle sur le bord droit
		ind_vertex_1 = i*(nb_vertices_even_line + nb_vertices_odd_line) + nb_vertices_odd_line - 1;
		ind_vertex_2 = ind_vertex_1 + nb_vertices_even_line;
		ind_vertex_3 = ind_vertex_2 + nb_vertices_odd_line;

		(*v_ind_triangles_using_vertex)[ind_vertex_1].push_back(ind);
		(*v_ind_triangles_using_vertex)[ind_vertex_2].push_back(ind);
		(*v_ind_triangles_using_vertex)[ind_vertex_3].push_back(ind);

		// Edge 1 (vertex_1 -- vertex_2)
		ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_2]);

		(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

		e1 = &(*v_edge)[ind_edge];

		// Edge 2 (vertex_2 -- vertex_3)
		ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_2], (*v_ind_edges_using_vertex)[ind_vertex_3]);

		(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

		e2 = &(*v_edge)[ind_edge];

		// Edge 3 (vertex_1 -- vertex_3)
		ind_edge = find_first_equal_element((*v_ind_edges_using_vertex)[ind_vertex_1], (*v_ind_edges_using_vertex)[ind_vertex_3]);

		(*v_ind_triangles_using_edge)[ind_edge].push_back(ind);

		e3 = &(*v_edge)[ind_edge];

		// Remplissage de v_triangle
		(*v_triangle)[ind].set_edges(e1, e2, e3);
		(*v_triangle)[ind].set_ind_triangle(ind);

		ind++;
	}

	/*
	printf("\nv_ind_triangles_using_vertex\n");
	for (int i = 0; i < v_ind_triangles_using_vertex->size(); i++)
	{
	for (int j = 0; j < (*v_ind_triangles_using_vertex)[i].size(); j++)
	{
	printf("%d\t", (*v_ind_triangles_using_vertex)[i][j]);
	}
	printf("\n");
	}
	printf("\n");


	printf("\nv_ind_triangles_using_edge\n");
	for (int i = 0; i < v_ind_triangles_using_edge->size(); i++)
	{
	for (int j = 0; j < (*v_ind_triangles_using_edge)[i].size(); j++)
	{
	printf("%d\t", (*v_ind_triangles_using_edge)[i][j]);
	}
	printf("\n");
	}
	printf("\n");
	*/
	/*
	for (int i = 0; i < ind; i++)
	(*v_triangle)[i].display_triangle();
	*/
	/*
	printf("a : %f\n", a);
	printf("a_x : %f\n", a_x);
	printf("a_y : %f\n", a_y);
	printf("nb_line : %d\n", nb_line);
	printf("nb_odd_line : %d\n", nb_odd_line);
	printf("nb_even_line : %d\n", nb_even_line);
	printf("nb_vertices_odd_line : %d\n", nb_vertices_odd_line);
	printf("nb_vertices_even_line : %d\n", nb_vertices_even_line);
	printf("Nombre de sommets : %d\n", nb_vertices);
	printf("Nombre de côtés : %d\n", nb_edges);
	printf("Nombre de triangles : %d\n", nb_triangles);
	printf("Nombre de blocs : %d\n", nb_bloc);
	*/
}



/*******************************************************************
					  find_first_equal_element
********************************************************************/

int find_first_equal_element(vector<int> v1, vector<int> v2)
{
	int ind_1 = 0, ind_2 = 0, equal_element = -1;

	do
	{
		do
		{
			if (v1[ind_1] == v2[ind_2])
				equal_element = v1[ind_1];
			ind_2++;
		} while ((equal_element == -1) && (ind_2 <v2.size()));
		ind_1++;
		ind_2 = 0;
	} while ((equal_element == -1) && (ind_1 <v1.size()));

	return equal_element;
}



/*******************************************************************
				     compute_summed_line_tables
********************************************************************/

void compute_summed_line_tables(int * img, int width_img, int height_img, int nb_color, std::vector<unsigned long int>* summed_line_table, std::vector<unsigned long int>* summed_squared_line_table)
{
	int i, j, k, ind;

	// Resize and initialize summed_line_table
	summed_line_table->assign(height_img*width_img*nb_color, 0);

	// Resize and initialize summed_squared_line_table
	summed_squared_line_table->assign(height_img*width_img*nb_color, 0);

	ind = 0;

	for (k = 0; k < nb_color; k++)
		for (j = 0; j < height_img; j++)
		{
			(*summed_line_table)[ind] = (unsigned long int)img[ind];
			(*summed_squared_line_table)[ind] = (unsigned long int)img[ind] * (unsigned long int)img[ind];
			ind++;
			for (i = 1; i < width_img; i++)
			{
				(*summed_line_table)[ind] = (*summed_line_table)[ind - 1] + (unsigned long int)img[ind];
				(*summed_squared_line_table)[ind] = (*summed_squared_line_table)[ind - 1] + (unsigned long int)img[ind] * (unsigned long int)img[ind];
				ind++;
			}
		}
}
