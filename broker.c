#include "main.h"





void init_broker(struct broker_t *b){
    
    sem_init(&b->mutex_topic, 0, 1);
    sem_init(&b->message_is_arrived, 0, 0);
    sem_init(&b->waiting_end_broker, 0, 0);

    for(int i=0; i<N_TOPIC; i++){
        for(int j=0; j<N_LISTENER; j++){
            sem_init(&b->semaphore_subscriber[i][j], 0, 0);
            b->message_dup_broker[i][j] = -1;
            if(i == j % N_TOPIC)
                b->matrix_of_subscriber[i][j] = 1;
            else
                b->matrix_of_subscriber[i][j] = 0;
        }
    }

    b->message_dup = -1;
    b->message_qos = -1;
    b->topic = -1;
    b->puback = 0;
    b->puback_broker = 0;
    b->disconnected = 0;

    b->pubrec = 0;
    b->pubrel = 0;
    b->pubcomp = 0;
    b->pubrec_broker = 0;
    b->pubrel_broker = 0;
    b->pubcomp_broker = 0;

    b->comunication_id = 0;
}





/*
    broker manages one lister at time and so, the PUBACK is sent to the sender
    when all the listener are sent their PUBACK to the broker;

    after the broker receives the PUBACK from one listener, pass to the next;
*/
void broker_send_with_qos_one(int topic, struct broker_t *b){

    for(int i=0; i<N_LISTENER; i++){
        if(b->matrix_of_subscriber[topic][i]){

            b->message_dup_broker[topic][i] += 1;
            int time_before_next_sending = 0;

            printf("( Comunication %d ) Broker is sending a MESSAGE to the listener %d of TOPIC %d with DUP %d...\n", b->comunication_id, i, topic, b->message_dup_broker[topic][i]);

            sem_post(&b->semaphore_subscriber[topic][i]);

            while(1){
                if(b->puback_broker){
                    b->puback_broker = 0;
                    printf("( Comunication %d ) Broker is receiving a PUBACK...\n", b->comunication_id);
                    break;
                }
                if(time_before_next_sending == TIMEOUT){
                    b->message_dup_broker[topic][i] += 1;
                    printf("( Comunication %d ) TIMEOUT -> Broker is resending a MESSAGE with DUP = %d...\n", b->comunication_id, b->message_dup_broker[topic][i]);
                    time_before_next_sending = 0;
                }else
                    time_before_next_sending += 1;
            }
        }
    }

    printf("( Comunication %d ) All the listeners send their PUBACK...\n", b->comunication_id);
}





/*
    all the message are processed, also the duplicated one;
    this until sender receives PUBACK and then stop sending;

    the PUBACK is send at the end of the processing of one message by
    all listeners;
    the duplicate one are processed by listeners but sender do not 
    worry because also one PUBACK is important and not of the all duplicate; 
*/
void broker_receive_with_qos_one(int topic, struct broker_t *b){

    int only_one_ack_to_sender = 0;
    while(1){
        if(b->message_dup == -1)
            break;

        printf("( Comunication %d ) Broker is sending new MESSAGE to the listeners...\n", b->comunication_id);

        broker_send_with_qos_one(topic, b);

        if(only_one_ack_to_sender == 0){
            printf("( Comunication %d ) Broker is sending PUBACK to the sender...\n",b->comunication_id);
            only_one_ack_to_sender = 1;
            b->puback = 1;
        }

        b->message_dup -= 1;
    }
}





/*
    broker manages one lister at time;

    after the broker receives the PUBCOMP from one listener, pass to the next;
*/
void broker_send_with_qos_two(int topic, struct broker_t *b){
    
    for(int i=0; i<N_LISTENER; i++){
        if(b->matrix_of_subscriber[topic][i]){

            b->message_dup_broker[topic][i] += 1;
            int time_before_next_sending = 0;

            printf("( Comunication %d ) Broker is sending a MESSAGE to the listener %d of TOPIC %d with DUP %d...\n", b->comunication_id, i, topic, b->message_dup_broker[topic][i]);

            sem_post(&b->semaphore_subscriber[topic][i]);

            while(1){
                if(b->pubrec_broker){
                    printf("( Comunication %d ) Broker is receiving a PUBREC...\n", b->comunication_id);
                    b->message_dup_broker[topic][i] = 0;
                    b->pubrec_broker = 0;
                    break;
                }
                if(time_before_next_sending == TIMEOUT){
                    b->message_dup_broker[topic][i] += 1;
                    printf("( Comunication %d ) TIMEOUT -> Broker is resending a MESSAGE with DUP = %d...\n", b->comunication_id, b->message_dup_broker[topic][i]);
                    time_before_next_sending = 0;
                }else
                    time_before_next_sending += 1;
            }
        
            printf("( Comunication %d ) Broker is sending a PUBREL to the listener...\n", b->comunication_id);
            b->pubrel_broker = 1;

            while(1){
                if(b->pubcomp_broker){
                    printf("( Comunication %d ) Broker is receiving a PUBCOMP...\n", b->comunication_id);
                    b->pubcomp_broker = 0;
                    break;
                }
            }
        }

    }

    printf("( Comunication %d ) All the listeners send their PUBCOMP...\n", b->comunication_id);
}





/*
    broker process only one message without duplicates e sends message to 
    the subscribers only after saing to the sender that his message is arrived;
*/
void broker_receive_with_qos_two(int topic, struct broker_t *b){

    b->pubrec = 1;

    while(1){
        if(b->pubrel){
            printf("( Comunication %d ) Broker is receiving a PUBREL...\n", b->comunication_id);
            b->pubrel = 0;
            break;
        }
    }

    printf("( Comunication %d ) Broker is sending the ONLY ONE new MESSAGE to the listeners...\n", b->comunication_id);

    broker_send_with_qos_two(topic, b);

    printf("( Comunication %d ) Broker is sending PUBCOMP to the sender...\n", b->comunication_id);
    b->pubcomp = 1;
}





/*
    it is broker's responsability to wake interested listeners
*/
void *broker_routine(void *arg){

    extern struct broker_t broker; 
    printf("          [ Creating Broker ]\n");

    for(int i=0; i<N_ITER * N_SENDER; i++){

        sem_wait(&broker.message_is_arrived);
        printf("( Comunication %d ) Broker is receiving one message with QoS %d...\n", broker.comunication_id, broker.message_qos + 1);

        if(broker.message_qos == 0)
            broker_receive_with_qos_one(broker.topic, &broker);
        else
            broker_receive_with_qos_two(broker.topic, &broker);
        
        printf("Broker are waiting next sender...\n");
        sem_post(&broker.waiting_end_broker);
    }
    
    printf("          [ Broker comunicates to listeners that there are no more senders ]\n");

    broker.disconnected = 1;
    for(int i=0; i<N_TOPIC; i++)
        for(int j=0; j<N_LISTENER; j++)
            sem_post(&broker.semaphore_subscriber[i][j]);

    printf("          [ All the Senders are disconnected -> Ending Broker ]\n");
    return NULL;
}