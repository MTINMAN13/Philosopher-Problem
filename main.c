/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mman <mman@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 17:52:25 by mman              #+#    #+#             */
/*   Updated: 2024/03/21 21:00:00 by mman             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	ft_get_current_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		write(2, "gettimeofday() error\n", 22);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_param_check(int argc, char *argv[])
{
	if (argc == 5)
	{
		atoi(argv[1]);
		atoi(argv[2]);
		atoi(argv[3]);
		atoi(argv[4]);
	}
	else if (argc == 6)
		atoi(argv[5]);
	else
	{
		printf("Please use 4 to 5 arguments; \n1 [PHILO COUNT]\n2 [TIME TO DIE]\n3 [TIME TO EAT]\n4 [TIME TO SLEEP]\n5 [EAT COUNT MAX](optional)\n\n\n");
		exit(EXIT_FAILURE);
	}
}

void	ft_mutex_init(pthread_mutex_t **forks, int count)
{
	count--;
	while (count >= 0)
	{
		forks[count] = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
		if (forks[count] == 0)
			exit(EXIT_FAILURE);
		pthread_mutex_init(forks[count], NULL);
		count--;
	}
}

void	ft_mutex_destroy(pthread_mutex_t **forks, int count)
{
	count--;
	while (count >= 0)
	{
		pthread_mutex_destroy(forks[count]);
		free(forks[count]);
		count--;
	}
}

int	ft_util_lfork(int position, int count)
{
	if (position != 0)
		return (position - 1);
	return (count - 1);
}

void	*ft_thread(void *arg)
{
	t_philo	*guy;
	int		id;

	guy = (t_philo *)arg;
	id = guy->id;
	printf("Executing Philo   %i\n", id);
	usleep(300);
}

void	ft_cleanup(t_philo *philo_data)
{
	free(philo_data);
}

void	ft_exiter(int count, t_philo *philo_data, pthread_mutex_t **forks)
{
	ft_cleanup(philo_data);
	ft_mutex_destroy(forks, count);
}

void	ft_program(int count, char *argv[], pthread_mutex_t **forks)
{
	t_philo		*philo_data;
	int			buff;

	buff = 0;
	philo_data = malloc(sizeof(t_philo) * count);
	while (buff < count)
	{
		philo_data[buff].state = 3;
		philo_data[buff].id = buff + 1;
		philo_data[buff].eat_last = 0;
		philo_data[buff].time_to_die = atoi(argv[2]);
		philo_data[buff].time_to_eat = atoi(argv[3]);
		philo_data[buff].time_to_sleep = atoi(argv[4]);
		philo_data[buff].max_optional_count = atoi(argv[5]);
		philo_data[buff].left = forks[ft_util_lfork(buff, count)];
		philo_data[buff].right = forks[buff % (count - 1)];
		pthread_create(&philo_data[buff].thread, NULL,
			ft_thread, &philo_data[buff]);
		// // create a thread
		// printf("this was %i\n", buff);
		// printf("id was %i\n", philo_data[buff].id);
		usleep(300);
		buff++;
	}
	buff--;
	while (buff >= 0)
	{
		// join a thread
		pthread_join(philo_data[buff].thread, NULL);
		buff--;
	}
	//free stuff at the end
	ft_cleanup(philo_data);
	//  'Count' Philosopher sits next to Philosopher 1
	printf("\nClean Exit 1\n");
}

int	main(int argc, char *argv[])
{
	pthread_mutex_t	*forks[MAX_PHILOSOPHERS];
	int				philo_count;

	ft_param_check(argc, argv);
	philo_count = atoi(argv[1]);
	ft_mutex_init(forks, philo_count);
	ft_program(philo_count, argv, forks);
	ft_mutex_destroy(forks, philo_count);
	printf("\nClean Exit 0\n");
	return (EXIT_SUCCESS);
}
