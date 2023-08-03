#include "main.h"

/*
    at each listener is assigned one particular topic in order to test different things;
*/
void *listener_routine(void *arg){
    int n_listener = (intptr_t)arg;
    int topic = n_listener % N_TOPIC;
    printf("Creating Listener %d from topic %d\n", n_listener + 1, topic  + 1);

    for(int i=0; i<N_ITER; i++){
        printf("Listener %d are listening...\n", n_listener + 1);
    }

    printf("Ending Listener %d\n", n_listener + 1);
    return NULL;
}