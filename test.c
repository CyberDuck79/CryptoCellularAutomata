/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/30 16:25:19 by fhenrion          #+#    #+#             */
/*   Updated: 2019/08/30 16:50:29 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// programme de test de repetition dans sequence pseudo aleatoire

#include "header.h"

int		sequence_size(int fd)
{
	int		size = 0;
	char	c;

	while (read(fd, &c, 1))
	{
		if (c == "\n")
			size++;
	}
	return (size);
}

ull		**parse_sequence(int fd, int size)
{
	int	i = 0;
	int buffer[20];
	ull	**sequence;

	if (!(sequence = (ull**)malloc(sizeof(ull*) * size)))
		return (NULL);
	while (i < size)
	{
		sequence[i] = (ull*)malloc(sizeof(ull));
		read(fd, buffer, 20);
		if (!(sequence[i] = strtoull(buffer, NULL, 10)))
			return (NULL);
		read(fd, buffer, 1);
		i++;
	}
	return (sequence);
}

int		main(int argc, char **argv)
{
	int	fd;
	int size;
	ull	**sequence;

	if (argc == 2)
	{
		if ((fd = open(argv[1], O_RDONLY)) == -1)
		{
			perror("open failed");
			exit(EXIT_FAILURE);
		}
		size = sequence_size(fd);
		close(fd);
		fd = open(argv[1], O_RDONLY);
		sequence = parse_sequence(fd, size);
		// launch analysis
	}
	return (0);
}