#include "data_structure.h"
#include "listener.h"
#include "sender.h"

int main(){

    pthread_attr_t a;
    pthread_t thread_listener[N_LISTENER];
    pthread_t thread_sender[N_SENDER];

    srand(555);
    pthread_attr_init(&a);

    init_broker(&broker);

    /*
        creation listeners and senders
    */
    for(int i=0; i<N_LISTENER; i++)
        pthread_create(&thread_listener[i], &a, listener, (void *)(intptr_t)(i));

    for(int i=0; i<N_SENDER; i++)
        pthread_create(&thread_sender[i], &a, sender, (void *)(intptr_t)(i));

    pthread_attr_destroy(&a);

    /*
        waitig the end of listeners and senders
    */
    for (int j=0; j<N_LISTENER; j++)
        pthread_join(thread_listener[j], NULL);
    
    for (int j=0; j<N_SENDER; j++)
        pthread_join(thread_sender[j], NULL);

    printf("All senders and listeners are disconnected.\n");

    return 0;
}