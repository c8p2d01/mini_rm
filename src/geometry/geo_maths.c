/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   geo_maths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cdahlhof <cdahlhof@student.42wolfsburg.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 17:43:54 by cdahlhof          #+#    #+#             */
/*   Updated: 2022/06/15 21:53:43 by cdahlhof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minirm.h"

/**
 * @brief return a vector orthogonal to both the given vectors
 * @param a [t_vec]
 * @param b [t_vec]
 * @return [t_vec]
*/
t_vec	cross(t_vec a, t_vec b)
{
	t_vec	res;

	res.x = (a.y * b.z) - (a.z * b.y);
	res.y = (a.z * b.x) - (a.x * b.z);
	res.z = (a.x * b.y) - (a.y * b.x);
	unit(&res);
	return (res);
}

t_vec	init_vec(double x, double y, double z)
{
	t_vec	res;

	res.x = x;
	res.y = y;
	res.z = z;
	return (res);
}

t_vec	*vec_alloc(t_vec clone)
{
	t_vec	*res;

	res = malloc(sizeof(t_vec));
	res->x = clone.x;
	res->y = clone.y;
	res->z = clone.z;
	return (res);
}

/**
 * @brief return the length of a vector
 * @param a [t_vec]
 * @return [t_vec]
*/
double	veclen(t_vec a)
{
	if (a.x || a.y || a.z)
		return (pow(pow(a.x, 2) + pow(a.y, 2) + pow(a.z, 2), 0.5));
	return (0);
}

/**
 * @brief return a vector connecting two points (! order counts)
 * @param a [t_vec] starting point
 * @param b [t_vec] ending point
 * @return [t_vec]
*/
t_vec	connect(t_vec a, t_vec b)
{
	t_vec	res;

	res.x = b.x - a.x;
	res.y = b.y - a.y;
	res.z = b.z - a.z;
	return (res);
}

/**
 * @brief add a two vectors together
 * @param a [t_vec] the vector being added to
 * @param b [t_vec] the vector to add
*/
void	addto(t_vec *a, t_vec b)
{
	a->x += b.x;
	a->y += b.y;
	a->z += b.z;
}

/**
 * @brief add a two vectors together
 * @param a [t_vec] the vector being added to
 * @param b [t_vec] the vector to add
*/
t_vec	v_sum(t_vec a, t_vec b)
{
	t_vec	res;

	res.x = a.x + b.x;
	res.y = a.y + b.y;
	res.z = a.z + b.z;
	return (res);
}

/**
 * @brief multiply a vector by a constant
 * @param a [t_vec] the vector mutiplied
 * @param m [double] the multiplicant
*/
void	product(t_vec *a, double m)
{
	if (!a)
		return ;
	a->x *= m;
	a->y *= m;
	a->z *= m;
}

/**
 * @brief multiply a vector by a constant, and return a dublicate
 * @param a [t_vec] the vector mutiplied
 * @param m [double] the multiplicant
 */
t_vec	v_product(t_vec a, double m)
{
	t_vec	res;

	res.x = a.x * m;
	res.y = a.y * m;
	res.z = a.z * m;
	return (res);
}

/**
 * @brief add a set distance to a vector
 * @param a [t_vec] the vector
 * @param m [double] to be added length
*/
void	resize(t_vec *a, double m)
{
	t_vec	o;

	if (!a)
		return ;
	o.x = a->x;
	o.y = a->y;
	o.z = a->z;
	unit(&o);
	m -= 1;
	product(&o, m);
	addto(a, o);
}

/**
 * @brief change a vecto to have the total lenth of 1, no change direction
 * @param a [t_vec] vector to operade on
 * @return [void]
*/
void	unit(t_vec	*a)
{
	double	d;

	if (!a)
		return ;
	d = 1 / veclen(*a);
	a->x *= d;
	a->y *= d;
	a->z *= d;
}

/**
 * @brief change a vecto to have the total lenth of 1, no change direction
 * @param a [t_vec] vector to operade on
 * @return [void]
*/
t_vec	v_unit(t_vec	a)
{
	t_vec	res;
	double	d;

	d = 1 / veclen(a);
	res.x = a.x * d;
	res.y = a.y * d;
	res.z = a.z * d;
	return (res);
}

/**
 * @brief calculate the angle between two vectors
 * @param a [t_vec]
 * @param b [t_vec]
*/
double	angle(t_vec a, t_vec b)
{
	double	scalar;

	scalar = calculate_dot(&a, &b);
	return ((acos(scalar / (veclen(a) * veclen(b)))));
}

/**
 * @brief Return the dot calculations of two Vectors
 * 
 * @param a Pointer of type double
 * @param b Pointer of type double
 * @return double
 */
double	calculate_dot(t_vec *a, t_vec *b)
{
	double	dot;

	if (b != NULL)
		dot = a->x * b->x + a->y * b->y + a->z * b->z;
	else
		dot = a->x * a->x + a->y * a->y + a->z * a->z;
	return (dot);
}

/**
 * @brief return a vector wit all the inverted values of the argument
 * @param a vector to invert
*/
t_vec	v_invert(t_vec a)
{
	t_vec	res;

	res.x = -a.x;
	res.y = -a.y;
	res.z = -a.z;
	return (res);
}

/**
 * @brief rotate a vector around the vertical axis
 * 
 * @param org 
 * @param deg 
 * @return t_vec 
 */
t_vec	rotate_Z(t_vec org, double deg)
{
	t_vec	res;
	double rad = deg * PI / 180;

	res.x = org.x * cos(rad) - org.y * sin(rad);
	res.y = org.x * sin(rad) + org.y * cos(rad);
	res.z = org.z;
	return (res);
}

/**
 * @brief tilt a vector up od down, independent of axial orientation
 * 
 * @param org 
 * @param up 
 * @return t_vec 
 */
t_vec	tilt(t_vec org, bool up)
{
	t_vec	res;

	double off;
	if (up)
		off = 1.1;
	else
		off = 0.9;
	t_vec	orth = v_unit(cross(org, init_vec(0, 0, 1)));
	t_vec	ninety = v_unit(cross(org, orth));
	t_vec	scale = v_product(connect(ninety, v_unit(org)), off);
	return (connect(init_vec(0, 0, 0), v_sum(scale, ninety)));
}

