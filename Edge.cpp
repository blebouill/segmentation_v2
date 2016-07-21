#include <iostream>
#include <cmath>

#include "Edge.h"



Edge::Edge()
{
	length = 0.0;
	pixels_sum_left = 0;
	pixels_sum_right = 0;
}


Edge::Edge(Vertex * _vertex_0, Vertex * _vertex_1, int _ind_edge)
{
	this->set_vertices(_vertex_0, _vertex_1);
	ind_edge = _ind_edge;
	pixels_sum_left = 0;
	pixels_sum_right = 0;
}


Edge::~Edge()
{
}


double Edge::get_length() const
{
	return length;
}


Vertex * Edge::get_vertex_0() const
{
	return v_edge_vertices[0];
}


Vertex * Edge::get_vertex_1() const
{
	return v_edge_vertices[1];
}


std::vector<Vertex*> Edge::get_vertices() const
{
	return v_edge_vertices;
}


std::vector<unsigned long int> Edge::get_sum_left() const
{
	return v_sum_left;
}

std::vector<unsigned long int> Edge::get_sum_of_sqares_left() const
{
	return v_sum_of_sqares_left;
}

int Edge::get_pixels_sum_left() const
{
	return pixels_sum_left;
}


std::vector<unsigned long int> Edge::get_sum_right() const
{
	return v_sum_right;
}

std::vector<unsigned long int> Edge::get_sum_of_sqares_right() const
{
	return v_sum_of_sqares_right;
}

int Edge::get_pixels_sum_right() const
{
	return pixels_sum_right;
}


int Edge::get_ind_edge() const
{
	return ind_edge;
}


void Edge::set_vertices(Vertex * _vertex_0, Vertex * _vertex_1)
{
	v_edge_vertices.clear();

	v_edge_vertices.push_back(_vertex_0);
	v_edge_vertices.push_back(_vertex_1);

	this->update_length();
}


void Edge::set_vertices(std::vector<Vertex*> _v_edge_vertices)
{
	v_edge_vertices = _v_edge_vertices;

	this->update_length();
}


void Edge::set_sum_left(std::vector<unsigned long int> _v_sum_left)
{
	v_sum_left = _v_sum_left;
}

void Edge::set_sum_of_sqares_left(std::vector<unsigned long int> _v_sum_of_sqares_left)
{
	v_sum_of_sqares_left = _v_sum_of_sqares_left;
}

void Edge::set_pixels_sum_left(int _pixels_sum_left)
{
	pixels_sum_left = _pixels_sum_left;
}


void Edge::set_sum_right(std::vector<unsigned long int> _v_sum_right)
{
	v_sum_right = _v_sum_right;
}

void Edge::set_sum_of_sqares_right(std::vector<unsigned long int> _v_sum_of_sqares_right)
{
	v_sum_of_sqares_right = _v_sum_of_sqares_right;
}

void Edge::set_pixels_sum_right(int _pixels_sum_right)
{
	pixels_sum_right = _pixels_sum_right;
}


void Edge::set_ind_edge(int _ind_edge)
{
	ind_edge = _ind_edge;
}


void Edge::update_length()
{
	double delta_x = v_edge_vertices[0]->get_x() - v_edge_vertices[1]->get_x();
	double delta_y = v_edge_vertices[0]->get_y() - v_edge_vertices[1]->get_y();

	length = sqrt(delta_x*delta_x + delta_y*delta_y);
}


