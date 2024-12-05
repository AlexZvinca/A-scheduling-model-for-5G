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
    NrOfChannels = 5;//read from omnetpp.ini
    selfMsg = new cMessage("selfMsg");
    for(int i=0; i<10;i++){
           q[i]=0;
           NrBlocks[i]=0;
    }
    scheduleAt(simTime(), selfMsg);


}

void Scheduler::handleMessage(cMessage *msg)
{
   // int q[NrUsers];
 //   int NrBlocks[Nrusers];

    /*
    for(int j=0;j<NrUsers; j++){
        q[j]= getParentModule()->getSubmodule("user",j)->getSubmodule("myqq")->par("qlp");
        EV << "q["<<j<<"]= " << q[j] <<endl;
    }
     */


   // int userWeights[NrUsers];
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

        while (remainingBlocks > 0) {
            bool anyAllocationMade = false;

            for (int i = 0; i < NrUsers; i++) {
                // Only allocate blocks to users with a non-zero queue length
                if (q[i] > 0 && remainingBlocks > 0) {
                    NrBlocks[i]++;
                    remainingBlocks--;
                    anyAllocationMade = true;
                }
            }

            // Exit the loop if no allocation was made (i.e., all users' queues are empty)
            if (!anyAllocationMade) {
                break;
            }
        }

        for(int i=0;i < NrUsers;i++){
            if(NrBlocks[i] > 0){
                cMessage *cmd = new cMessage("cmd");
                cmd->addPar("nrBlocks");
                cmd->par("nrBlocks").setLongValue(NrBlocks[i]);
                send(cmd,"txScheduling",i);
                EV << "Allocated " << NrBlocks[i] << " blocks to user " << i << endl;
            }
        }

        scheduleAt(simTime()+par("schedulingPeriod").doubleValue(), selfMsg);
    }
}
