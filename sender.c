#include "main.h"

void sender_send_with_qos_one(int n_sender, int topic, struct broker_t *b){
    
    int time_before_next_sending = 0;
    int qos = 0;

    sem_wait(&b->mutex_topic);

    /*
        send the initial message and if after a while we do not
        receive the PUBACK message, resend the message  
    */
    b->message_dup += 1;
    b->message_qos = qos;
    b->topic = topic;
    printf("Sender %d is sending a MESSAGE with DUP = %d...\n", n_sender, b->message_dup);

    sem_post(&b->message_is_arrived);

    while(1){
        if(time_before_next_sending == TIMEOUT){
            if(b->puback){
                printf("Sender %d is receiving a PUBACK...\n", n_sender);
                break;
            }else{
                b->message_dup += 1;
                printf("TIMEOUT...Sender %d is resending a MESSAGE with DUP = %d...\n", n_sender, b->message_dup);
                time_before_next_sending = 0;
            }
        }else
            time_before_next_sending += 1;
    }

    sem_wait(&b->waiting_end_broker);

    sem_post(&b->mutex_topic);
}

void sender_send_with_qos_two(int n_sender, int topic, struct broker_t *b){

}

/*
    each seder send to specific topic and alternate different QoS;
    -> even senders for QoS 1
    -> odd senders for QoS 2
*/
void *sender_routine(void *arg){
    
    extern struct broker_t broker; 
    int n_sender = (intptr_t)arg;
    int topic = n_sender % N_TOPIC;
    printf("Creating Sender %d for TOPIC %d...\n", n_sender, topic);

    for(int i=0; i<N_ITER; i++){
        int QoS = i % 2;
        printf("Sender %d are sending with QoS %d...\n", n_sender, QoS + 1);

        if(QoS == 0)
            sender_send_with_qos_one(n_sender, topic, &broker);
        else
            sender_send_with_qos_two(n_sender, topic, &broker);
    }

    printf("Ending Sender %d...\n", n_sender);
    return NULL;
}