/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 11:13:11 by frafal            #+#    #+#             */
/*   Updated: 2023/01/19 16:43:01 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_data
{
	int				num;
	int				ttd;
	int				tte;
	int				tts;
	int				eat_times;
	struct timeval	tv0;
	struct timeval	tv1;
}	t_data;

int	ft_atoi(const char *nptr);

#endif