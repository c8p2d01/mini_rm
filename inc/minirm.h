#ifndef MINIRM_H
# define MINIRM_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <math.h>
# include "../MLX42/include/MLX42/MLX42.h"
# include "../src/lft/libft.h"
/*
 * DEFINES
 */

# define RENDER_DISTANCE 20000
# define WDTH 400
# define HGHT 400
# define DIVERGENCE 100
# define PI 3.14159265359

# define BLACK "\033[30m"
# define GREY "\033[90m"
# define RED "\033[91m"
# define CYAN "\033[36m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[94m"
# define MAGENTA "\033[35m"

# define NORMAL "\033[0m"
# define FAT "\033[1m"
# define DIM "\033[2m"
# define UNDERLINE "\033[4m"
# define CROSS "\033[9m"
# define BACKGROUND "\033[7m"
# define GONE "\033[8m"
# define CLEAR_TERM "\e[1;1H\e[2J"

/**
 * struct to represent vectors as one unit and to make returns easier
 * this includes directio vectors and location vectors
 * @param x [double] Value for the X-Component of the vector
 * @param y [double] Value for the Y-Component of the vector
 * @param z [double] Value for the Z-Component of the vector
*/
typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}	t_vec;

/**
 * struct for the Ambient lighting
 * @param id [char] char identifier
 * @param lr [double] lighting ratio [0.0 - 1.0]
 * @param r [int] Red [0 - 255]
 * @param g [int] Green [0 - 255]
 * @param b [int] Blue [0 - 255]
*/
typedef struct s_al
{
	char	id;
	double	lr;
	int		r;
	int		g;
	int		b;
}	t_al;

/**
 * struct for the Camara; If v_o == [0][0][0], -> [1][0][0]
 * @param id [char] char identifier
 * @param cor [double[3]] coordinates of view point [x][y][z]
 * @param v_o [double[3]] orientation vector [x][y][z] [-1 - 1]
 * @param fov [double] field of view in degrees [0 - 180]
*/
typedef struct s_cam
{
	char	id;
	t_vec	cor;
	t_vec	v_o;
	double	fov;
}	t_cam;

/**
 * struct for Light
 * @param id [char] char identifier
 * @param cor [double[3]] coordinates of view point [x][y][z]
 * @param lr [double] lighting ratio [0.0 - 1.0]
 * @param r [int] Red [0 - 255]
 * @param g [int] Green [0 - 255]
 * @param b [int] Blue [0 - 255]
*/
typedef struct s_light
{
	char	id;
	t_vec	cor;
	double	lr;
	int		r;
	int		g;
	int		b;
}	t_light;

/**
 * struct for object
 * @param id [char] char identifier S = sphere; P = plane; Z = cylinder
 * @param cor [double[3]] coordinates of view point [x][y][z]
 * @param v_o [double[3]] 3d normalization orientation vector [x][y][z] [-1 - 1]
 * @param rad [double] object rad 
 * @param hght [double] object hight
 * @param r [int] Red [0 - 255]
 * @param g [int] Green [0 - 255]
 * @param b [int] Blue [0 - 255]
*/
typedef struct s_obj
{
	char	id;
	t_vec	cor;
	t_vec	v_o;
	double	rad;
	double	hght;
	int		r;
	int		g;
	int		b;
}	t_obj;

typedef struct s_mrt
{
	void	*mlx;
	void	*img;
	t_al	*al;
	t_light	**l;
	t_cam	*cam;
	t_obj	**obj;
	t_obj	tmp;
}				t_mrt;

//GEOMETRY-

t_vec	cross(t_vec a, t_vec b);
t_vec	init_vec(double x, double y, double z);
t_vec	*vec_alloc(t_vec clone);
double	veclen(t_vec a);
t_vec	connect(t_vec a, t_vec b);
void	addto(t_vec *a, t_vec b);
t_vec	v_sum(t_vec a, t_vec b);
void	product(t_vec *a, double m);
t_vec	v_product(t_vec a, double m);
void	resize(t_vec *a, double m);
void	unit(t_vec	*a);
t_vec	v_unit(t_vec	a);
double	angle(t_vec a, t_vec b);
double	calculate_dot(t_vec *a, t_vec *b);
t_vec	v_invert(t_vec a);
t_vec	rotate_Z(t_vec org, double deg);
t_vec	tilt(t_vec org, bool up);
t_vec	reflect(t_vec in, t_vec norm);// EXIT

// DISTANCES
double	min_dst(t_obj **objs, t_vec point, t_obj **hit);
double	s_sphere(t_obj *sph, t_vec *point);

// INPUT

void	*extract_line(char **lines, t_mrt *mrt);//proto

//UTILES

void	rt_er_exit(t_mrt *mrt, int good, int count);
int		rt_exit(t_mrt *mrt);

int		create_trgb(int t, int r, int g, int b);
void	my_mlx_pixel_put(void *mlx, int x, int y, int color);

char	*trm_whtsp(char *s, int dir);
int		is_whspace(char *s, int dir);
int		ft_white(char c);
char	**split_wh(char const *s);

int		count_input(t_list *lst, int *count, char *tmp);
int		check_count(int *count);
void	limit(double *var, double upper, double lower);
double	i_limit(double var, double upper, double lower);

//INITIALIZATION

int		check_deci(char *s);
int		init_cor(char **info, t_vec *cor, int flag);
int		init_rgb(char **info, int *r, int *g, int *b);
int		init_fov(char *info, double *fov);
int		init_dim(char *info, double *dim, int flag);

int		init_al(t_al *al, char **info);
int		init_cam(t_cam *ca, char **info);
int		init_light(t_light **l, char **info, int p);

int		init_sph(t_obj **sp, char **info, int p);
int		init_pl(t_obj **pl, char **info, int p);
int		init_cyl(t_obj **cy, char **info, int p);

//FUNCTIONS

int		input(t_mrt *mrt, char *file);
t_vec	*screen(t_cam *cam);
t_vec	single_ray(int x, int y, t_cam *cam, t_vec	scr[3]);
void	ray(t_mrt *mrt, int x, int y, t_vec *scr, bool print);
void	retrace(t_mrt *mrt);
void	reorient(t_mrt *mrt, char dir);


void	render(t_mrt *mrt);

#endif