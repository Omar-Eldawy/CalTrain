#include <pthread.h>
#include <stdio.h>
#include "caltrain.h"

//int i=1,j=1,k=1;
void
station_init(struct station *station)
{
	station->waiting_passengers = 0;
    station->empty_seats = 0;
    station->valid_seats = 0;
    pthread_mutex_init(&station->lock, NULL);
    pthread_cond_init(&station->available_train, NULL);
    pthread_cond_init(&station->available_seat, NULL);
    pthread_cond_init(&station->train_can_go,NULL);
}

void
station_load_train(struct station *station, int count)
{
//    j=1;
//    k=1;
    if(count !=0 && station->waiting_passengers !=0) {
        station->valid_seats = count;
        station->empty_seats = (station->waiting_passengers > count) ? count : station->waiting_passengers;
        pthread_cond_broadcast(&station->available_train);
        pthread_cond_signal(&station->available_seat);
        pthread_cond_wait(&station->train_can_go, &station->lock);
        station->valid_seats = 0;
    }
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->lock);
    station->waiting_passengers++;
//    printf("Passenger %d is waiting for train\n",i++);
    pthread_cond_wait(&station->available_train, &station->lock);
    pthread_cond_wait(&station->available_seat, &station->lock);
    station->waiting_passengers--;
    station->valid_seats--;
//    printf("Passenger %d is on board\n",j++);
    pthread_mutex_unlock(&station->lock);
    if(station->valid_seats != 0) {
        pthread_cond_signal(&station->available_seat);
    }
}

void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->lock);
    station->empty_seats--;
//    printf("Passenger %d sat down\n empty seats = %d\n",k++,station->empty_seats);
    if(station->empty_seats == 0){
        pthread_cond_signal(&station->train_can_go);
    }
    pthread_mutex_unlock(&station->lock);
}