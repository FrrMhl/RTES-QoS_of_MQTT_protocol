#include "data_structure.h"

struct broker_t{
    sem_t mutex_topic[N_TOPIC];

} broker;

void init_broker(struct broker_t *b){
    for(int i=0; i<N_TOPIC; i++)
        sem_init(&b->mutex_topic[i], 0, 1);

}