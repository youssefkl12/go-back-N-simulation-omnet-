#include "node.h"

#include <iostream>

Define_Module(Node);
void Node::initialize()
{
    // TODO - Generated method body
}

void Node::handleMessage(cMessage *msg)
{

  string recived_coord=msg->getName();

////////////////////////////Handling the time out of the frame ////////////////////////////////////
  if(msg->isSelfMessage())
  {
      handleTimeout();
      cout<<"timedout"<<endl;
  }
  ////////////////////////////////////////////////////////////////////////////////////////////////
//////////////this if condition is used so that the text file is read only once when the coordinator sends the initialization message
  if(!msg->isSelfMessage())
  {

      //check it is from the coordinator
      if(strcmp(msg->getSenderGate()->getName(),"out0")==0||strcmp(msg->getSenderGate()->getName(),"out1")==0){

          if(recived_coord[0]=='0'){
              fillBuffer("node0.txt", true);
              seq_c=(int)getParentModule()->par("WS").doubleValue();
          }
          if(recived_coord[0]=='1'){
              fillBuffer("node1.txt", true);
              seq_c=(int)getParentModule()->par("WS").doubleValue();
          }
      }
  }
/////////////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------//
  //we should either recive an ack or send a new msg or handle self msg(timeout)
   if( this->sender_node == true)
   {

      if(!msg->isSelfMessage())
       {
          if(strcmp(msg->getSenderGate()->getName(),"out")==0 )//3ashan lw gy mn elcoordinator mn3mlsh 7aga
          {
             MyMessage*message = check_and_cast<MyMessage*>(msg);
             if((message->getM_Frame_Type() == 1)) //if ack
             {

                 handleAck(message);
             }
          }
        }

      for (int i = sender_index; i <= end; i++)
      {
          sender_index++;
          double ms = simTime().dbl();
          double pt=getParentModule()->par("PT").doubleValue();
          time=time_frame-ms;
          if (time<0)
          {
              time=pt;
          }
          else
          {
              time+=pt;
          }
          //in case of error delay
          if(buffer[i].second == "0001" || buffer[i].second == "0011" || buffer[i].second == "1001" ||buffer[i].second == "1011")
          {
              time += getParentModule()->par("ED").doubleValue();
          }
          //in case off no loss p.s no timer is being put on
          if( buffer[i].second != "0100" && buffer[i].second != "0101" && buffer[i].second != "0110" && buffer[i].second != "0111" && buffer[i].second != "1100" && buffer[i].second != "1101" && buffer[i].second !="1110" && buffer[i].second !="1111")
          {
              if(check_buffer[i]==false)
              {
                if(buffer[i].second=="0001" || buffer[i].second=="0011" || buffer[i].second=="1001" ||buffer[i].second=="1011"  )
                {
                  outobj.open("output.txt",std::ios_base::app);
                  outobj<<"At ["<<simTime().dbl()+time-pt-getParentModule()->par("ED").doubleValue()<<"] "<<this->getName()<<" introducing channel error with code =["<<buffer[i].second<<"]"<<endl;
                  outobj.close();

                }

                else
                {
                  outobj.open("output.txt",std::ios_base::app);
                  outobj<<"At ["<<simTime().dbl()+time-pt<<"] "<<this->getName()<<" introducing channel error with code =["<<buffer[i].second<<"]"<<endl;
                  outobj.close();
                }
                check_buffer[i]=true;
              }
              cout<<"time2="<<time;
              sending((i%((int)getParentModule()->par("WS").doubleValue()+1)), buffer[i].first,time,buffer[i].second);//must be
          }

          //in case of loss bnzwd elPT bs msh bnzwd elTD
          if(buffer[i].second == "0100" || buffer[i].second == "0101" || buffer[i].second == "0110" || buffer[i].second == "0111" || buffer[i].second == "1100" || buffer[i].second == "1101" || buffer[i].second=="1111" || buffer[i].second=="1110" )
           {
              timer_start_Ack(time);
              if(check_buffer[i]==false)
              {
                  outobj.open("output.txt",std::ios_base::app);
                  outobj<<"At ["<<simTime().dbl()+time-pt<<"] "<<this->getName()<<" introducing channel error with code =["<<buffer[i].second<<"]"<<endl;
                  outobj.close();
                  check_buffer[i]=true;
              }
          }
          if (buffer[i].second=="1111" || buffer[i].second=="1110"|| buffer[i].second=="0111"|| buffer[i].second=="0110")
          {
              double error_time2 = time + getParentModule()->par("DD").doubleValue();
          }
          if(buffer[i].second == "0001" || buffer[i].second == "0011" || buffer[i].second == "1001" ||buffer[i].second == "1011")
           {
                time -= getParentModule()->par("ED").doubleValue();
           }
          //loss only
          if (buffer[i].second=="1100" ||buffer[i].second=="1101"||buffer[i].second=="0100" || buffer[i].second=="0101")
          {
             string frammedPayload=framing_function(buffer[i].first);
             outobj.close();
             print_output(-1,1,0,time,(i%((int)getParentModule()->par("WS").doubleValue()+1)),0,frammedPayload,0);
          }
          if (buffer[i].second=="0110" ||buffer[i].second=="0111"||buffer[i].second=="1110"||buffer[i].second=="1111")//duplication+loss
          {
             string frammedPayload=framing_function(buffer[i].first);
             outobj.close();
             print_output(-1,1,1,time,(i%((int)getParentModule()->par("WS").doubleValue()+1)),0,frammedPayload,0);
             outobj.close();
             print_output(-1,1,2,time+getParentModule()->par("DD").doubleValue(),(i%((int)getParentModule()->par("WS").doubleValue()+1)),0,frammedPayload,0);
          }
          time_frame=ms+time;
      }
    }
    else if(this->sender_node == false)//check  if we are in the sender node to send
    {
        recieving(msg);
    }
}
