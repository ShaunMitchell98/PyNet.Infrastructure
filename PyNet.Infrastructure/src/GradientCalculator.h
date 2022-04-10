#pragma once
#include <memory>
#include <vector>
#include "PyNet.DI/Context.h"
#include "AdjustmentCalculator.h"
#include "PyNet.Models/ILogger.h"

using namespace std;
using namespace PyNet::Models;
using namespace PyNet::DI;

namespace PyNet::Infrastructure {

	class GradientCalculator {
	private:
		shared_ptr<Context> _context;
		shared_ptr<AdjustmentCalculator> _adjustmentCalculator;
		shared_ptr<ILogger> _logger;

		unique_ptr<Matrix> CalculateWeightMatrixGradient(const Matrix& layerAboveMatrix, const Vector& inputLayer, Vector& outputLayer,
			Vector& dLoss_dLayerAbove);
		double CalculateBiasGradient(const Matrix& layerAboveMatrix, const Vector& inputLayer, Vector& outputLayer, Vector& dLoss_dLayerAbove);

		GradientCalculator(shared_ptr<Context> context, shared_ptr<AdjustmentCalculator> adjustmentCalculator, shared_ptr<ILogger> logger) :
			_context{ context }, _adjustmentCalculator{ adjustmentCalculator }, _logger{ logger }{}

	public:

		static auto factory(shared_ptr<Context> context, shared_ptr<AdjustmentCalculator> adjustmentCalculator, shared_ptr<ILogger> logger) {
			return new GradientCalculator{ context, adjustmentCalculator, logger };
		}

		void CalculateGradients(const vector<unique_ptr<Matrix>>& weightMatrices,
			const vector<unique_ptr<Vector>>& layers, const Vector& expectedLayer, const Vector& lossDerivative);
	};
}
