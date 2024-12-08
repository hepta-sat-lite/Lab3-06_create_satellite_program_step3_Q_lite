#include "mbed.h"
#include "LITE_CDH.h"
#include "LITE_EPS.h"
LITE_CDH cdh(PB_5, PB_4, PB_3, PA_8, "sd", PA_3);
LITE_EPS eps(PA_0,PA_4);
DigitalOut cond[]={PB_1, PA_5};
RawSerial gs(USBTX,USBRX,9600);
Timer sattime;
int rcmd = 0, cmdflag = 0; //command variable
 
//getting command and flag
void commandget()
{
    rcmd=gs.getc();
    cmdflag = 1;
}
//interrupting 
void receive(int rcmd, int cmdflag)
{
    gs.attach(commandget,Serial::RxIrq);
}
//initialzing
void initialize()
{
    rcmd = 0;
    cmdflag = 0;
    cond[0] = 0;
}

int main() {
    gs.printf("From Sat : Operation Start...\r\n");
    int flag = 0;     // condition
    float sattime=0.0,batvol,temp; //Voltage, Temerature
    receive(rcmd,cmdflag); //interupting by ground station command
    
    for(int i = 0; i < 100; i++) {
        //Sensing and Transmitting HK data
        eps.vol(&batvol);
        temp = 28.5;
        gs.printf("HEPTASAT::Condition = %d, Time = %f [s], BatVol = %.2f [V],Temp = %.2f [C]\r\n",flag,sattime,batvol,temp);
        //Condition
        cond[0] = 1;
        //Power Saving Mode
        if(batvol <= 3.5) {
            eps.shut_down_regulator();
            flag = 1;
        } else {
            eps.turn_on_regulator();
            flag = 0;
        }
        if(flag == 1) {
            gs.printf("Power saving mode ON\r\n"); 
        }
        //Contents of command
        if (cmdflag == 1) {
            if (rcmd == 'a') {
               for(int j=0;j<5;j++){
                gs.printf("HEPTASAT::Hello World!\r\n");
                cond[0] = 0;
                wait(1);
                cond[0] = 1;
               }
            }
            if (rcmd == 'b') {
                



                
            }
            initialize();
        }
        
        //Operation Interval
        wait(1.0);
        sattime = sattime+1.0;
    }
    gs.printf("From Sat : Operation Stop...\r\n");
}
           