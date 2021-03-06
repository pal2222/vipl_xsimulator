
#ifndef oim_samplemanager_h
#define oim_samplemanager_h

#include "common.h"
#include "Sampler.hpp"
#include "SpreadSampler.h"
#include "InfluenceDistribution.h"

#define INCREMENTAL_THRESHOLD 0.02

using namespace std;

struct SampleType {
	shared_ptr<vector<unode_int>> sample;
	int age; // this sample is generated at trial #age
	int lastUsedTrial;
	double alpha, beta; // this sample is generated under prior (alpha, beta)
};

class SampleManager {
private:
	const Graph& graph_;
	vector<SampleType> sample_pool_;
	int pointer_;
	std::random_device rd_;
	std::mt19937 gen_;

	SampleManager(const Graph& graph) : graph_(graph), gen_(rd_()) {
		sample_pool_.clear();
		sample_pool_.reserve(MAX_R);
		pointer_ = -1;
	}

	~SampleManager() {
		if (instance) delete instance;
		if (exploreInstance) delete exploreInstance;
	}

	static SampleManager* instance;
	static SampleManager* exploreInstance;
	static int currentTrial;
	static bool explore;
	static vector<int> node_age;
	static double hit, miss, case1, case2, case3;

public:
	static void setInstance(const Graph& graph) {
		if (instance) delete instance;
		if (exploreInstance) delete exploreInstance;
		instance = new SampleManager(graph);
		exploreInstance = new SampleManager(graph);
		node_age = vector<int>(graph.get_number_nodes(), -1);
		currentTrial = -1;
		explore = false;
	}

	static SampleManager* getInstance() {
		return explore ? exploreInstance : instance;
	}

	static void reset(int trial, bool _explore = false) {
		hit = 0, miss = 0;
		case1 = 0, case2 = 0, case3 = 0;
		currentTrial = trial;
		explore = _explore;
		if (explore && exploreInstance->sample_pool_.size()) {
			exploreInstance->pointer_ = -1;
		}
		if (!explore && instance->sample_pool_.size()) {
			instance->pointer_ = -1;
		}
	}

	static void update_node_age(const unordered_set<unode_int>& trials) {
		if (hit + miss > 0.1) {
			reused_ratio = (hit) / (hit + miss);
		}
		for (auto node : trials) {
			node_age[(int)node] = currentTrial;
		}
	}

	double ABS(double x) {
		return x < -1e-9 ? -x : x;
	}

	bool isAccepted(double alpha, double beta) {
		return ABS(alpha / (alpha + beta) - graph_.alpha_prior / (graph_.alpha_prior
			+ graph_.beta_prior)) <= INCREMENTAL_THRESHOLD;
	}

	// hardcoded for reverse set
	shared_ptr<vector<unode_int>> getSample(
		const vector<unode_int>& graph_nodes, Sampler& sampler,
		const unordered_set<unode_int>& activated,
		std::uniform_int_distribution<int>& dst) {
		bool goodSampleFlag = true;
		// check whether there is sample that can be reused
		do {
			if (sample_pool_.size() == 0) break;
			SampleType& sample = sample_pool_[(pointer_ + 1) % (int)sample_pool_.size()];
			if (sample.lastUsedTrial >= currentTrial) {
				case1 += 1;
				break;
			}

			if (!isAccepted(sample.alpha, sample.beta)) {
				goodSampleFlag = false;
				case2 += 1;
				break;
			}
			for (auto node : *(sample.sample)) {
				if (node_age[node] >= sample.age) {
					goodSampleFlag = false;
					case3 += 1;
					break;
				}
			}
			if (!goodSampleFlag) break;

			hit += 1;
			sample.lastUsedTrial = currentTrial;
			pointer_++;
			return sample.sample;
		} while (false);

		miss += 1;
		std::unordered_set<unode_int> seeds;
		unode_int nd = graph_nodes[dst(gen_)];
		seeds.insert(nd);
		sampler.trial(graph_, activated, seeds, true);

		shared_ptr<vector<unode_int>>sample(new vector<unode_int>());
		sample->push_back(nd);
		for (TrialType tt : sampler.get_trials()) {
			if (tt.trial == 1) {
				sample->push_back(tt.target);
			}
		}

		if (!goodSampleFlag || (int)sample_pool_.size() >= MAX_R) {
			pointer_ = (pointer_ + 1) % (int)sample_pool_.size();
		}
		else {
			pointer_ = sample_pool_.size();
			sample_pool_.push_back(SampleType());
		}
		SampleType& new_sample = sample_pool_[pointer_];
		new_sample.alpha = graph_.alpha_prior;
		new_sample.beta = graph_.beta_prior;
		new_sample.sample = sample;
		new_sample.age = currentTrial;
		new_sample.lastUsedTrial = currentTrial;
		return new_sample.sample;
	}
};

SampleManager* SampleManager::instance = NULL;
SampleManager* SampleManager::exploreInstance = NULL;
int SampleManager::currentTrial = -1;
bool SampleManager::explore = false;
vector<int> SampleManager::node_age = vector<int>();
double SampleManager::hit = 0;
double SampleManager::miss = 0;
double SampleManager::case1 = 0;
double SampleManager::case2 = 0;
double SampleManager::case3 = 0;

#endif