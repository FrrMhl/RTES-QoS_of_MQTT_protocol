#include "main.h"





/*
    listeners send also first PUBACK to the broker and meanwhile process the other duplicates
*/
void listener_receive_with_qos_one(int topic, int n_listener, struct broker_t *b){

    int only_one_ack_to_broker = 0;
    while(1){
        if(b->message_dup_broker[topic][n_listener] == -1)
            break;

        printf("( Communication %d ) Listener %d is receiving new MESSAGE from the broker...\n", b->communication_id, n_listener);
        printf("( Communication %d )  -> \'%s\'\n", b->communication_id, b->message);

        /*
            here listener comunicate with the underlaying applicaton
        */

        if(only_one_ack_to_broker == 0){
            printf("( Communication %d ) Listener %d is sending PUBACK to the broker...\n", b->communication_id, n_listener);
            only_one_ack_to_broker = 1;
            b->puback_broker = 1;
        }

        b->message_dup_broker[topic][n_listener] -= 1;
    }
}





void listener_receive_with_qos_two(int topic, int n_listener, struct broker_t *b){

    printf("( Communication %d ) Listener %d is receiving new MESSAGE from the broker...\n", b->communication_id, n_listener);
    printf("( Communication %d )  -> \'%s\'\n", b->communication_id, b->message);

    b->pubrec_broker = 1;

    while(1){
        if(b->pubrel_broker){
            printf("( Communication %d ) Listener is receiving a PUBREL...\n", b->communication_id);
            b->pubrel_broker = 0;
            break;
        }
    } 
    
    /*
        here listener comunicate with the underlaying applicaton
    */

    printf("( Communication %d ) Listener %d is sending PUBCOMP to the broker...\n", b->communication_id, n_listener);

    b->pubcomp_broker = 1;
}





/*
    at each listener is assigned one particular topic in order to test different things;
*/
void *listener_routine(void *arg){

    extern struct broker_t broker; 

    int n_listener = (intptr_t)arg;
    int topic = n_listener % N_TOPIC;
    printf("\n          [ Creating Listener %d for topic %d ]\n\n", n_listener, topic);

    while(1){
        sem_wait(&broker.semaphore_subscriber[topic][n_listener]);

        if(broker.disconnected)
            break;
        
        if(broker.message_qos == 0)
            listener_receive_with_qos_one(topic, n_listener, &broker);
        else
            listener_receive_with_qos_two(topic, n_listener, &broker);
    }

    printf("\n          [ Ending Listener %d ]\n\n", n_listener);
    return NULL;
}