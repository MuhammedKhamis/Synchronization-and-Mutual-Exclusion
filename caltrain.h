#ifndef CALTRAIN_H
#define CALTRAIN_H

#include <pthread.h>

struct station {
  pthread_mutex_t lock;
  pthread_cond_t trainInStation;
  pthread_cond_t inTheTrain;
  pthread_cond_t trainFull;
  int emptySeates;
  int passengersToEnter;
  int passengersOut;
};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);

#endif /* CALTRAIN_H */
