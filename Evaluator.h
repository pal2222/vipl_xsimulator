#ifndef __oim__Evaluator__
#define __oim__Evaluator__

#include <unordered_set>

#include "Graph.h"
#include "Sampler.hpp"

struct NodeType 
{
	unode_int id;
	unode_int deg;
	bool operator<(const NodeType &a) const 
	{
		return deg < a.deg ? true : (deg > a.deg ? false : id > a.id);
	}
};
/**
Interface for algorithm to specify how is chosen the set of seeds. It is
implemented by Random, HighestDegree, DiscountDegree, CELF, TIM, SSA and PMC.
*/
class Evaluator 
{
protected:
	bool incremental_;

public:
	virtual std::unordered_set<unode_int> select(
		const Graph& graph, Sampler& sampler,
		const std::unordered_set<unode_int>& activated, unsigned int k) = 0; // 纯虚函数 不存在定义

	void setIncremental(bool inc) { incremental_ = inc; }
};

#endif