#include "node.h"

#include <iostream>

/*type return :char which is the parity byte (8 bits)
 *parameters: the payload string of the message
 *use:calculating the parity bit
 *
 *
 * */
char Node:: errorDetectionSender(string payload)
{

    std:: bitset<8> initialxorbits(payload[0]);
    for(int i=1;i<payload.size();i++)
    {
        std::bitset<8> curr(payload[i]);
        initialxorbits=initialxorbits^curr;
    }
    return((char)initialxorbits.to_ulong());
}
/*type return :bool if an error is detected or not
 *parameters: the payload string of the message and the parity byte(trailer)
 *use:calculating the parity bit and return true or false depending on it
 *true if there is an error false otherwise
 *
 * */
bool Node::errorDetectionReciver(string payload, char trailer)
{
     std:: bitset<8> initialxorbits(payload[0]);
     for(int i=1;i<payload.size();i++)
       {
           std::bitset<8> curr(payload[i]);
           initialxorbits=initialxorbits^curr;
       }

     char intialxorchar=((char)initialxorbits.to_ulong());
     if(intialxorchar==trailer)
     {
         return false;
     }
     else
     {
         return true;
     }

}

/*/*type return :the modifyed string
 *parameters: the payload of the message and the bit to be modified
 *use:modify a given bit in the string
 *
 *
 *
 * */
string Node::modifyPayload(string msg,int &bit)
{
    vector <bitset<8>> msg_payload (msg.size());
       int m = int(uniform(0,msg.size()));
       int m2 = int(uniform(0,8));
       bitset<8> to_be_modfied(msg[m]);
       if(to_be_modfied[m2]==0)
       {
           to_be_modfied[m2]=1;
       }
       else
       {
           to_be_modfied[m2]=0;
       }

       msg[m]=(char)to_be_modfied.to_ulong();
       bit=m*8+m2;
       return msg;
}


