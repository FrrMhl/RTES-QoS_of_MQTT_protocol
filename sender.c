#include "sender.h"
#include "data_structure.h"

/*
    seders send to all topics and alternate different QoS
    even senders for QoS 1
    odd senders for QoS 2
*/
void send_with_qos_one(int n_sender){

}

void send_with_qos_two(int n_sender){
    
}

void *sender(void *arg){
    int n_sender = (intptr_t)arg;
    int topic = n_sender % N_TOPIC;
    printf("Creating Sender %d to topic %d\n", n_sender + 1, topic + 1);

    for(int i=0; i<N_ITER; i++){
        int QoS = i % 2;
        printf("Sender %d are sending with QoS %d...\n", n_sender + 1, QoS + 1);

        if(QoS == 0)
            send_with_qos_one(n_sender);
        else
            send_with_qos_two(n_sender);
    }

    printf("Ending Sender %d\n", n_sender + 1);
    return NULL;
}