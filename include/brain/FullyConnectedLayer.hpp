//
// Created by reuben on 5/3/17.
//

#ifndef AEVO_FULLYCONNECTEDLAYER_HPP
#define AEVO_FULLYCONNECTEDLAYER_HPP

#include <brain/Layer.hpp>
#include <brain/Neuron.hpp>

class FullyConnectedLayer : public Layer {
public:
	vector<Neuron> neurons;//TODO Make private
	FullyConnectedLayer(vector<vector<weight> > &);

	FullyConnectedLayer(FullyConnectedLayer &);

	~FullyConnectedLayer();

	void set(FullyConnectedLayer &);

	FullyConnectedLayer operator=(FullyConnectedLayer &);

	Layer *getDeepCopy();


	void compute();
};


#endif //AEVO_FULLYCONNECTEDLAYER_HPP
