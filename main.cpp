#include <boost/random/mersenne_twister.hpp>
#include "common.h"
#include "SpreadSampler.h"
#include "Evaluator.h"
#include "graph_utils.h"
#include "Strategy.h"
#include "LogDiffusion.h"
#include "HighestDegreeEvaluator.hpp"
#include "RandomEvaluator.hpp"
#include "TIMEvaluator.hpp"
using namespace std;


int main()
{


	boost::mt19937 gen;

	Graph g;
	
	SpreadSampler sp(1,1);
	std::vector<std::unique_ptr<Evaluator>> evaluators;
	evaluators.push_back(std::unique_ptr<Evaluator>(new RandomEvaluator()));
	evaluators.push_back(std::unique_ptr<Evaluator>(new HighestDegreeEvaluator()));
	evaluators.push_back(std::unique_ptr<Evaluator>(new TIMEvaluator()));

	double alpha = 1, beta = 1;
	int budget = 50;
	int k = 2;
	int model = 1;
	bool update = true;
	int learn = 1;
	std::unique_ptr<LogDiffusion> log_diffusion;
	string filename = "NetHEPT.csv";
	Graph original_graph, model_graph;  // orininal_graph for real model_graph for estimated
	load_model_and_original_graph(filename, alpha, beta, original_graph,
		model_graph, model);
	ExponentiatedGradientStrategy strategy(
		model_graph, original_graph, *evaluators.at(2),
		update, learn, model, std::move(log_diffusion));
	strategy.perform(budget, k);
	//expgr(argc, argv, evaluators);
	

	return 0;

}