bool Edge::update_sums(std::vector<unsigned long int> summed_line_table, std::vector<unsigned long int> summed_squared_line_table, int width_img, int height_img, int nb_color)
{
	int k, ind_pixel_left, ind_pixel_right;
	int x1, x2, y1, y2;
	int dx, dy, e;

	// Initialize pixels_sum_X
	pixels_sum_left = 0;
	pixels_sum_right = 0;

	// Initialize v_sum_X
	v_sum_left.assign(nb_color,0);
	v_sum_right.assign(nb_color, 0);
	v_sum_of_sqares_left.assign(nb_color, 0);
	v_sum_of_sqares_right.assign(nb_color, 0);

	// Récupération des sommets pour avoir x1 = x_min (donc dx = x2 - x1 >= 0)
	if (v_edge_vertices[0]->get_x() <= v_edge_vertices[1]->get_x())
	{
		x1 = (int)round(v_edge_vertices[0]->get_x());
		x2 = (int)round(v_edge_vertices[1]->get_x());
		y1 = (int)round(v_edge_vertices[0]->get_y());
		y2 = (int)round(v_edge_vertices[1]->get_y());
	}
	else
	{
		x1 = (int)round(v_edge_vertices[1]->get_x());
		x2 = (int)round(v_edge_vertices[0]->get_x());
		y1 = (int)round(v_edge_vertices[1]->get_y());
		y2 = (int)round(v_edge_vertices[0]->get_y());
	}

	// Cas particulier : ligne horizontale
	if (y1 == y2)
	{
		for (k = 0; k < nb_color; k++)
		{
			// Indice des pixels
			ind_pixel_left = (k*height_img + y1)*width_img + x1 - 1;	// Ne pas oublier le -1 pour le left
			ind_pixel_right = (k*height_img + y2)*width_img + x2;

			// Assignation des valeurs
			v_sum_left[k] = (x1 > 0) ? summed_line_table[ind_pixel_left] : 0;	// Si x1 = 0, pas de pixels à gauche donc x1 - 1 en dehors de l'image
			v_sum_right[k] = summed_line_table[ind_pixel_right];
			v_sum_of_sqares_left[k] = (x1 > 0) ? summed_squared_line_table[ind_pixel_left] : 0;
			v_sum_of_sqares_right[k] = summed_squared_line_table[ind_pixel_right];

			// Nombre de pixels
			pixels_sum_left = x1;
			pixels_sum_right = x2 + 1;
		}
		return true;
	}

	// Cas général - Algorithme de Bresenham dans les octants 1, 2, 7 et 8 car x1 = x_min
	dx = x2 - x1;
	dy = y2 - y1;

	// Initialisation avec les valeurs des extremités
	for (k = 0; k < nb_color; k++)
	{
		ind_pixel_left = (k*height_img + y1)*width_img + x1 - 1;	// Pixel à gauche de (x1,y1)
		ind_pixel_right = (k*height_img + y2)*width_img + x2; // Pixel (x2,y2)

		// Assignation des valeurs
		v_sum_left[k] = (x1 > 0) ? summed_line_table[ind_pixel_left] : 0;
		v_sum_right[k] = summed_line_table[ind_pixel_right];
		v_sum_of_sqares_left[k] = (x1 > 0) ? summed_squared_line_table[ind_pixel_left] : 0;
		v_sum_of_sqares_right[k] = summed_squared_line_table[ind_pixel_right];

		// Nombre de pixels
		pixels_sum_left = x1;
		pixels_sum_right = x2 + 1;
	}

	// dx >= 0 et dy != 0 donc des tests (if) ont été supprimés car inutiles
	if (dx != 0)
	{
		if (dy > 0)	// Vecteur oblique dans le premier cadran
		{
			if (dx >= dy) // Vecteur dans le premier octant
			{
				e = dx;
				dx = 2 * e;
				dy = 2 * dy;
				while (x1 != x2)
				{
					x1++;
					e -= dy;
					if (e < 0)
					{
						y1++;
						e += dx;
						// Mise à jour des sommes
						for (k = 0; k < nb_color; k++)
						{
							// Indice des pixels
							ind_pixel_left = (k*height_img + y1)*width_img + x1 - 1;	// Pixel à gauche de la nouvelle position
							ind_pixel_right = (k*height_img + y1 - 1)*width_img + x1 - 1; // Pixel au dessus et à gauche de la nouvelle position

							// Assignation des valeurs
							v_sum_left[k] += summed_line_table[ind_pixel_left];
							v_sum_right[k] += summed_line_table[ind_pixel_right];
							v_sum_of_sqares_left[k] += summed_squared_line_table[ind_pixel_left];
							v_sum_of_sqares_right[k] += summed_squared_line_table[ind_pixel_right];

							// Nombre de pixels
							pixels_sum_left += x1;
							pixels_sum_right += x1 ;
						}
					}
				}
			}

			else // Vecteur dans le second octant (dx < dy)
			{
				e = dy;
				dx = 2 * dx;
				dy = 2 * e;
				while (y1 != y2)
				{
					y1++;
					e -= dx;

					// Mise à jour des sommes right
					for (k = 0; k < nb_color; k++)
					{
						// Indice des pixels
						ind_pixel_right = (k*height_img + y1 - 1)*width_img + x1; // Pixel au dessus de la nouvelle position sans avoir mis à jour x1

						// Assignation des valeurs
						v_sum_right[k] += summed_line_table[ind_pixel_right];
						v_sum_of_sqares_right[k] += summed_squared_line_table[ind_pixel_right];

						// Nombre de pixels
						pixels_sum_right += x1 + 1;
					}

					if (e < 0)
					{
						x1++;
						e += dy;
					}

					// Mise à jour des sommes left
					for (k = 0; k < nb_color; k++)
					{
						// Indice des pixels
						ind_pixel_left = (k*height_img + y1)*width_img + x1 - 1;	// Pixel à gauche de la nouvelle position

						// Assignation des valeurs
						v_sum_left[k] += summed_line_table[ind_pixel_left];
						v_sum_of_sqares_left[k] += summed_squared_line_table[ind_pixel_left];

						// Nombre de pixels
						pixels_sum_right += x1;
					}
				}
			}
		}

		else // Vecteur oblique dans le quatrieme cadran (dy < 0)
		{
			if (dx >= -dy) // Vecteur dans le huitième octant
			{
				e = dx;
				dx = 2 * e;
				dy = 2 * dy;
				while (x1 != x2)
				{
					x1++;
					e += dy;
					if (e < 0)
					{
						y1--;
						e += dx;
						// Mise à jour des sommes
						for (k = 0; k < nb_color; k++)
						{
							// Indice des pixels
							ind_pixel_left = (k*height_img + y1)*width_img + x1 - 1;	// Pixel à gauche de la nouvelle position
							ind_pixel_right = (k*height_img + y1 + 1)*width_img + x1 - 1; // Pixel en dessous et à gauche de la nouvelle position

							// Assignation des valeurs
							v_sum_left[k] += summed_line_table[ind_pixel_left];
							v_sum_right[k] += summed_line_table[ind_pixel_right];
							v_sum_of_sqares_left[k] += summed_squared_line_table[ind_pixel_left];
							v_sum_of_sqares_right[k] += summed_squared_line_table[ind_pixel_right];

							// Nombre de pixels
							pixels_sum_left += x1;
							pixels_sum_right += x1;
						}
					}
				}
			}

			else // Vecteur dans le septième octant (dx < -dy)
			{
				e = dy;
				dx = 2 * dx;
				dy = 2 * e;
				while (y1 != y2)
				{
					y1--;
					e += dx;

					// Mise à jour des sommes right
					for (k = 0; k < nb_color; k++)
					{
						// Indice des pixels
						ind_pixel_right = (k*height_img + y1 + 1)*width_img + x1; // Pixel en dessous de la nouvelle position sans avoir mis à jour x1

						 // Assignation des valeurs
						v_sum_right[k] += summed_line_table[ind_pixel_right];
						v_sum_of_sqares_right[k] += summed_squared_line_table[ind_pixel_right];

						// Nombre de pixels
						pixels_sum_right += x1 + 1;
					}

					if (e > 0)
					{
						x1++;
						e += dy;
					}

					// Mise à jour des sommes left
					for (k = 0; k < nb_color; k++)
					{
						// Indice des pixels
						ind_pixel_left = (k*height_img + y1)*width_img + x1 - 1;	// Pixel à gauche de la nouvelle position

						// Assignation des valeurs
						v_sum_left[k] += summed_line_table[ind_pixel_left];
						v_sum_of_sqares_left[k] += summed_squared_line_table[ind_pixel_left];

						// Nombre de pixels
						pixels_sum_right += x1;
					}
				}
			}
		}
	}

	else // Ligne verticale (dx = 0)
	{
		if (dy > 0)
		{
			while (y1 != y2)
			{
				y1++;
				// Mise à jour des sommes
				for (k = 0; k < nb_color; k++)
				{
					// Indice des pixels
					ind_pixel_left = (k*height_img + y1)*width_img + x1 - 1;	// Pixel à gauche de la nouvelle position
					ind_pixel_right = (k*height_img + y1 - 1)*width_img + x1; // Pixel au dessus de la nouvelle position

					 // Assignation des valeurs
					v_sum_left[k] += (x1 > 0) ? summed_line_table[ind_pixel_left] : 0;
					v_sum_right[k] += summed_line_table[ind_pixel_right];
					v_sum_of_sqares_left[k] += (x1 > 0) ? summed_squared_line_table[ind_pixel_left] : 0;
					v_sum_of_sqares_right[k] += summed_squared_line_table[ind_pixel_right];

					// Nombre de pixels
					pixels_sum_left += x1;
					pixels_sum_right += x1 + 1;
				}
			}
		}

		else // (dy < 0)
		{
			while (y1 != y2)
			{
				y1--;
				// Mise à jour des sommes
				for (k = 0; k < nb_color; k++)
				{
					// Indice des pixels
					ind_pixel_left = (k*height_img + y1)*width_img + x1 - 1;	// Pixel à gauche de la nouvelle position
					ind_pixel_right = (k*height_img + y1 + 1)*width_img + x1; // Pixel en dessous de la nouvelle position

					// Assignation des valeurs
					v_sum_left[k] += (x1 > 0) ? summed_line_table[ind_pixel_left] : 0;
					v_sum_right[k] += summed_line_table[ind_pixel_right];
					v_sum_of_sqares_left[k] += (x1 > 0) ? summed_squared_line_table[ind_pixel_left] : 0;
					v_sum_of_sqares_right[k] += summed_squared_line_table[ind_pixel_right];

					// Nombre de pixels
					pixels_sum_left += x1;
					pixels_sum_right += x1 + 1;
				}
			}
		}
	}
	
	return true;
}


