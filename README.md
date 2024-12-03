# 

Based on files from Doru Todincă - Computer Aided Design - AC - UPT

The simulation model consists of the following OMNeT++ modules:
1. A number of mobile users. In the first stages of the model you can implement two identical users, then you can consider a number of K users, organized as an array of users.
   
2. A scheduler. The scheduler reads the lengths of users' queues and implements a scheduling algorithm that allocates a number of radio channels to each user. Then it informs each users how many radio blocks to send during the current scheduling cycle. You can consider that the total number of radio channels is B, for example B=30 in LTE. The scheduler is activated every TB ms (a scheduling cycle). The value of the scheduling cycle is 1 ms in LTE.
   
3. A sink. The sink models the destination of the data. When the data packets created by an user arrive to the sink module, the sink simply deletes the OMNeT++ messages representing the data packets. Also, the sink is used to collect statistics about the simulation, statistics that can be for each user and for the entire system. This statistical information can be: the number of data packets that arrive to the sink, the mean, minimum and maximum delay of the data packets, etc.

A user consists of a source module (or generator) and a buffer module, implemented as a queue. The generator generates a number of data packets at certain time intervals. The data packets are stored in the buffer. When the scheduler informs the user that it can send a number of, e.g. m data packets, then the firsts m data packets from the buffer are send to the destination (the sink module).

Basic requirements:
Implement the simulation model described above using a fixed channel allocation scheduling algorithm, e.g., each user receives a fix number of radio chanmels (of RBs).

More advanced requirements
Implement other scheduling algorithms and compare their performance:
Implement the PF (proportional fair) channel dependent scheduling algorithm: you have to model (randomly) the quality of the radio channel r[i] for each user i. For each user it will be computed a parameter p[i], as a product of its radio link quality r[i] and the time elapsed since it was served last time. The users will be served in the decreasing order of their parameters p[i].
Control the network load and represent graphically the mean delay of the data packets of the users as a function of the network load, e.g. for network load of 30%, 50%, 70% and 90%.
Compare the performance of the users (the mean delay here) when the users have different weights W[i]. For example, you have two high priority (HP) users with W[i]=6 or 8, 5 medium priority users (MP) with W[i]=2 and 3 low priority users (LP) with W[i]=1.

