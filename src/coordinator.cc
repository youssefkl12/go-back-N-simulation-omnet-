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

#include "coordinator.h"
#include <iostream>
#include<fstream>
#include<string>
using namespace omnetpp;
using namespace std;
Define_Module(Coordinator);

void Coordinator::initialize()
{
    // TODO - Generated method body
    // TODO - Generated method body
       std::string input_num;
       std::string line;
       std:: ifstream myfile("text.txt");
       std::cout << "entering file in cordinator" << '\n';
       if (myfile.is_open())
       {
           while (getline(myfile, line))
           {
               input_num=line;
               std::cout << "file is open" << '\n';
               std::cout << input_num[0] << input_num[2]<< '\n';

           }
           myfile.close();
       }else{
           throw cRuntimeError("Error opening file '%s'?", "text.txt");
       }
       cMessage *msg = new cMessage();

       int start_time = std::atoi(&input_num[2]);
       std::cout << input_num[2] << "is the start time"<< '\n';
       //----------THERE WILL BE AN ERROR MUST BE ADJUSTED-----------//

       msg->setName(input_num.c_str());
       scheduleAt(start_time, msg);

       //-----------------------------------------------------------//
}

void Coordinator::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    string recived_coord=msg->getName();
    if(recived_coord[0]=='0'){
              send(msg,"out0");
          }else{
              send(msg,"out1");
          }
}
