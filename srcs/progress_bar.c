/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progress_bar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 15:13:50 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/21 15:37:22 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/progress_bar.h"

void ft_progress(size_t total) {
	static size_t count = 1;
	int bar = count * BAR_LENGTH / total;
	int load = BAR_LENGTH - bar;
	int percent = count * 100 / total;

	if (count == 1) {
		printf("\e[?25l");
	}
	printf("\rProgress [%.*s%.*s] %i%%", bar, BAR, load, LOAD, percent);
	fflush(stdout);
	if (count < total) {
		++count;
	}	
}

void ft_progress_end(void) {
	printf("\e[?25h");
	write(1, "\n", 1);
}
