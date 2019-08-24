/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/08/24 13:17:34 by fhenrion          #+#    #+#             */
/*   Updated: 2019/08/24 14:50:06 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

char	*add_extension(char *str)
{
	char	*output;

	output = (char*)malloc(sizeof(str) + 3);
	output = strcpy(output, str);
	output = strcat(output, ".ca");
	return (output);
}

char	*remove_extension(char *str)
{
	char	*output;

	output = (char*)malloc(sizeof(str) - 3);
	output = strncpy(output, str, strlen(str) - 3);
	output[strlen(str) - 3] = '\0';
	return (output);
}

int		check_extension(char *str)
{
	if (strcmp(str + (strlen(str) - 3), ".ca"))
		return (0);
	return (1);
}

void	open_error(int open)
{
	
	if (open)
		perror("open failed");
	else
		write(2, "open failed: Not correct file extension\n", 40);
	exit(EXIT_FAILURE);
}

int		check_option(char *str)
{
	if (!strcmp("-e", str))
		return (1);
	else if (!strcmp("-d", str))
		return (1);
	return (0);
}