/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/21 15:24:58 by fhenrion          #+#    #+#             */
/*   Updated: 2021/03/21 20:19:47 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ARGS_H
# define ARGS_H

# include <string.h>
# include <stdlib.h>

int check_nonce(const char *str);
uint64_t get_nonce(const char *str);
const char *output_filename(int option, const char *input_filename);
int parse_option(const char *arg);

#endif