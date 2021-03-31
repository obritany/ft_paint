#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct	s_area
{
	int		width;
	int		height;
	char	empty;
	char	*map;
}				t_area;

typedef struct	s_rect
{
	char	type;
	float	x;
	float	y;
	float		width;
	float		height;
	char	color;
}				t_rect;

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

void ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

void draw_pix(t_area *area, t_rect *rect, int y, int x)
{
	if (y < rect->y || y > (rect->y + rect->height) || x < rect->x || x > (rect->x + rect->width))
		return ;
	
	if (rect->type == 'R' ||  (y - rect->y) < 1.0 || (x - rect->x) < 1.0 || (rect->y + rect->height - y) < 1.0 || (rect->x + rect->width - x) < 1.0)
		area->map[y * area->width + x] = rect->color;
}

int draw_rect(t_area *area, t_rect *rect)
{
	int	y;
	int	x;

	if (rect->width <= 0 || rect->height <= 0 || (rect->type != 'r' && rect->type != 'R'))
		return (1);

	y = 0;
	while (y < area->height)
	{
		x = 0;
		while (x < area->width)
		{
			draw_pix(area, rect, y, x);
			x++;
		}
		y++;
	}
	return (0);	
}

void print_map(t_area *area)
{
	int i;

	i = 0;
	while (i < area->width * area->height)
	{
		write(1, &(area->map[i]), area->width);
		write(1, "\n", 1);
		i += area->width;
	}
}

int read_file(FILE *file)
{
	t_area	area;
	t_rect	rect;
	int		rslt;
	int		i;
	
	if (!file)
		return (1);
	if (fscanf(file, "%d %d %c\n", &area.width, &area.height, &area.empty) != 3)
		return (2);
	if (((area.width < 1) || (area.width > 300) || (area.height < 1)) || (area.height > 300))
		return (3);
	if (!(area.map = malloc(area.width * area.height)))
		return (4);

	i = 0;
	while (i < area.width * area.height)
		area.map[i++] = area.empty;

	while ((rslt = fscanf(file, "%c %f %f %f %f %c", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.color)) == 6)
	{
		if (draw_rect(&area, &rect))
			return (5);
	}
	if (rslt == -1)
	{
		print_map(&area);
		return (0);
	}

	return (6);
}

int main(int argc, char *argv[])
{
	FILE *file;
	
	if (argc == 2)
	{
		file = fopen(argv[1], "r");
		if (!read_file(file))
			return (0);
		ft_putstr("Error: Operation file corrupted\n");
	}
	else
		ft_putstr("Error: argument\n");

	return (1);
}
