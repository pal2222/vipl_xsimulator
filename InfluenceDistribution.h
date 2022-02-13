#ifndef __oim__InfluenceDistribution__
#define __oim__InfluenceDistribution__

#include "common.h"

#define INFLUENCE_MED  0
#define INFLUENCE_UPPER  1
#define INFLUENCE_ADAPTIVE 2
#define INFLUENCE_UCB 3
#define INFLUENCE_THOMPSON 4

class InfluenceDistribution {
protected:
	unode_int hits_ = 0;
	unode_int misses_ = 0;
	double round_ = 0;

public:
	virtual void update(unode_int, unode_int) {};

	virtual void update_prior(double, double) {};

	virtual double mean() = 0;

	virtual double sample(unsigned int) = 0;

	virtual double sq_error() { return 0.0; }

	void set_round(double new_round) { round_ += new_round; }

	unode_int get_hits() { return hits_; }

	unode_int get_misses() { return misses_; }
};

#endif /* defined(__oim__InfluenceDistribution__) */
