//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_FULLYCONNECTEDLAYER_HPP
#define AEVO_FULLYCONNECTEDLAYER_HPP

#include <brain/Layer.hpp>
#include <brain/Neuron.hpp>

#include <shared/Utils.hpp>

class FullyConnectedLayer : public Layer {
protected:
	vector<Neuron> neurons;//TODO Make private
public:
	FullyConnectedLayer(vector<vector<weight> > &);

	FullyConnectedLayer(const FullyConnectedLayer &);

	FullyConnectedLayer(vector<Neuron> &);

	~FullyConnectedLayer();

	void operator=(const FullyConnectedLayer &);

	vector<weight> getNeuronWeights(int i);

	void setNeuronWeights(int i, vector<weight> &);

	Layer *getDeepCopy();

	void compute();

	void save(ostream &);

	static FullyConnectedLayer getLoadedLayer(istream &);
};


#endif //AEVO_FULLYCONNECTEDLAYER_HPP
