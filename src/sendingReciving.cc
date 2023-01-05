#include "node.h"

#include <iostream>

/*return type:void
 *parameters:the recived message
 *use:
 *1-recive the message from the sender
 *2-detecting errors in the frame sent
 *3-detect if the frame expected in the one recived or not and to progress the expected or not
 *4-implementing the loss probablity of the ack
 *
 *
 *
 * */
void Node::recieving(cMessage *msg)
{
    MyMessage*message = check_and_cast<MyMessage*>(msg);
//    cout<<"payload at the reciever"<<message->getM_Payload()<<endl;
//    cout<<"the time at the payload is"<<simTime().dbl()<<endl;
    MyMessage*new_message = new MyMessage;
    bool error_detected= errorDetectionReciver(message->getM_Payload(),message->getM_Trailer());

    if(message->getM_header() != frame_expected_reciever){
        new_message->setM_Ack_Nack_NUM( (frame_expected_reciever + (int)getParentModule()->par("WS").doubleValue())%(1+(int)getParentModule()->par("WS").doubleValue()));
        new_message->setM_Frame_Type(1) ;
    }
    else if((message->getM_header() == frame_expected_reciever)&&error_detected==false)
    {
        new_message->setM_Ack_Nack_NUM( (frame_expected_reciever)%(1+(int)getParentModule()->par("WS").doubleValue()));
        frame_expected_reciever = (frame_expected_reciever + 1)%((int)getParentModule()->par("WS").doubleValue()+1);
        new_message->setM_Frame_Type(1) ;
    }
    else if((message->getM_header() == frame_expected_reciever)&&error_detected==true)
    {
        new_message->setM_Frame_Type(2) ;
        new_message->setM_Ack_Nack_NUM( (frame_expected_reciever)%(1+(int)getParentModule()->par("WS").doubleValue()));
    }

   // message->setM_Ack_Nack_NUM(message->getM_header());
    double pt = getParentModule()->par("PT").doubleValue();
    double td = getParentModule()->par("TD").doubleValue();
    int prob = uniform(0,100);
    if(prob > getParentModule()->par("LP").doubleValue()*100)
    {
        if(new_message->getM_Frame_Type()==2)
        {
            outobj.open("output.txt",std::ios_base::app);
            outobj<<"At time["<<simTime().dbl()+pt<<"] "<<this->getName()<<" sending[NACK] with number["<<new_message->getM_Ack_Nack_NUM()<<"], loss [No]"<<endl; //number da eh ? w loss eh brdo?
            outobj.close();
        }
        sendDelayed(new_message,pt+td,"out");
    }
    else
    {
        outobj.open("output.txt",std::ios_base::app);
        outobj<<"At time["<<simTime().dbl()+pt<<"] "<<this->getName()<<" sending[ACK] with number["<<new_message->getM_Ack_Nack_NUM()<<"], loss [Yes]"<<endl; //number da eh ? w loss eh brdo?
        outobj.close();
    }
    cout<<"probality="<<prob<<endl;
    cancelAndDelete(message);
}


/*return type:void
 *parameters:the sequence number of the message,the payload to send ,delay number(num),Error code(code)
 *uses:
 *1-prepare the message to send(geting it from the buffer,adding sequnce number ,calculating the parity bit,adding type)
 *2-modifing the payload in case we need to implement an error
 *3-according to each error code we will make modification to the message :
 *              0000 NO ERRORS
 *              1000,1001,1011,1010 Modification is common between them
 *              0001 DELAY
 *              0010 DUPLICATION
 *              0011 DUPLICATION AND DELAY
 *              1000 MODIFUCATION
 *              1001 MODIFY AND DELAY
 *              1010 MODIFY AND DUPLICATE
 *              1011 MODIFY DUPLICATION AND DELAY
 *
 *4-create a pointer to the message and push it to schedualed messages
 *
 * */


