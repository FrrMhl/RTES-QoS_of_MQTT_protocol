#include "main.h"

struct broker_t broker;

int main(){

    pthread_attr_t a;
    pthread_t thread_listener[N_LISTENER];
    pthread_t thread_sender[N_SENDER];
    pthread_t thread_broker;

    srand(555);
    pthread_attr_init(&a);

    init_broker(&broker);

    /*
        creation broker, listeners and senders;
    */
    pthread_create(&thread_broker, &a, broker_routine, (void *)NULL);

    for(int i=0; i<N_LISTENER; i++)
        pthread_create(&thread_listener[i], &a, listener_routine, (void *)(intptr_t)(i));

    for(int i=0; i<N_SENDER; i++)
        pthread_create(&thread_sender[i], &a, sender_routine, (void *)(intptr_t)(i));

    pthread_attr_destroy(&a);

    /*
        waitig the end of listeners, senders and broker;
    */
    for (int j=0; j<N_LISTENER; j++)
        pthread_join(thread_listener[j], NULL);
    
    for (int j=0; j<N_SENDER; j++)
        pthread_join(thread_sender[j], NULL);
    
    pthread_join(thread_broker, NULL);

    printf("\n\n          [ All senders and listeners are disconnected. ]\n");

    return 0;
}