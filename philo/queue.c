/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frafal <frafal@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 10:33:21 by frafal            #+#    #+#             */
/*   Updated: 2023/01/27 15:40:26 by frafal           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	*init_queue(t_data *data)
{
	int	i;
	int	start;

	data->queue = malloc(data->num * sizeof (int));
	if (data->queue == NULL)
	{
		printf("malloc fail\n");
		return (NULL);
	}
	i = 0;
	start = 1;
	if (data->num == 1)
	{
		data->queue[i] = start;
		return (data->queue);
	}
	while (i < data->num / 2)
	{
		data->queue[i] = start;
		start = start + data->num / 2;
		i++;
	}
	return (data->queue);
}

void	print_queue(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num / 2)
	{
		printf("%d, ", data->queue[i]);
		i++;
	}
	printf("\n");
}
