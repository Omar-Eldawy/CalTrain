#include <pthread.h>
#include <stdio.h>
#include "caltrain.h"

void
station_init(struct station *station) {
    station->waiting_passengers = 0;
    station->empty_seats = 0;
    station->passengers_on_board = 0;
    pthread_mutex_init(&station->lock, NULL);
    pthread_cond_init(&station->available_train, NULL);
    pthread_cond_init(&station->train_can_go, NULL);
}

void
station_load_train(struct station *station, int count) {
    pthread_mutex_lock(&station->lock);

        //check if the train has empty seats and there are passengers waiting otherwise train leaves the station
        if (count && station->waiting_passengers) {
            station->empty_seats = count;

            //wake up all passengers waiting for the train
            pthread_cond_broadcast(&station->available_train);

            //while loop to handle spurious wakeup
            while (station->empty_seats && station->waiting_passengers) {
                //wait for all passengers to board the train
                pthread_cond_wait(&station->train_can_go, &station->lock);
            }

            station->passengers_on_board = 0;
        }

    pthread_mutex_unlock(&station->lock);
}

void
station_wait_for_train(struct station *station) {
    pthread_mutex_lock(&station->lock);

        station->waiting_passengers++;

        //wait for a train to arrive
        //as there is no train available, no empty seats or passengers on board
        //but when the train arrives, empty seats will be available and passengers on board remains 0
        while (station->passengers_on_board == station->empty_seats) {
            pthread_cond_wait(&station->available_train, &station->lock);
            //passengers try to board the train
            //check if there are empty seats remaining
        }

        //passenger who successfully board the train
        station->waiting_passengers--;
        station->passengers_on_board++;

    pthread_mutex_unlock(&station->lock);
}

void
station_on_board(struct station *station) {
    pthread_mutex_lock(&station->lock);

        //passenger has sat down
        station->passengers_on_board--;
        station->empty_seats--;

        //if there are no more empty seats or no more passengers waiting for the train
        //and all passengers on board sat down, train can leave
        if (!station->empty_seats || (!station->passengers_on_board && !station->waiting_passengers)) {
            pthread_cond_signal(&station->train_can_go);
        }

    pthread_mutex_unlock(&station->lock);
}