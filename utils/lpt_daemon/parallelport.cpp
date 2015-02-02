/**
 * @file parallelport.cpp
 * @brief Daemon program thats reads and writes LPT port to print debug information
 *
 *
 * @details Holds the abstract class AbstractRobot which all the robots inherit.
 *
 * Copyright: See COPYING file that comes with this distribution
 *
 * @author Margus Ernits <margus.ernits@itcollege.ee>, (C) 2011
 * thanks http://eagerfish.eu/lpt-programming-example-in-c-for-ubuntu-linux
 * thanks http://tldp.org/HOWTO/IO-Port-Programming.html
 */
#include "parallelport.h"
#include <sys/time.h>

ParallelPort::ParallelPort()
{

    lpt_init();


}

void ParallelPort::go(){
    int i=255;
    int val=0;
    QTime last_printed;
    int last_channel=10;
    int cur_channel=10;
    int signal_start=0;

    struct timeval start, end;

    long mtime, seconds, useconds;

    cout << "GO"<<endl;


	outb(255,PORT_ADDR);
	last_printed.start();
    while (true) {

        val=inb(PORT_ADDR);
        //cout << val << endl;
        if(bit_is_set(val,7)){
//            cout << "Bit7=1"<<endl;
            if(signal_start==0){
                signal_start=1;
                gettimeofday(&start, NULL);
            }
        }else{
  //          cout << "Bit7=0"<<endl;
            if (signal_start==1){
                gettimeofday(&end, NULL);
                seconds  = end.tv_sec  - start.tv_sec;
                useconds = end.tv_usec - start.tv_usec;
                mtime = ((seconds) * 10000 + useconds/100.0) + 0.5;
               // cout << "Signaali aeg: "<< mtime <<endl;
                signal_start=0;
		if (mtime>=189){
			cur_channel=1;		
		}else if(mtime >=186){
			cur_channel=2;			
		}else if(mtime>=183){
			cur_channel=3;
		}else if(mtime>=179){
			cur_channel=4;
		}else{
			cout<<"ERROR detecting channel"<<endl;
		}
		

		if(cur_channel!=last_channel){
			cout<<"Channel-: "<<cur_channel<<endl;
			last_channel=cur_channel;		
			last_printed.start();
		}else if(last_printed.elapsed()>1000){
			cout <<"Channel+: "<<cur_channel<<endl;
			last_printed.start();
		}
		

            }
        }

	


//        outb(inb(PORT_ADDR)| 0x11, PORT_ADDR);
//        i=inb(PORT_ADDR);
//        cout<<i<<endl;
//        outb(4|i,PORT_ADDR);

    }


    cout << "TESTIME makrosid" << endl;
    set_bit(i,0);
    clear_bit(i,4);
    out_binary(i);
    cout<<endl;
    if (bit_is_set(i,0)){
        cout<<"Bit 0 is set"<<endl;
    }else{
        cout<<"Bit 0 is not set"<<endl;
    }
    cout << endl<<"END" << endl;


 lpt_cleanup();
}
