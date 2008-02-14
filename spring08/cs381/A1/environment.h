#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "agent.h"

class Environment {
  char ** env;
  size_t x; //environment x dimension
  size_t y; //environment y dimension
  size_t steps; //total steps/iterations so far
  Agent agent;
  bool  agent_bumped;
  size_t ax; //agent x
  size_t ay; //agent y
  size_t hx; //agent home x
  size_t hy; //agent home y
  Heading ah; //agent heading
  int score;
  size_t charge;
  bool done;
  bool init;

  public:
  Environment(char * filename);
  ~Environment();
  bool AtHome() const;
  void Print() const;
  bool Done() const;
  void Step();

};
#endif
