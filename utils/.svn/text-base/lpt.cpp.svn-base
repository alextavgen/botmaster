#include <iostream>
#include <sys/io.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


using namespace std;


void lpt_init() {
  if (ioperm(0x378, 3, 1)) {
    perror("ioperm"); exit(1);
  }
}

void lpt_cleanup() {
  if (ioperm(0x378, 3, 0)) {
    perror("ioperm"); exit(1);
  }
}




int main(int argn, char ** argv){
int i=0;
lpt_init();

while(1){
outb(1, 0x378);
sleep(1);
i=inb(0x378);
cout<<i<<endl;
outb(i+1,0x378);
sleep(1);
}
lpt_cleanup();

return 0;

}
