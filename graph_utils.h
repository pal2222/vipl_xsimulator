#ifndef __oim__graph_utils__
#define __oim__graph_utils__

#include <iostream>
#include <fstream>

#include "InfluenceDistribution.h"
#include "SingleInfluence.h"
#include "BetaInfluence.h"
#include "Graph.h"


/**
Load the graph from file and returns the number of nodes
*/
unode_int load_original_graph(
	std::string filename, Graph& graph, int model = 1) {
	std::ifstream file(filename);
	unode_int src, tgt;
	double prob;
	unode_int edges = 0;
	while (file >> src >> tgt >> prob) {
		std::shared_ptr<InfluenceDistribution> dst_original(
			new SingleInfluence(prob));
		graph.add_edge(src, tgt, dst_original);
		edges++;
	}
	if (model == 0) // If LT model, we need to create distributions for each nodes
		graph.build_lt_distribution(INFLUENCE_MED);
	return edges;
}

/**
Load the graph from file in two Graph objects: (a) original graph (the *real*
graph) (b) model graph (graph estimation).
Returns the number of nodes.
*/
unode_int load_model_and_original_graph(
	std::string filename, double alpha, double beta,
	Graph& original_graph, Graph& model_graph, int model = 1) {
	std::ifstream file(filename);
	unode_int src, tgt;
	double prob;
	unode_int edges = 0;
	while (file >> src >> tgt >> prob) {
		std::shared_ptr<InfluenceDistribution> dst_original(
			new SingleInfluence(prob));
		std::shared_ptr<InfluenceDistribution> dst_model(
			new BetaInfluence(alpha, beta, prob));
		original_graph.add_edge(src, tgt, dst_original);
		model_graph.add_edge(src, tgt, dst_model);
		edges++;
	}
	if (model == 0) { // If LT model, we need to create distributions for each nodes
		original_graph.build_lt_distribution(INFLUENCE_MED);
		// Not for model graph as it is used only by expg that does not handle LT
	}
	model_graph.set_prior(alpha, beta);
	return edges;
}

#endif