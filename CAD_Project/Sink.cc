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

#include "Sink.h"

Define_Module(Sink);

void Sink::initialize()
{
    lifetimeSignal = registerSignal("lifetime");
}

void Sink::handleMessage(cMessage *msg)
{
    simtime_t lifetime = simTime() - msg->getCreationTime();
    /*int NrUsers;

    std::vector<cOutVector> lifetimeVectors;
    NrUsers = par("gateSize").intValue();

    lifetimeVectors.resize(NrUsers);
    for (int i = 0; i < NrUsers; i++) {
        std::string vectorName = "User " + std::to_string(i) + " Lifetime";
        lifetimeVectors[i].setName(vectorName.c_str());
    }

    for(int i=0;i < NrUsers;i++){
        if (msg->arrivedOn("rxPackets",i)) {
            //lifetimeVectors[i].record(lifetime[i].dbl());
            //EV << "Recording lifetime for user " << i << ": " << lifetime[i] << endl;
        }
    }*/

      EV << "Received " << msg->getName() << ", lifetime: " << lifetime << "s" << endl;
      emit(lifetimeSignal, lifetime);
      delete msg;
}
