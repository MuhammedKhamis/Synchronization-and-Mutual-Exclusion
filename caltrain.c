#include <pthread.h>
#include "caltrain.h"
#include <stdio.h>

void debug(const char* word){
  // for debuging
  printf("%s\n",word);
}


void station_init(struct station *station){
  // Lock, no one can work without it.
  pthread_mutex_init(&(station->lock), NULL);
  // condition train arriving the station.
  pthread_cond_init(&(station->trainInStation), NULL);
  // condtion for passenger in the train.
  pthread_cond_init(&(station->inTheTrain), NULL);
  // condtion that train is full and ready to go.
  pthread_cond_init(&(station->trainFull), NULL);
  // number of passengers that will go from outside to the inside of the train.
  station->passengersToEnter=0;
  // number of passengers waiting outside for the train.
  station->passengersOut=0;
 }

/** Invokes when train arrives,
The function must not return until the train is satisfactorily loaded.

the train must leave the station promptly,
if no passengers are waiting at the station or it has no available free seats.
*/
void station_load_train(struct station *station, int count){

    pthread_mutex_lock(&(station->lock));
//////////////////////////////////Critical Section//////////////////////////////
    while(station->passengersOut > 0 && count > 0){
      // signal for each thread at a time.
        pthread_cond_signal(&(station->trainInStation));
        // decrese number of seats by 1.
        count--;
        // wait until passenger set in the train.
        pthread_cond_wait(&(station->inTheTrain),&(station->lock));
    }
      // wait until they tell us to leave.
      if(station->passengersToEnter > 0){
        pthread_cond_wait(&(station->trainFull), &(station->lock));
      }
    ////////////////////////////////////////////////////////////////////////////////
    pthread_mutex_unlock(&(station->lock));

}
/**
When a passenger robot arrives in a station ->

This function must not return until a train is in the station.
and there are enough free seats on the train for this passenger to sit down.
Once this function returns, the passenger robot will move the passenger on board
the train and into a seat.
*/
void station_wait_for_train(struct station *station){

  pthread_mutex_lock(&(station->lock));
  /////////////////////////Critical Section/////////////////////////////////////
  // increase number of passengers outside by one.
  station->passengersOut++;
  // wait until the train arrives.
  pthread_cond_wait(&(station->trainInStation),&(station->lock));
  //the train has come :D

  // number of passengers that entered from outside to the inside increased by one.
  station->passengersToEnter++;

  // enumber of passengers out decreased by one.
  station->passengersOut--;

  // tell the train we are ready and set for the next passenger.
  pthread_cond_broadcast(&(station->inTheTrain));
  //////////////////////////////////////////////////////////////////////////////
  pthread_mutex_unlock(&(station->lock));

}
/**
  Once the passenger is seated, it will call the function
  to let the train know that it’s on board.
*/
void station_on_board(struct station *station){

  pthread_mutex_lock(&(station->lock));
  /////////////////////////Critical Section/////////////////////////////////////
  // number of passengers entered decreased by one.
  station->passengersToEnter--;

  // checked all are set in place
  if(!(station->passengersToEnter)){
    // tell the train to leave the station.
    pthread_cond_broadcast(&(station->trainFull));
  }
  //////////////////////////////////////////////////////////////////////////////
  pthread_mutex_unlock(&(station->lock));
}
