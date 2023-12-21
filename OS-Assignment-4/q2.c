#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>



sem_t mutex, boardQueue, unboardQueue, allAboard, allAshore , temp;
int totalPassengers, currentPassengers = 0;

int MAX_CAPACITY;
int x  = 0;
void *car(void *args) {
    while (1) {
        sem_wait(&boardQueue);  
        // printf("Car is loading passengers...\n");
        load();
        sleep(1); // Simulating loading time
        sem_post(&allAboard);   
        printf("Car is full. Ride begins now!\n");

        sem_wait(&unboardQueue); 
        // printf("Car is unloading passengers...\n");
        unload();
        sleep(1); // Simulating unloading time
        sem_post(&allAshore);  
    }
    return NULL;
}

void *passenger(void *args) {
    int id = *(int *)args;
    while (1) {
        sem_wait(&mutex);
        if (currentPassengers < MAX_CAPACITY) {
            // printf("Passenger %d is boarding the car.\n", id);
            board(id);
            currentPassengers++;
            if (currentPassengers == MAX_CAPACITY) {
                sem_post(&boardQueue);  
                sem_wait(&allAboard);   
                sem_post(&unboardQueue); 
                sem_wait(&allAshore);   
                currentPassengers = 0;
                x = 0;
                sem_post(&temp);
            }
        }
        sem_post(&mutex);

        // sem_wait(&temp);
        if(x == 1){
            offboard(id);
            sleep(1);
            x = 1;
        }
        // Simulate passenger waiting before trying to board again

        sleep(2);
    }
    return NULL;
}

void board(int id){
    printf("Passenger %d is boarding the car.\n", id);
    sleep(1);   
}

void unload(){
    printf("Car is unloading passengers...\n");
    sleep(1);
}
void load(){
    printf("Car is loading passengers...\n");
    sleep(1);
}
void offboard(int id){
    printf("Passenger %d is offboarding \n",id);
}

int main() {
    printf("Enter the total number of passengers: ");
    scanf("%d", &totalPassengers);
    printf("Enter the maximum capacity of the car: ");
    scanf("%d", &MAX_CAPACITY);

    pthread_t car_thread, passenger_threads[totalPassengers];
    int passenger_ids[totalPassengers];

    for (int i = 0; i < totalPassengers; i++)
    {
        
    }
    sem_init(&mutex, 0, 1);
    sem_init(&boardQueue, 0, 0);
    sem_init(&unboardQueue, 0, 0);
    sem_init(&allAboard, 0, 0);
    sem_init(&allAshore, 0, 0);
    sem_init(&temp, 0, 0);

    
    
    pthread_create(&car_thread, NULL, car, NULL);

    for (int i = 0; i < totalPassengers; ++i) {
        passenger_ids[i] = i + 1;
        pthread_create(&passenger_threads[i], NULL, passenger, &passenger_ids[i]);
    }

    pthread_join(car_thread, NULL);

    for (int i = 0; i < totalPassengers; ++i) {
        pthread_join(passenger_threads[i], NULL);
    }

    sem_destroy(&mutex);
    sem_destroy(&boardQueue);
    sem_destroy(&unboardQueue);
    sem_destroy(&allAboard);
    sem_destroy(&allAshore);
    sem_destroy(&temp);
    return 0;
}
