#ifndef _NETWORKS_PRO_NODE_H
#define _NETWORKS_PRO_NODE_H

#include <omnetpp.h>
#include <bitset>
#include <vector>
#include <string>
#include<fstream>
#include"MyMessage_m.h"


using namespace omnetpp;
using namespace std;
/**
 * TODO - Generated class
 */
class Node : public cSimpleModule
{
private:
    bool sender_node = false;
   //this is a bool value that will help us determine if the current node we are in is a sender of a receiver
   //as we will set it with 0 for the receiver and 1 for the sender and use it for all if conditions
   vector<pair<string, string>>buffer;
   //this buffer will contain the messages for the sender and the receiver
   //in case of the sender we will push back 5 messages and keep pushing when we get an ack
   //in case of the receiver we will only push and pop one message
   int buffer_counter=0;
   int begin = 0;
   int end;
   int frame_expected_reciever = 0;
   int ack_expected_number=0;
   int sender_index = 0;
   double time_frame=0;
   double time=0;
   int seq_a=0;
   int seq_c;
   int counting = 0;
   vector<cMessage*> timeouts;
   vector<pair<MyMessage*,double>> scheduled_msg;
   vector <bool> check_buffer;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void sending(int seq_num, string payload,double num,string code);
    virtual string modifyPayload(string msg,int &bit);
    virtual void recieving(cMessage *msg);
    virtual void timer_start_Ack(double time_delay);
    virtual void fillBuffer(string txt_file, bool sender_bool);
    virtual void handleTimeout();
    virtual void handleAck(MyMessage*msg);
    virtual bool between(int a,int b , int c);
    virtual void print_output(int modfied,int lost,int duplicate,double time,int seq_num,bitset<4> initialxorbits,string frammedPayload,int errordelay);
    std::ofstream outobj;
    virtual string framing_function(string payload);
    virtual string deframing_function(string payload);
    virtual char errorDetectionSender(string payload);
    virtual bool errorDetectionReciver(string payload, char trailer);


};

#endif
