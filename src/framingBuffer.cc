#include "node.h"

#include <iostream>


/*return type:payload after framing
 *parameters:the original payload
 *use:it is used to frame the message payload
 * */
string Node:: framing_function(string payload){
    string frammed_payload;

    for(int i=0;i<payload.size();i++){
        if(payload[i]=='$'||payload[i]=='/'){
            frammed_payload.append("/");
            frammed_payload=frammed_payload+payload[i];
        }else{

            frammed_payload=frammed_payload+payload[i];
        }
}
frammed_payload.insert(0, "$");
frammed_payload.append("$");
return frammed_payload;
}



/*
 *return type:payload after deframing
 *parameters:the framed by load
 *use:it is used to deframe the message payload
 *
 *NOTE:we didnt need to use it
 *
 * */

string Node::deframing_function(string payload){


    string deframmed_payload;
    for(int i=1;i<payload.size()-1;i++){

        if(payload[i]=='/'){
            i++;
            deframmed_payload=deframmed_payload+payload[i];
        }
        else
        {
            deframmed_payload=deframmed_payload+payload[i];
        }
}

return deframmed_payload;

}

/*return type:void
 *paramters:the name of the text file we will be reading
 *use:to read the text file and fill the buffer with the lines ,each index has a line
 *the error bits and the actuall message are stored on pairs
 * */


void Node::fillBuffer(string txt_file, bool sender_bool)
{
    this->sender_node= sender_bool;
    string line;
    int i=0;
    ifstream myfile(txt_file);
    const char* text_file_char = txt_file.c_str();
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            string error_bits = "";
            error_bits+=line[0];
            error_bits+=line[1];
            error_bits+=line[2];
            error_bits+=line[3];
            line.erase(0, 5);
            pair<string, string> data_txt;
            data_txt.first = line;
            data_txt.second = error_bits;
            this->buffer.push_back(data_txt);
            i++;
            check_buffer.push_back(false);
        }
        myfile.close();
        if(i < getParentModule()->par("WS").doubleValue())
        {
            end = buffer.size() - 1;
        }
        else
        {
            end = getParentModule()->par("WS").doubleValue()-1;
        }
     }
     else
     {
         throw cRuntimeError("Error opening file '%s'?", text_file_char);
     }
}


