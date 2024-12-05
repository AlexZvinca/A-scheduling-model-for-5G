//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Scheduler.h"

Define_Module(Scheduler);



Scheduler::Scheduler()
{
    selfMsg = nullptr;
}

Scheduler::~Scheduler()
{
    cancelAndDelete(selfMsg);
}


void Scheduler::initialize()
{
    NrUsers = par("gateSize").intValue();
    NrOfChannels = 14;//read from omnetpp.ini
    selfMsg = new cMessage("selfMsg");
    for(int i=0; i<NrUsers;i++){
           q[i]=0;
           NrBlocks[i]=0;
           weights[i] = i+1;
           radioQuality[i] = uniform(1, 10);
           lastServedTime[i] = simTime();

           EV << "Initialize user: " << i <<" weight: " << weights[i]
                   << " Radio Quality: " << radioQuality[i] << endl;
    }
    scheduleAt(simTime(), selfMsg);


}

void Scheduler::handleMessage(cMessage *msg)
{

    for(int i=0;i < NrUsers;i++){
        if (msg->arrivedOn("rxInfo", i)){
            q[i]= msg->par("ql_info");
            EV << "Update: q["<<i<<"]= " << q[i] <<endl;
            delete(msg);
        }
    }

    if (msg == selfMsg){

        memset(NrBlocks, 0, sizeof(NrBlocks));
        int totalBlocks = NrOfChannels;
        int remainingBlocks = totalBlocks;
        double priorities[NrUsers];
        double totalPriority = 0.0;

        for(int i=0; i<10;i++){
            radioQuality[i] = uniform(1, 10);
        }

        //round robin in numerical order
        /*while (remainingBlocks > 0) {
            bool anyAllocationMade = false;

            for (int i = 0; i < NrUsers; i++) {
                // Only allocate blocks to users with a non-zero queue length
                if (q[i] > 0 && remainingBlocks > 0) {
                    NrBlocks[i]++;
                    remainingBlocks--;
                    anyAllocationMade = true;

                    lastServedTime[i] = simTime();
                }
            }

            // Exit the loop if no allocation was made (i.e., all users' queues are empty)
            if (!anyAllocationMade) {
                break;
            }
        }*/


        for (int i = 0; i < NrUsers; i++) {

                   //OS
                   //priorities[i] = radioQuality[i];

                   //RR - time priority
                   //priorities[i] = simTime().dbl() - lastServedTime[i].dbl();

                   //WRR
                   //priorities[i] = weights[i] * (simTime().dbl() - lastServedTime[i].dbl());

                   //PF
                   priorities[i] = radioQuality[i] * weights[i] * (simTime().dbl() - lastServedTime[i].dbl());

                   //LQ
                   //priorities[i] = q[i] * weights[i];

                   totalPriority += priorities[i];
               }

        for (int i = 0; i < NrUsers; i++) {
                if (remainingBlocks > 0 && q[i] > 0) {
                    // Proportional allocation
                    int allocatedBlocks = std::round((priorities[i] / totalPriority) * remainingBlocks);
                    allocatedBlocks = std::min(allocatedBlocks, q[i]); // Do not allocate more than available in queue
                    allocatedBlocks = std::min(allocatedBlocks, remainingBlocks); // Do not exceed remaining blocks

                    NrBlocks[i] = allocatedBlocks;
                    remainingBlocks -= allocatedBlocks;

                    // Update last served time if blocks were allocated
                    if (NrBlocks[i] > 0) {
                        lastServedTime[i] = simTime();
                    }
                }
            }

        //distribute remaining blocks
        for (int i = 0; i < NrUsers && remainingBlocks > 0; i++) {
               if (q[i] > NrBlocks[i]) { // Only assign remaining blocks to users with unfulfilled demand
                   NrBlocks[i]++;
                   remainingBlocks--;
               }
        }




        for(int i=0;i < NrUsers;i++){
            if(NrBlocks[i] > 0){
                cMessage *cmd = new cMessage("cmd");
                cmd->addPar("nrBlocks");
                cmd->par("nrBlocks").setLongValue(NrBlocks[i]);
                send(cmd,"txScheduling",i);
                EV << "Allocated " << NrBlocks[i] << " blocks to user " << i
                                   << " (Priority: " << priorities[i] << ")" << endl;
            }
        }

        scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);
    }
}