void Node::sending(int seq_num, string payload, double num,string code )
{
    int bit; //modfied bit
    double td=getParentModule()->par("TD").doubleValue();
    MyMessage * ourMsg= new MyMessage();
    ourMsg->setM_header(seq_num);
    string frammedPayload=framing_function(payload);
    ourMsg->setM_Payload(frammedPayload.c_str());
    string newPayload= ourMsg->getM_Payload();
    char trailer = errorDetectionSender(newPayload);
    ourMsg->setM_Trailer(trailer);
    ourMsg->setM_Frame_Type(0);
    string seq_num_string=to_string(seq_num);
    string modifiedpp=modifyPayload(newPayload,bit);

    if(code=="1000" || code=="1001" || code=="1011" || code=="1010") //modfication
    {
        ourMsg->setM_Payload(modifiedpp.c_str());
    }

    sendDelayed(ourMsg, num+td ,"out");
    std:: bitset<4> initialxorbits(trailer);
    if (code=="0000")//no error
    {
        outobj.close();
        print_output(-1,0,0,num,seq_num,initialxorbits,frammedPayload,0);
        //print_output(-1,0,0,num+simTime().dbl(),seq_num,initialxorbits,ourMsg->getM_Payload(),0);
    }
    else if(code=="0001")//error delay
    {
        outobj.close();
        print_output(-1,0,0,num-getParentModule()->par("ED").doubleValue(),seq_num,initialxorbits,ourMsg->getM_Payload(),getParentModule()->par("ED").doubleValue());
    }
    else if(code=="0010") //duplication hn3mel tany
    {
        outobj.close();
        print_output(-1,0,1,num,seq_num,initialxorbits,ourMsg->getM_Payload(),0);
    }
    else if(code=="0011") //duplication and error delay
    {
        outobj.close();
        print_output(-1,0,1,num-getParentModule()->par("ED").doubleValue(),seq_num,initialxorbits,ourMsg->getM_Payload(),getParentModule()->par("ED").doubleValue());
    }
    else if(code=="1000") //modfied
    {
         outobj.close();
         print_output(bit,0,0,num,seq_num,initialxorbits,ourMsg->getM_Payload(),0);
    }
    else if(code=="1001") //modfied and delay
    {
         outobj.close();
         print_output(bit,0,0,num-getParentModule()->par("ED").doubleValue(),seq_num,initialxorbits,ourMsg->getM_Payload(),getParentModule()->par("ED").doubleValue());
    }
    else if(code=="1010") //modfied and duplication
    {
        outobj.close();
        print_output(bit,0,1,num,seq_num,initialxorbits,ourMsg->getM_Payload(),0);
    }
    else if(code=="1011") //modfied and duplication and delay
    {
        outobj.close();
        print_output(bit,0,1,num-getParentModule()->par("ED").doubleValue(),seq_num,initialxorbits,ourMsg->getM_Payload(),getParentModule()->par("ED").doubleValue());
    }

    //--------------in case of duplication-----------------//
    if(code == "0010"  ) //duplication
    {
        MyMessage * new_ourMsg= new MyMessage();
        new_ourMsg->setM_header(seq_num);
        string frammedPayload=framing_function(payload);
        new_ourMsg->setM_Payload(frammedPayload.c_str());
        string newPayload = new_ourMsg->getM_Payload();
        char trailer = errorDetectionSender(newPayload);
        new_ourMsg->setM_Trailer(trailer);
        new_ourMsg->setM_Frame_Type(0) ;
        string seq_num_string=to_string(seq_num);
        sendDelayed(new_ourMsg,num+getParentModule()->par("DD").doubleValue()+td,"out");
        std:: bitset<4> initialxorbits(trailer);
        outobj.close();
        print_output(-1,0,2,num+getParentModule()->par("DD").doubleValue(),seq_num,initialxorbits,new_ourMsg->getM_Payload(),0);
    }

    else if(code =="1010")//duplication and modfication
    {
        MyMessage * new_ourMsg= new MyMessage();
        new_ourMsg->setM_header(seq_num);
        string frammedPayload=framing_function(payload);
        new_ourMsg->setM_Payload(modifiedpp.c_str());
        char trailer = errorDetectionSender(newPayload);
        new_ourMsg->setM_Trailer(trailer);
        new_ourMsg->setM_Frame_Type(0) ;
        string seq_num_string=to_string(seq_num);
        sendDelayed(new_ourMsg,num+getParentModule()->par("DD").doubleValue()+td,"out");
        std:: bitset<4> initialxorbits(trailer);
        outobj.close();
        print_output(bit,0,2,num+getParentModule()->par("DD").doubleValue(),seq_num,initialxorbits,new_ourMsg->getM_Payload(),0);
    }

    else if(code == "0011")// Duplication and error delay
    {
        MyMessage * new_ourMsg= new MyMessage();
        new_ourMsg->setM_header(seq_num);
        string frammedPayload=framing_function(payload);
        new_ourMsg->setM_Payload(frammedPayload.c_str());
        string newPayload = new_ourMsg->getM_Payload();
        char trailer = errorDetectionSender(newPayload);
        new_ourMsg->setM_Trailer(trailer);
        new_ourMsg->setM_Frame_Type(0);
        string seq_num_string=to_string(seq_num);
        sendDelayed(new_ourMsg,num+getParentModule()->par("DD").doubleValue()+td,"out");
        std:: bitset<4> initialxorbits(trailer);
        outobj.close();
        print_output(-1,0,2,num+getParentModule()->par("DD").doubleValue()-getParentModule()->par("ED").doubleValue(),seq_num,initialxorbits,new_ourMsg->getM_Payload(),getParentModule()->par("ED").doubleValue());
        //timer_start_Ack(num-getParentModule()->par("TD").doubleValue()-getParentModule()->par("ED").doubleValue());
    }
    else if(code =="1011" ) //Duplication and delay and modification
    {
        MyMessage * new_ourMsg= new MyMessage();
        new_ourMsg->setM_header(seq_num);
        string frammedPayload=framing_function(payload);
        new_ourMsg->setM_Payload(modifiedpp.c_str());

        char trailer = errorDetectionSender(newPayload);
        new_ourMsg->setM_Trailer(trailer);
        new_ourMsg->setM_Frame_Type(0) ;
        string seq_num_string=to_string(seq_num);
        sendDelayed(new_ourMsg,num+getParentModule()->par("DD").doubleValue()+td,"out");
        std:: bitset<4> initialxorbits(trailer);
        outobj.close();
        print_output(bit,0,2,num+getParentModule()->par("DD").doubleValue()-getParentModule()->par("ED").doubleValue(),seq_num,initialxorbits,new_ourMsg->getM_Payload(),getParentModule()->par("ED").doubleValue());
    }
    //-------------------------------------------------------//
    if(code == "0001" || code == "1001" || code =="1011" || code == "0011") //error delay
    {
        timer_start_Ack(num - getParentModule()->par("ED").doubleValue());
    }
    else
    {
        timer_start_Ack(num);
    }
    pair<MyMessage*,double> s_m;
    s_m.first = ourMsg;
    s_m.second = num+td+simTime().dbl();
    scheduled_msg.push_back(s_m);

}
