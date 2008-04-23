#include "bn.h"
#include <ctime>
#include <string>

//this is the network which was used to generate training-data-7
int main() {
	BayesianNetwork bn(7);
	enum {a,b,c,d,e,f,g};

	bn.AddLink(a,c);
	bn.AddLink(b,c);
	bn.AddLink(d,e);
	bn.AddLink(c,g);
	bn.AddLink(c,f);
	bn.AddLink(e,f);

	bn.AddCPT(a, 0.2);
	bn.AddCPT(b, 0.5);
	bn.AddCPT(d, 0.7);

	bn.AddCPT(c, 0.1, 0.3, 0.5, 0.7 );
	bn.AddCPT(e, 0.3,0.7);
	bn.AddCPT(g, 0.8,0.2);
	bn.AddCPT(f, 0.9, 0.7, 0.5, 0.1 );

	bn.SeedRNG( static_cast<unsigned int>(std::time(0)));

	bn.GenerateData(10000,"training-data-7");
}
