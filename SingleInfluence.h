#ifndef __oim__SingleInfluence__
#define __oim__SingleInfluence__

#include "InfluenceDistribution.h"

/**
Influence on *known* edges. This influence distribution is used with the known
graph.
*/
class SingleInfluence : public InfluenceDistribution {
private:
	double value_;

public:
	SingleInfluence(double influence_value) : value_(influence_value) {};

	double mean() { return value_; }

	double sample(unsigned int) { return value_; }
};

#endif