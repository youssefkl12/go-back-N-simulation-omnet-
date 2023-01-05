#include "node.h"

#include <iostream>

/*return:void
 *parameters:modified bit number,lost or not,duplication number,message time (num),sequnce number,trailer of the message,payload,delay
 *use:to print the output file recurring line
 *
 * */


void Node:: print_output(int modfied,int lost,int duplicate,double num,int seq_num,bitset<4> initialxorbits,string frammedPayload,int delay)
{
    string lost_ch;
    if (lost==0)
    {
        lost_ch="No";
    }
    else if(lost==1)
    {
        lost_ch="yes";
    }
    outobj.open("output.txt",std::ios_base::app);
    outobj<<"At time["<<num+simTime().dbl()<<"] "<<this->getName()<<"[sent] frame with seq number["<<seq_num<<"] and payload ["<<frammedPayload<<"] and trailer ["<<initialxorbits<<"] modfied["<<modfied<<"] ,lost["<<lost_ch<<"], duplicate["<<duplicate<<"],delay ["<<delay<<"]" <<endl;
    outobj.close();
}
