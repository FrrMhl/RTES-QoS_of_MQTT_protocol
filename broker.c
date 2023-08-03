#include "main.h"

void init_broker(struct broker_t *b){
    
    sem_init(&b->mutex_topic, 0, 1);

    sem_init(&b->message_is_arrived, 0, 0);

    sem_init(&b->waiting_end_broker, 0, 0);
    
    b->message_qos = -1;
    b->topic = -1;
    b->message_dup = -1;

    for(int i=0; i<N_TOPIC; i++)
        for(int j=0; j<N_LISTENER; j++){
            sem_init(&b->semaphore_subscriber[i][j], 0, 0);
            if(j % N_TOPIC == i)
                b->matrix_of_subscriber[i][j] = 1;
            else
                b->matrix_of_subscriber[i][j] = 0;
        }
    
    b->puback = 0;
}

/*
    all the message are processed, also the duplicated one;
    this until sender receives PUBACK and then stop sending;
*/
void receive_with_qos_one(int topic, int qos, struct broker_t *b){
    while(1){
        if(b->message_dup == -1){
            b->puback = 0;
            break;
        }

        // elaborazione usando la stessa qos (il broker diventa il sender e il listener il broker)

        b->message_dup -= 1;
        b->puback = 1;
    }
}

void receive_with_qos_two(int topic, int qos, struct broker_t *b){

}

void *broker_routine(void *arg){

    extern struct broker_t broker; 
    printf("Creating Broker\n");

    for(int i=0; i<N_ITER * N_SENDER; i++){

        sem_wait(&(&broker)->message_is_arrived);

        if((&broker)->message_qos == 0)
            receive_with_qos_one((&broker)->topic, (&broker)->message_qos, &broker);
        else
            receive_with_qos_two((&broker)->topic, (&broker)->message_qos, &broker);
        
        sem_post(&(&broker)->waiting_end_broker);
    }
    
    return NULL;
}