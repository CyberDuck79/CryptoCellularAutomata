/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/16 13:55:45 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/21 15:54:49 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/args.h"

static const char *add_extension(const char *name) {
	size_t len = strlen(name);
	char *new = (char*)malloc(len + 4);
	if (!new) {
		return NULL;
	}
	strncpy(new, name, len);
	strncpy(new + len, ".ca", 4);
	return new;
}

static const char *remove_extension(const char *name) {
	size_t len = strlen(name) - 3;
	char *new = (char*)malloc(len + 1);
	if (!new) {
		return NULL;
	}
	new[len] = '\0';
	return strncpy(new, name, len);
}

const char *output_filename(int option, const char *input_filename) {
	if (option) {
		return add_extension(input_filename);
	}
	return remove_extension(input_filename);
}

int parse_option(const char *arg) {
	if (!strncmp(arg, "-e", 3)) {
		return 1;
	} else if (!strncmp(arg, "-d", 3)) {
		return 0;
	}
	return -1;
}
