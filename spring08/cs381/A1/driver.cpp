#include <iostream>
#include "definitions.h"
#include "agent.h"
#include "environment.h"


int main (int , char** argv) {
  Environment e(argv[1]);

  while (!e.Done()) {
    e.Print();
    e.Step();
  }
  e.Print();
}
