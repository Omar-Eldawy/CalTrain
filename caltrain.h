#include <pthread.h>

struct station {
	int waiting_passengers;
    int valid_seats;
    int empty_seats;
    pthread_mutex_t lock;
    pthread_cond_t available_train;
    pthread_cond_t available_seat;
    pthread_cond_t train_can_go;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);