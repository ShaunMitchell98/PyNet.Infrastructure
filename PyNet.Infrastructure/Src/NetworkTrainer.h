#pragma once

#include <memory>
#include <vector>
#include "PyNet.Models/ILogger.h"
#include "PyNet.Models/Vector.h"
#include "PyNet.Models/Context.h"
#include "AdjustmentCalculator.h"

using namespace PyNet::Models;

class NetworkTrainer
{
private:
	Vector& _dError_dActivatedLayerAbove;
	Vector& _dError_dActivatedOutput;
	ILogger& _logger;
	AdjustmentCalculator& _adjustmentCalculator;
	Settings& _settings;
	di::Context& _context;

	double CalculateErrorDerivativeForFinalLayer(Vector* finalLayer, Vector* expectedLayer);
	void GetAdjustmentsForWeightMatrix(Matrix* weightMatrix, Vector* inputLayer, Vector* outputLayer, int weightMatrixIndex);
	void GetAdjustments(std::vector<Matrix*> weightMatrices, std::vector<Vector*> layers);
	void GetdError_dActivatedOutput(Matrix* weightMatrix, PyNet::Models::Vector* inputLayer, PyNet::Models::Vector* outputLayer);
public:

	static auto factory(ILogger& logger, AdjustmentCalculator& adjustmentCalculator, Settings& settings, di::Context& context,
		Vector& dError_dActivatedLayerAbove, Vector& dError_dActivatedOutput) {
		return new NetworkTrainer{ logger, adjustmentCalculator, settings, context, dError_dActivatedLayerAbove, dError_dActivatedOutput };
	}

	NetworkTrainer(ILogger& logger, AdjustmentCalculator& adjustmentCalculator, Settings& settings, di::Context& context, Vector& dError_dActivatedLayerAbove,
		Vector& dError_dActivatedOutput) : _logger(logger), _adjustmentCalculator(adjustmentCalculator), _settings(settings), _context(context),
		_dError_dActivatedLayerAbove(dError_dActivatedLayerAbove), _dError_dActivatedOutput(dError_dActivatedOutput) {}

	double TrainNetwork(std::vector<Matrix*> weightMatrices, std::vector<Vector*> layers, PyNet::Models::Vector* expectedLayer);
	void UpdateWeights(std::vector<Matrix*> weightMatrices, std::vector<Vector*> biases, double learningRate);
};

