#include "NetworkRunner.h"

namespace PyNet::Infrastructure {
    
	double* NetworkRunner::Run(double* inputLayer) {

			_pyNetwork->Layers[0]->Set(_pyNetwork->GetInputSize(), inputLayer);

			for (size_t i = 0; i < _pyNetwork->Weights.size(); i++) {
				_layerPropagator->PropagateLayer(*_pyNetwork->Weights[i], *_pyNetwork->Layers[i], *_pyNetwork->Biases[i], *_pyNetwork->Layers[i + 1]);
			}

			_layerNormaliser->NormaliseLayer(_pyNetwork->GetLastLayer());

			return _pyNetwork->GetLastLayer().GetAddress(0);
		}
}
	