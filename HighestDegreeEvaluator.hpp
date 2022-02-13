#ifndef __oim__HighestDegreeEvaluator__
#define __oim__HighestDegreeEvaluator__

#include "common.h"
#include "Evaluator.h"

#include <boost/heap/fibonacci_heap.hpp>

class HighestDegreeEvaluator : public Evaluator
{
private:
	std::unordered_set<unode_int> seed_sets_;

	std::unordered_set<unode_int> select(
		const Graph& graph, Sampler&,
		const std::unordered_set<unode_int>&, unsigned int k) 
	{
		std::unordered_set<unode_int> set;
		boost::heap::fibonacci_heap<NodeType> queue;
		for (unode_int node : graph.get_nodes()) 
		{
			NodeType nstruct;
			nstruct.id = node;
			nstruct.deg = 0;
			if (graph.has_neighbours(node))
				nstruct.deg = graph.get_neighbours(node).size();
			queue.push(nstruct);
		}
		while (set.size() < k && !queue.empty())
		{
			NodeType nstruct = queue.top();
			if (seed_sets_.find(nstruct.id) == seed_sets_.end())
			{
				// Guarantee no duplicate nodes in the seed set for all trials
				set.insert(nstruct.id);
				seed_sets_.insert(nstruct.id);
			}
			queue.pop();
		}
		return set;
	}
};

#endif 