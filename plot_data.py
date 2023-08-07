import matplotlib.pyplot as plt
import numpy as np

f_qos_one = open('results_QoS_1.txt', 'r')
f_qos_two = open('results_QoS_2.txt', 'r')

n_listeners = []
time_qos_1 = []
time_qos_2 = []

'''
    take also 1 element every 5 in order to make an understandable graph
'''
n_listeners = [int(x.split()[0]) for x in f_qos_one.readlines()]
n_listeners = [n_listeners[i] for i in range(0, len(n_listeners), 5)]

f_qos_one.seek(0)
time_qos_1 = [float(x.split()[1]) for x in f_qos_one.readlines()]
time_qos_1 = [time_qos_1[i] for i in range(0, len(time_qos_1), 5)]

time_qos_2 = [float(x.split()[1]) for x in f_qos_two.readlines()]
time_qos_2 = [time_qos_2[i] for i in range(0, len(time_qos_2), 5)]

plt.plot(n_listeners, time_qos_1, label='QoS 1')
plt.plot(n_listeners, time_qos_2, label='QoS 2')

plt.xlabel("Number of Listeners")
plt.ylabel("Average time among all Senders (s)")
plt.suptitle("Difference between duration of a connection with QoS 1 and with QoS 2.")
plt.legend()
plt.show()
