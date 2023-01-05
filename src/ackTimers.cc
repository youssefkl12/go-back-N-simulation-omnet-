#include "node.h"

#include <iostream>
/*Return Type:void
 *paramters:the time we want to the timer to trigger from the sending time of the message
 *use:it is used to start the ack timer which will cause a time out if it was not recived
 * */
void Node::timer_start_Ack(double time_delay){
   cMessage* timer = new cMessage();
    scheduleAt(simTime() + getParentModule()->par("TO").doubleValue()+time_delay, timer);
    timeouts.push_back(timer);
}


/*
 *return type:void
 *parameters:none
 *use:after we recive the self message of the timeout will start deleting the schedulaed messges by sendDelayed
 *and the other timer that havent arrived yet.
 *They are deleted through there vectors members (schedled_msg and timeouts)
 * */
void Node::handleTimeout()
{
    int seqnum=(begin%((int)getParentModule()->par("WS").doubleValue()+1));;



    while(!scheduled_msg.empty())
    {
        cMessage* temp_msg = scheduled_msg[0].first;

        if(scheduled_msg[0].second > simTime().dbl())
        {
            seqnum=scheduled_msg[0].first->getM_header();

            cancelAndDelete(temp_msg);
        }
        scheduled_msg.erase(scheduled_msg.begin());
        //scheduled_msg_time.erase(scheduled_msg_time.begin());

    }
    while(!timeouts.empty())
    {
        cMessage* temp_msg = timeouts[0];
        cancelAndDelete(temp_msg);
        timeouts.erase(timeouts.begin());

    }
    outobj.open("output.txt",std::ios_base::app);
    outobj<<"timeout event at time["<<simTime().dbl()<<"] "<<this->getName()<<"for frame with seq_num=["<<seqnum<<"]"<<endl;
    sender_index = begin;
    buffer[begin].second = "0000";
}




/*return Type:bool
 *parameters:a(ack expected),b(frame to be compared),c(next frame to send)
 *use:for detecting if the ack recived is within the sending window
 *
 *
 * */


bool Node::between(int a,int b , int c)
{
    if(((a<=b)&&(b<c))||((c<a)&&(a<=b))||((b<c)&&(c<a)))
    {
        return true;
    }
    return false;
}





/*return Type:void
 *paramters:the message that we recived the ack in
 *use:
 *1-checking if the ack recived is in the window
 *2-implementung accumilative ack
 *3-progressing the sender window
 *4-updating the a b c parameters
 * */

void Node::handleAck(MyMessage*msg)
{
   if(between(seq_a,msg->getM_Ack_Nack_NUM(),seq_c))
   {
       //cout<<"sender index after ack"<<msg->getM_Ack_Nack_NUM()<<endl;
       while(!scheduled_msg.empty())
       {
           if((begin%((int)getParentModule()->par("WS").doubleValue()+1) != msg->getM_Ack_Nack_NUM()))
           {
               cMessage* temp_msg_1 = timeouts[0];//msh mot2kda hysht8l wala l2
               cMessage* temp_msg_2 = scheduled_msg[0].first;
               timeouts.erase(timeouts.begin());
               scheduled_msg.erase(scheduled_msg.begin());
               cancelAndDelete(temp_msg_1);
               //scheduled_msg_time.erase(scheduled_msg_time.begin());
               begin += 1;
               if(end+1 != buffer.size())
               {
                  end += 1;
               }
           }
           else
           {
               break;
           }

       }

      if(strcmp(this->getName(),"node0")==0)
      {
      outobj.open("output.txt",std::ios_base::app);
      outobj<<"At time["<<simTime().dbl()-getParentModule()->par("TD").doubleValue()<<"] node1 sending[ACK] with number["<<msg->getM_Ack_Nack_NUM()<<"], loss [No]"<<endl; //number da eh ? w loss eh brdo?
      outobj.close();
      }
      else
      {
          outobj.open("output.txt",std::ios_base::app);
          outobj<<"At time["<<simTime().dbl()-getParentModule()->par("TD").doubleValue()<<"] node0 sending[ACK] with number["<<msg->getM_Ack_Nack_NUM()<<"], loss [No]"<<endl; //number da eh ? w loss eh brdo?
          outobj.close();
      }

      cMessage* temp_msg_1 = timeouts[0];//msh mot2kda hysht8l wala l2
      cMessage* temp_msg_2 = scheduled_msg[0].first;
      timeouts.erase(timeouts.begin());
      scheduled_msg.erase(scheduled_msg.begin());
      cancelAndDelete(temp_msg_1);
      cancelAndDelete(msg);
      //scheduled_msg_time.erase(scheduled_msg_time.begin());
      begin += 1;
      if(end + 1 != buffer.size())
      {
         end += 1;
      }
      seq_a=(begin%((int)getParentModule()->par("WS").doubleValue()+1));
      seq_c=((end+1)%((int)getParentModule()->par("WS").doubleValue()+1));
   }
}

