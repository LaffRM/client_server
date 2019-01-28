#include <iostream>
#include <stdio.h>
#include "../../common/log/log.h"
using namespace std;

int main()
{

    printf("Start Log sample.\n");
    log a(65535 , false, "file", "");

    for(int i=0; i<20; i++)
    {
      char trap[1024]="abcdefghijklmnopqrs234234 34 123412341234 1234 2134 12341234 1324 234123412341234234 34 123412341234 1234 2134 12341234 1324 23412341234123";
        //trap[18] = char(i+48);
        a.write(trap);
    }

    a.writeb((void const*)"12345", 5,  "test: ");

    unsigned char buf[60];
    for(size_t i=0; i<60; i++) buf[i]=i+(i*2);
    
    for(int i=0; i<20; i++){
      buf[0]=i;
      char info[32];
      sprintf(info, "Buf[%d]: ", i);
      a.writeb(buf, 60, info);
    }
    return 0;
}
