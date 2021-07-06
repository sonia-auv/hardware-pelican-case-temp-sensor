/***
 * 
 * Example code: This code is a simple program that turn on/off a LED with a button while another LED flash.
 * 
 ***/

#include "main.h"

//RS485 declaration
RS485 rs(SLAVE_XXX);

//Declaration of threads
Thread thread;

//Insert all function thread here
void threadFunction()
{
  //Declaration, only put one command per thread
  while(1)
  {
    //rs.read, action & rs.write
  }
}
 
int main()
{
  //Declaration of all the thread (linked to one function)
  thread.start(threadFunction);
  thread.set_priority(osPriorityHigh);
}