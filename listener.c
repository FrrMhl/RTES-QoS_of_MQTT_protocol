#include "main.h"





void listener_receive_with_qos_one(int topic, int n_listener, struct broker_t *b){

    while(1){
        if(b->message_dup_broker[topic][n_listener] == -1)
            break;

        printf("Listener %d is receiving new MESSAGE from the broker...\n", n_listener);
        printf("Listener %d is sending PUBACK to the broker...\n", n_listener);

        b->message_dup_broker[topic][n_listener] -= 1;
        b->puback_broker = 1;
    }
}





void listener_receive_with_qos_two(int topic, int n_listener, struct broker_t *b){

    printf("Listener %d is receiving new MESSAGE from the broker...\n", n_listener);

    b->pubrec_broker = 1;

    while(1){
        if(b->pubrel_broker){
            printf("Listener is receiving a PUBREL...\n");
            b->pubrel_broker = 0;
            break;
        }
    }

    /*
        listeners send the ONLY ONE message to the application;
    */  
    
    printf("Listener %d is sending PUBCOMP to the broker...\n", n_listener);

    b->pubcomp_broker = 1;
}





/*
    at each listener is assigned one particular topic in order to test different things;
*/
void *listener_routine(void *arg){

    extern struct broker_t broker; 

    int n_listener = (intptr_t)arg;
    int topic = n_listener % N_TOPIC;
    printf("Creating Listener %d for topic %d...\n", n_listener, topic);

    while(1){
        sem_wait(&broker.semaphore_subscriber[topic][n_listener]);

        if(broker.disconnected)
            break;
        
        if(broker.message_qos == 0)
            listener_receive_with_qos_one(topic, n_listener, &broker);
        else
            listener_receive_with_qos_two(topic, n_listener, &broker);
    }

    printf("Ending Listener %d...\n", n_listener);
    return NULL;
}