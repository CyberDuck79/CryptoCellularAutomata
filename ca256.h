/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ca256.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/17 16:09:21 by fhenrion          #+#    #+#             */
/*   Updated: 2020/01/21 10:54:03 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CA256_H
# define CA256_H

# include <stddef.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>

typedef unsigned long long	ull;

#define B(x) (1ULL << (x))

typedef enum	e_rule
{
	RULE0 = 90,
	RULE1 = 105,
	RULE2 = 150,
	RULE3 = 165,
}				rule;

#endif
