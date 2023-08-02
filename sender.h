#ifndef SENDER_H_
#define SENDER_H_

void send_with_qos_one(int n_sender);
void send_with_qos_two(int n_sender);
void *sender(void *arg);

#endif