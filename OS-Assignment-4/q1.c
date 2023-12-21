#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define totalPhilosophers 5

pthread_mutex_t forks[totalPhilosophers];
pthread_mutex_t bowl; 
pthread_cond_t can_eat;            
int eating_count = 0;               
int max_allowed_eating = 2;        

void eating(int id) {
    printf("Philosopher %d is Eating\n", id);
    sleep(2); 
}

void thinking(int id) {
    printf("Philosopher %d is Thinking\n", id);
    sleep(1);
}

void* philosopher(void* args) {
    int id = *((int*)args);
    int left_fork = id;
    int right_fork = (id + 1) % totalPhilosophers;

    while (1) {
        thinking(id);
        // thinkgin to eat or not 
        
        pthread_mutex_lock(&bowl);
        while (eating_count >= max_allowed_eating) {
            pthread_cond_wait(&can_eat, &bowl);
        }
        eating_count++; 
        pthread_mutex_unlock(&bowl);

        pthread_mutex_lock(&forks[left_fork]);
        pthread_mutex_lock(&forks[right_fork]);

        eating(id);

        pthread_mutex_unlock(&forks[right_fork]);
        pthread_mutex_unlock(&forks[left_fork]);

        pthread_mutex_lock(&bowl);
        eating_count--; 
        pthread_cond_signal(&can_eat); 
        pthread_mutex_unlock(&bowl);

        thinking(id);
    }

    return NULL;
}

int main() {
    pthread_t philosophers[totalPhilosophers];
    int ids[totalPhilosophers];

    pthread_mutex_init(&bowl, NULL);
    pthread_cond_init(&can_eat, NULL);

    for (int i = 0; i < totalPhilosophers; ++i) {
        pthread_mutex_init(&forks[i], NULL);
    }


    for (int i = 0; i < totalPhilosophers; i++)
    {
        
    }
    

  
    for (int i = 0; i < totalPhilosophers; ++i) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < totalPhilosophers; ++i) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < totalPhilosophers; ++i) {
        pthread_mutex_destroy(&forks[i]);
    }

    pthread_mutex_destroy(&bowl);
    pthread_cond_destroy(&can_eat);

    return 0;
}
