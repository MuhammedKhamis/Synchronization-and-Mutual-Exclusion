#include <pthread.h>
#include "caltrain.h"
#include <stdio.h>

void debug(const char* word){
  // for debuging
  printf("%s\n",word);
}


void station_init(struct station *station){
  // dah el lock bta3i ma7desh y2dar yeshta8al me 8ero
  pthread_mutex_init(&(station->lock), NULL);
  // el train wasal el m7ata
  pthread_cond_init(&(station->trainInStation), NULL);
  // el passenger rakab el train 5las
  pthread_cond_init(&(station->inTheTrain), NULL);
  // el train 5las 5las we gahez ino yeseb el m7ata
  pthread_cond_init(&(station->trainFull), NULL);
  // 3ddad el nas el 7aterkab me bara le gowa el train
  station->passengersToEnter=0;
  // 3ddad el nas ely mestanya bara
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
    // pthread_cond_broadcast(&(station->trainInStation));
    // bug Here if broadcast : 3shan low inta sa7ethom kolohm mmomken yego wara b3d we mayb2ash
    // lltrain forsa inoh yege fe elnos mmken yegi fe el 25er we tkon 25dt 2ktar men ele inta 3ayzoh we ydrab
    // fa enta lazem tsa7y wa7d wa7d wara b3d mesh m3 ba3d
    while(station->passengersOut > 0 && count > 0){
      // Hena basa7y wa7d at a time.
        pthread_cond_signal(&(station->trainInStation));
        //decrese 3dd el krasy el gowa by 1
        count--;
      // estana l7ad ma el passenger yed5ol we y23od b3d keda kamel
        pthread_cond_wait(&(station->inTheTrain),&(station->lock));
    }
      // wait until they tell us to leave n3ed kol el nas 2bl ma nemshy
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
  // bashof kam wa7d 3ayez el train
  station->passengersOut++;
  // b2olo estana l7ad ma el train yege
  pthread_cond_wait(&(station->trainInStation),&(station->lock));
  //el train gah Heeeee :party: :D

  // el passenger 7ayerkab el train we el passengers ely da5lo me bara
  // le gowa 7ayzedo be wa7ed
  station->passengersToEnter++;
  // el passengers el bara 7a2lo wa7ed
  station->passengersOut--;
  // 2ol le el train 2ny 5last we 23adt 3shan ya5od ely ba3di
  pthread_cond_broadcast(&(station->inTheTrain));
  //////////////////////////////////////////////////////////////////////////////
  pthread_mutex_unlock(&(station->lock));
  // Side note : m3rfesh 25er satren law etbadelo eh ely 7aye7sal bas 2zon mafesh
  //7aga 7at7sal 3shan el mutex lesa m3aya we el train lesa mestany 3leh fa howa
  // mesh 7ayt7ark men 3'er ma ana 23mel release aka signal.

}
/**
  Once the passenger is seated, it will call the function
  to let the train know that it’s on board.
*/
void station_on_board(struct station *station){

  pthread_mutex_lock(&(station->lock));
  /////////////////////////Critical Section/////////////////////////////////////
  // Note:
  // t2reban el function deh bt3mel check in kol wa7ed me el passengers 23ad makano
  // 3shan el mo3ed beya3mel loop fe el test bay3ed kam thread da5alt gowa el train
  // 3shan keda feh counter llenas ely 7atd5ol 3shan el loop el bara tt2kd
  station->passengersToEnter--;

  // Note :
  // 23taked mesh 7tefra2 low 3malt el condition dah bara wala gowa 3shan
  // el passengers wa2fen l2ed el train howa ely beynadi bas el train wa2ef 3shan
  // by3ed el nas ely 3ndo gowa 2bl ma yemshy
  if(!(station->passengersToEnter)){
    // 5lash 2otobes complete :'''D
    pthread_cond_signal(&(station->trainFull));
  }
  //////////////////////////////////////////////////////////////////////////////
  pthread_mutex_unlock(&(station->lock));
}
