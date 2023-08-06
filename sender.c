#include "main.h"





void sender_send_with_qos_one(int n_sender, int topic, struct broker_t *b, int iteration){
    
    int time_before_next_sending = 0;
    int qos = 0;

    sem_wait(&b->mutex_topic);

    /*
        send the initial message and if after a while we do not
        receive the PUBACK message, resend the message;  
    */
    sprintf(b->message, "Message from SENDER %d.", n_sender);
    b->communication_id = n_sender * (N_SENDER-1) + iteration; 
    b->message_dup = 0;
    b->message_qos = qos;
    b->topic = topic;
    printf("\n\n( Communication %d ) Sender %d is sending a MESSAGE in QoS = %d with DUP = %d...\n", b->communication_id, n_sender, qos + 1, b->message_dup);

    sem_post(&b->message_is_arrived);

    while(1){
        if(b->puback){
            printf("( Communication %d ) Sender %d is receiving a PUBACK...\n", b->communication_id, n_sender);
            b->puback = 0;
            break;
        }
        if(time_before_next_sending == TIMEOUT){
            b->message_dup += 1;
            printf("( Communication %d ) TIMEOUT -> Sender %d is resending a MESSAGE with DUP = %d...\n", b->communication_id, n_sender, b->message_dup);
            time_before_next_sending = 0;
        }else
            time_before_next_sending += 1;
    }

    printf("( Communication %d ) Sender %d finished is turn...\n\n\n", b->communication_id, n_sender);
    sem_wait(&b->waiting_end_broker);

    sem_post(&b->mutex_topic);
}





void sender_send_with_qos_two(int n_sender, int topic, struct broker_t *b, int iteration){

    int time_before_next_sending = 0;
    int qos = 1;

    sem_wait(&b->mutex_topic);

    /*
        send message and wait PUBREC; 
        if it don't arrived, sender resend the message and wait;
        when arrived PUBREC from broker, sender send PUBREL to inform the 
        broker to process the message;
        when broken terminates the processing (sending message to subscribers), sends
        PUBCOMP to the sender;

        with QoS 2, broker sends to the subscriber after saing to the sender that
        his message is arrived (in QoS 1 broker sends to the subscriber first and then saing
        to the sender and for this there can be duplicates);
    */
    sprintf(b->message, "Message from SENDER %d.", n_sender);
    b->communication_id = n_sender * (N_SENDER-1) + iteration; 
    b->message_dup = 0;
    b->message_qos = qos;
    b->topic = topic;
    printf("\n\n( Communication %d ) Sender %d is sending a MESSAGE in QoS = %d with DUP = %d...\n", b->communication_id, n_sender, qos + 1, b->message_dup);

    sem_post(&b->message_is_arrived);

    while(1){
        if(b->pubrec){
            printf("( Communication %d ) Sender %d is receiving a PUBREC...\n", b->communication_id, n_sender);
            b->pubrec = 0;
            b->message_dup = 0;
            break;
        }
        if(time_before_next_sending == TIMEOUT){
            b->message_dup += 1;
            printf("( Communication %d ) TIMEOUT -> Sender %d is resending a MESSAGE with DUP = %d...\n", b->communication_id, n_sender, b->message_dup);
            time_before_next_sending = 0;
        }else
            time_before_next_sending += 1;
    }

    printf("( Communication %d ) Sender %d is sending a PUBREL to the broker...\n", b->communication_id, n_sender);
    b->pubrel = 1;

    while(1){
        if(b->pubcomp){
            printf("( Communication %d ) Sender %d is receiving a PUBCOMP...\n", b->communication_id, n_sender);
            b->pubcomp = 0;
            break;
        }
    }

    printf("( Communication %d ) Sender %d finished is turn...\n\n\n", b->communication_id, n_sender);
    sem_wait(&b->waiting_end_broker);

    sem_post(&b->mutex_topic);
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

    printf("\n          [ Creating Sender %d for TOPIC %d ]\n", n_sender, topic);

    for(int i=0; i<N_ITER; i++){
        int QoS = i % 2;
        printf("\n          [ Sender %d is waiting to sending with QoS %d... ]\n\n", n_sender, QoS + 1);

        if(QoS == 0)
            sender_send_with_qos_one(n_sender, topic, &broker, i);
        else
            sender_send_with_qos_two(n_sender, topic, &broker, i);
    }

    printf("          [ Ending Sender %d ] \n", n_sender);
    return NULL;
}