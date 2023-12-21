#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_CARS 5

sem_t mutex;
sem_t bridge;
int bidgecar = 0;

void passing(int dir, int car_num)
{
    printf("Car %d from side %s is on the bridge\n", car_num, (dir == 0) ? "left" : "right");
    sleep(3);
    printf("Car %d from side %s has crossed the bridge\n", car_num, (dir == 0) ? "left" : "right");
}

void *car_thread(void *args)
{
    int *carinfo = (int *)args;
    int dir = carinfo[0];
    int car_num = carinfo[1];

    sem_wait(&mutex);
    while (bidgecar >= MAX_CARS)
    {
        sem_post(&mutex);
        sleep(1);
        sem_wait(&mutex);
    }
    bidgecar++;
    sem_post(&mutex);

    sem_wait(&bridge);
    passing(dir, car_num);
    sem_post(&bridge);

    sem_wait(&mutex);
    bidgecar--;
    sem_post(&mutex);

    free(carinfo);
    pthread_exit(NULL);
}

int main()
{
    int num_left, num_right;
    printf("Enter the number of cars on the left side: ");
    scanf("%d", &num_left);
    printf("Enter the number of cars on the right side: ");
    scanf("%d", &num_right);

    sem_init(&mutex, 0, 1);
    sem_init(&bridge, 0, MAX_CARS);

    pthread_t cars[num_left + num_right];

    for (int i = 0; i < num_left + num_right; i++)
    {
        int *carinfo = (int *)malloc(2 * sizeof(int));
        if (i < num_left)
        {
            carinfo[0] = 0; // 0 represents left side
            carinfo[1] = i + 1;
            pthread_create(&cars[i], NULL, car_thread, carinfo);
        }
        else
        {
            carinfo[0] = 1; // 1 represents right side
            carinfo[1] = i - num_left + 1;
            pthread_create(&cars[i], NULL, car_thread, carinfo);
        }
    }

    for (int i = 0; i < num_left + num_right; i++)
    {
        pthread_join(cars[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&bridge);

    return 0;
}
