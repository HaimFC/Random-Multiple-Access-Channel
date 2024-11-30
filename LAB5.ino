#include <C:\Users\gns_user\Desktop\LAB5\EthernetLab.h>
//------------------------------------------Defines------------------------------------
#define newData "Shahaf&Haim"                       //Data in string       ||-------------------------------------------------------------||
//#define SIZE 8                                  //Data size            || Able to change any data size between 0-12 and any duty cycle|| 
//#define BIT_TIME 10                                //duty cycle in millis ||-------------------------------------------------------------||
#define MOD 0
#define prog 300
//--------------------------------------------------------------------------------------
char DATA[20] = newData;
char Ack_frame[2];
int IndexFrame = 0;
unsigned long timer;
int random_time;
unsigned long currentTX;
unsigned long RTT_time;
unsigned long RTT=2*prog+strlen(DATA)+4;
unsigned long sendFrameTime;
unsigned long trynewshit;
int sn=0;
int rn;
int type;
char data2send[100];                                //frame we sending 
int index=1;                                        //index tell us if the status in collition or yos the bus is byse 
long number_random=16;
//-------------------------------------flag-------------------------------------------
bool sending_Data = true;
bool start_send_Data = false;
bool reqest_recive = false;
bool flag_mod_col = true;
bool flag_mod_busy = true;
bool flag_wait = false;
bool one_time = true;
bool rnd_flag = false;

//-------------------------------------Set up-------------------------------------------
void setup(){
    setMode(0);                                      //CRC no mistakes mode - TX MODE
    Serial.begin(115200);                            //BAUD_RATE = 115200 
    setAddress(1,20);
}
void loop() {
    layer_tx();                                      //TX Function - create frames
    layer_rx();                                      //RX Function - Check ACK 
}
//------------------------------------LAYER  TX----------------------------------------
void  layer_tx(){     
    if(!sending_Data){
      return;}
      if(one_time){
        data2send[0]='0';                              //type block 
        data2send[1] = '0';                            //sn block
        data2send[2] = '0';                            //2 byte length block
        data2send[3] = strlen(DATA);                   //2 byte length block
        strcat(data2send,DATA);
        //Serial.println("-----------");   
        //Serial.println(data2send);
        //Serial.println((int)data2send[3]);
        //Serial.println("-----------");
        one_time = false;
        } 
      if(checkLine()&&flag_mod_col&&flag_mod_busy){                        //If the bus is free to start sending data and if bus is busy wait 500 millis
        startPackage((void*)data2send,data2send[3]);
        if(!start_send_Data)
        {
          start_send_Data = true;
          sendFrameTime = millis();
        }
        long timer = millis();                    //time since arduino turned on
        if (timer - sendFrameTime >= prog)     //waitingTime is passed
        {
             index =1;                                  //Initial initialization mode
             number_random =16;                         //2^3+1 
             start_send_Data = false;                   //Finish send and the status now is not between the package      
             endPackage(1);                             //END sending package 
             sending_Data = false;                      //Stop sending 
             reqest_recive = true;                      //Waiting for Ack
             RTT_time =millis();                        //Start timer for RTT 300 mil seconde 
             
             return;
        }
        else                                      //time didnt pass yet, return false
        {
          if(!checkLine())
          {
            endPackage(0);                                                //stops the frame mid transmission (in case of a collision).                                             
            flag_mod_col = false;                                             //stop sending  and wait now
            random_time=random(number_random-1);                         //Uniform distribution
            trynewshit = millis();
            Serial.println("Random Number Changed");

          }
        }
      }
      else{
        if(!flag_mod_col)
        {
            
            if(millis() - trynewshit >= random_time)                                    //wait random time befor we start agin sending
            {
                flag_mod_col = true;                                         //return to send 
                index++;
                if(index<7)                                                   //index start from 1 
                   number_random =number_random*2;
                Serial.println("*****************");
                Serial.println("Collision on the line");
                Serial.print("Statistic random Number:"); 
                Serial.println(random_time); 
                Serial.println("*****************"); 
                
            }
            else
                 return;
            
        }
        else
        {   
             
            if(MOD==1){                                                   // one-presisten
              Serial.println("The line is busy before we start sending");    
              return; 
            }                                  
            else{                                                         // non-presisten
              if(!rnd_flag)
              {
                rnd_flag = true;
                random_time=random(number_random-1);                     //Uniform distribution
              }
              flag_mod_busy = false; 
              if(time_wait(random_time))                                  //wait random time befor we start agin sending
              {
                flag_mod_busy = true;                                         //return to send 
                rnd_flag = false;
                if(index<7)                                                   //index start from 1 
                  number_random =number_random*2;
                Serial.println("*****************");
                Serial.println("The line is busy after we start sending");  
                Serial.print("Statistic random Number:");  
                Serial.println(random_time); 
                Serial.println("*****************"); 
                index++; 
              }
              else
                  return;
          }
      }
  }
}

//------------------------------------LAYER  RX----------------------------------------
void layer_rx(){
      if(!reqest_recive){
        return;}
      if(readPackage(Ack_frame, 2)){                                    //read Ack 
              type = Ack_frame[0];                                      //block type which 1 ->mening Ack frame
              rn = Ack_frame[1];                                        //rn 
              //Serial.println("------print status(type/rn/sn)------");
              //Serial.println(type);
              //Serial.println(rn);
              //Serial.println(sn);                                       //Number frame 
              //Serial.println("------------------------------------");
              if(rn==(sn+1)%2){
                  sn++;                                                 //increas sn/the number frame 
                  data2send[1] =(sn)%2;
                  Serial.println("We got good Ack");
                  //RTT = RTT*((float)(sn-1)/sn)+((float)1/sn)*(millis()-RTT_time);
              }
              else
                  Serial.println("We got bad Ack"); 
             reqest_recive = false;
             sending_Data = true;
          }
              
      else if(RTT<=millis() - RTT_time){             //If you are in the field you can read from the line)
          reqest_recive = false;  
          sending_Data = true;
          Serial.println("rime out"); 
    }
      else{                                          //start sending from the beginning
        return;
     }
     return;
}



bool time_wait(long waitingTime)            //Time function for tx Function
{
  long timer = millis();                    //time since arduino turned on
  if (timer - currentTX >= waitingTime)     //waitingTime is passed
  {
      currentTX = millis();                 //save current time
      return true;                          //return the time is passed
  }
  else                                      //time didnt pass yet, return false
      return false;                 
}