void Edge::display_parameters() const
{
	printf("Edge %d \t - \t length = %f \n", ind_edge, length);

	printf("\tv_sum_left : ");
	if (v_sum_left.empty())
		printf("empty \n");
	else
	{
		for (int i = 0; i < v_sum_left.size(); i++)
			printf("%lu \t ", v_sum_left[i]);
		printf("\n");
	}

	printf("\tv_sum_right : ");
	if (v_sum_right.empty())
		printf("empty \n");
	else
	{
		for (int i = 0; i < v_sum_right.size(); i++)
			printf("%lu \t ", v_sum_right[i]);
		printf("\n");
	}

	printf("\tv_sum_of_sqares_left : ");
	if (v_sum_of_sqares_left.empty())
		printf("empty \n");
	else
	{
		for (int i = 0; i < v_sum_of_sqares_left.size(); i++)
			printf("%lu \t ", v_sum_of_sqares_left[i]);
		printf("\n");
	}

	printf("\tv_sum_of_sqares_right : ");
	if (v_sum_of_sqares_right.empty())
		printf("empty \n");
	else
	{
		for (int i = 0; i < v_sum_of_sqares_right.size(); i++)
			printf("%lu \t ", v_sum_of_sqares_right[i]);
		printf("\n");
	}

	printf("\tpixels_sum_left : %d\n", pixels_sum_left);
	printf("\tpixels_sum_right : %d\n", pixels_sum_right);

	printf("\t");
	v_edge_vertices[0]->display_parameters();
	printf("\t");
	v_edge_vertices[1]->display_parameters();
}
