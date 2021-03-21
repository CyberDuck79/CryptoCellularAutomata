/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   progress_bar.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fhenrion <fhenrion@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/13 15:13:47 by fhenrion          #+#    #+#             */
/*   Updated: 2020/11/06 15:21:55 by fhenrion         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROGRESS_BAR_H
# define PROGRESS_BAR_H

# include <stddef.h>
# include <unistd.h>
# include <fcntl.h>
# include <string.h>
# include <stdio.h>

# define BAR_LENGTH 60
# define BAR "============================================================"
# define LOAD "                                                            "

void	ft_progress(size_t total);
void	ft_progress_end(void);

#endif
