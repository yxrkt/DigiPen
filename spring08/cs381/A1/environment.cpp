#include <iostream>
#include <fstream>
#include <iomanip>
#include "environment.h"

//change agent(0) to agent(whatever_random_seed_you_want) - for debugging
Environment::Environment(char * filename) : x(0),y(0),steps(0),agent(0),agent_bumped(false),ax(0),ay(0),hx(0),hy(0),ah(EAST),score(0),charge(initial_charge),done(false),init(false) {
  //read filename
  std::fstream in(filename);
  if (in.good()) {
    in >> x;
    in >> y;
#ifdef ENVIRONMENT_DEBUG
    std::cout << "X=" << x << " Y=" << y << std::endl;
#endif
    env = new char*[y];
    for (size_t i=0;i<x;++i) {
      env[i] = new char[x];
    }
    try {
      for (size_t i=0;i<x;++i) {
	for (size_t j=0;j<y;++j) {
#ifdef ENVIRONMENT_DEBUG
	  std::cout << "reading " << i << " " << j << std::endl;
#endif
	  if (in.good()) in >> env[i][j];
	  else throw 1;
#ifdef ENVIRONMENT_DEBUG
	  std::cout << "got " << env[i][j] << std::endl;
#endif
	  switch (env[i][j]) {
	    case '>': ax=hx=i;ay=hy=j;  env[i][j]='.'; ah=EAST; break;
	    case '<': ax=hx=i;ay=hy=j;  env[i][j]='.'; ah=WEST; break;
	    case '^': ax=hx=i;ay=hy=j;  env[i][j]='.'; ah=NORTH; break;
	    case 'V': ax=hx=i;ay=hy=j;  env[i][j]='.'; ah=SOUTH; break;
	  }
	}
      }
      init=true;
    } catch (...) {
      std::cerr << "problem reading level\n";
      for (size_t i=0;i<x;++i) {
	delete [] env[i];
      }
      delete [] env;
    }
  }
}

Environment::~Environment() {
  if (init) {
    for (size_t i=0;i<x;++i) { delete [] env[i]; }
    delete [] env;
  }
}

bool Environment::AtHome() const {
  return (ax==hx && ay==hy);
}

void Environment::Print() const {
  const size_t width = 5;
  char prev_filler = std::cout.fill();  // save the original fill character
  std::cout << "============================\nAgent\nposition: " 
    << ax << " " << ay << "\nheading: ";
  switch (ah) {
    case  EAST : std::cout << ">";break;
    case  WEST : std::cout << "<";break;
    case  NORTH: std::cout << "^";break;
    case  SOUTH: std::cout << "V";break;
  }
  std::cout << "\nScore  " << score << std::endl;
  std::cout << "Charge " << charge << std::endl;
  std::cout << "Steps  " << steps << std::endl;
  if (init) {
    std::cout << std::endl << std::setw((width+1)*x) << std::setfill('-') << "" << std::endl << std::setfill(' ');
    for (size_t i=0;i<x;++i) {
      for (size_t j=0;j<y;++j) {
	size_t w=0;
	////////////////////////////////////////////////////////////
	if (i==ax && j==ay) {
	  ++w;
	  switch (ah) {
	    case  EAST : std::cout << ">";break;
	    case  WEST : std::cout << "<";break;
	    case  NORTH: std::cout << "^";break;
	    case  SOUTH: std::cout << "V";break;
	  }
	}
	////////////////////////////////////////////////////////////
	if (i==hx && j==hy) {
	  ++w;
	  std::cout << "H";
	}
	////////////////////////////////////////////////////////////
	std::cout << std::setw(width-w) << env[i][j] << "|";
      }
      std::cout << std::endl << std::setw((width+1)*x) << std::setfill('-') << "" << std::endl << std::setfill(' ');
    }
  }
  std::cout << std::setfill(prev_filler);
}

bool Environment::Done() const { return (!init || done);}

void Environment::Step() {
  if (Done()) return;

  if (++steps == max_steps) { 
    std::cout << "Agent timout: score " << score << std::endl;
    done=true;
    return;
  }
#ifdef EXTRA_CREDIT
  if (!--charge) {  //no charge for the first part
    std::cout << "Agent dead: score " << score << std::endl;
    done=true;
    return;
  }
  if ( AtHome() ) charge = initial_charge;
#endif
  --score; //step price

  Percept p;
  p.bump = agent_bumped;;
  p.home = AtHome();
  p.dirt = env[ax][ay]=='*';
  p.charge = charge;

  agent_bumped=false;

  Action a = agent.GetAction(p);
  std::cout << "Agent does ";
  switch (a) {
    case FORWARD: {
		    std::cout << "Forward\n";
		    int dx = ((ah+1)%2) * (ah-1);
		    int dy = (ah%2) * (2-ah);
		    if ( (!ax&&dx<0) || (!ay&&dy<0)  // ax+dx<0 won't work!
			|| ax+dx > x-1 || ay+dy>y-1
			|| env[ax+dx][ay+dy]=='#') { 
		      agent_bumped=true;
		    } else {
		      ax+=dx;
		      ay+=dy;
		    }
		  }
		  break;
    case LEFT:  ah=static_cast<Heading>( (ah+3)%4 ); 
		std::cout << "Left\n";
		break;
    case RIGHT: ah=static_cast<Heading>( (ah+1)%4 ); 
		std::cout << "Right\n";
		break;
    case SUCK: score += 20*(env[ax][ay]=='*'); env[ax][ay]='.'; 
	       std::cout << "Suck\n";
	       break;
    case SHUTOFF: score += 1000*p.home; done=true; 
		  std::cout << "Shutoff\n";
		  break;
  };

}
