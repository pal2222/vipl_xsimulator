#ifndef __oim__RandomEvaluator__
#define __oim__RandomEvaluator__

#include "common.h"
#include "Evaluator.h"

#include <time.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

class RandomEvaluator : public Evaluator 
{
private:
	std::mt19937 gen_;
	std::uniform_int_distribution<unode_int> dst_;  // uniform distribution generator

public:
	RandomEvaluator() : gen_(seed_ns()) {};

	std::unordered_set<unode_int> select(const Graph& graph, Sampler&,
		const std::unordered_set<unode_int>&, unsigned int k)    // unordered_set does not contain same value
	{
		dst_ = std::uniform_int_distribution<unode_int>(0, graph.get_number_nodes() - 1);
		std::unordered_set<unode_int> seeds;
		while (seeds.size() < k) 
		{
			unode_int seed = dst_(gen_);
			seeds.insert(seed);
		}
		return seeds;
	}
};

#endif 