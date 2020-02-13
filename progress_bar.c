/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progress_bar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 15:13:50 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/13 15:15:42 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "progress_bar.h"

void	ft_progress(size_t total)
{
	static size_t	count = 1;
	int				bar = count * BAR_LENGTH / total;;
	int				load = BAR_LENGTH - bar;
	int				percent = count * 100 / total;

	fflush(stdout);
	printf("\rProgress [%.*s%.*s] %i%%", bar, BAR, load, LOAD, percent);
	if (count < total)
		count++;
}