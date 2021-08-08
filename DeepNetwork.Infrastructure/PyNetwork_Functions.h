#ifndef KERNEL_PYNETWORK_FUNCTIONS
#define KERNEL_PYNETWORK_FUNCTIONS

#include "PyNetwork.h"

extern "C" {
#define EXPORT_SYMBOL __declspec(dllexport)

	EXPORT_SYMBOL PyNetwork* PyNetwork_New(int count) { return new PyNetwork(count); }
	EXPORT_SYMBOL void PyNetwork_AddLayer(PyNetwork* pyNetwork, int count) { pyNetwork->AddLayer(count); }
	EXPORT_SYMBOL void PyNetwork_Run(PyNetwork* pyNetwork, double* input_layer) { pyNetwork->Run(input_layer); }
	EXPORT_SYMBOL void PyNetwork_Train(PyNetwork* pyNetwork, double** inputLayers, double** expectedOutputs, int numberOfExamples, int batchSize) {
		pyNetwork->Train(inputLayers, expectedOutputs, numberOfExamples, batchSize);
	}

#undef EXPORT_SYMBOL
}

#endif