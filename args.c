/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:55:45 by fhenrion          #+#    #+#             */
/*   Updated: 2020/11/06 14:58:23 by fhenrion         ###   ########.fr       */
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
	strncpy(new + len, ".ca", 4);
	return (new);
}

static char	*remove_extension(char *name)
{
	char	*new;
	size_t	len;

	len = strlen(name) - 2;
	if (strncmp("ca", name + len, 2))
		return (NULL);
	new = (char*)malloc(len);
	strncpy(new, name, len);
	return (new);
}

char		*parse_option(char *option, char *input_file)
{
	if (!strcmp(option, "-e"))
		return (add_extension(input_file));
	else if (!strcmp(option, "-d"))
		return (remove_extension(input_file));
	return (NULL);
}
