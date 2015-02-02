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

#ifndef PARALLELPORT_H
#define PARALLELPORT_H


#include <iostream>
#include <sys/io.h> //for outp and inb
#include <unistd.h> //for ioperm
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <QTime>
#include <QDebug>


#define PORT_ADDR 0x378
#define MAJAKAS_VASAK 7
#define MAJAKAS_PAREM 6
#define OUT_VASAK 5
#define OUT_PAREM 4



#define set_bits(x,y) ((x) |= (y))
#define clear_bits(x,y) ((x) &= (~(y)))
#define set_bit(x,y) set_bits((x), (1 << (y)))
#define clear_bit(x,y) clear_bits((x), (1 << (y)))
#define bit_is_set(x,y) ((x) & (1 << (y)))
#define bit_is_clear(x,y) !bit_is_set((x), (y))



using namespace std;


class ParallelPort
{
public:
    ParallelPort();
    void go();


    inline  void lpt_init() {
        if (ioperm(PORT_ADDR, 3, 1)) {
            perror("ioperm"); exit(1);
        }
    }

    inline    void lpt_cleanup() {
        if (ioperm(PORT_ADDR, 3, 0)) {
            perror("ioperm"); exit(1);
        }
    }

    inline void out_binary(int s){
        int left;

                if(s <= 1) {
                        cout << s;

                        //cout.flush();
                        return;
                }

                left = s%2;
                out_binary(s >> 1);
                cout << left;
                //cout.flush();

    }


};

#endif // PARALLELPORT_H
