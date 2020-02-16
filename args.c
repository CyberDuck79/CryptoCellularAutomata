/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:55:45 by fhenrion          #+#    #+#             */
/*   Updated: 2020/02/16 13:57:14 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "ca256.h"

static char	*add_extension(char *name)
{
	char	*new;
	size_t	len;

	len = strlen(name);
	new = (char*)malloc(len + 4);
	strcpy(new, name);
	strlcpy(new + len, ".ca", 4);
	return (new);
}

static char	*remove_extension(char *name)
{
	char	*new;
	size_t	len;

	len = strlen(name) - 2;
	new = (char*)malloc(len);
	strlcpy(new, name, len);
	return (new);
}

char		*parse_option(char *option, char *input_file)
{
	if (!strcmp(option, "-e"))
		return (add_extension(input_file));
	else if (!strcmp(option, "-d"))
		return (remove_extension(input_file));
	write(1, "error option\n", 13);
	return (NULL);
}