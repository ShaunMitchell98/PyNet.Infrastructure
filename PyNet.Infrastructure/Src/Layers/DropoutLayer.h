#pragma once

#include "Layer.h"
#include "../Settings.h"
#include "PyNet.Models/Matrix.h"
#include "PyNet.DI/Context.h"
#include "BernoulliGenerator.h"

using namespace PyNet::DI;

namespace PyNet::Infrastructure::Layers {

	class DropoutLayer : public Layer {
	private:
		double _rate = 1;
		size_t _rows = 0;
		size_t _cols = 0;
		shared_ptr<Settings> _settings;
		shared_ptr<BernoulliGenerator> _bernoulliGenerator;

		DropoutLayer(shared_ptr<Settings> settings, shared_ptr<BernoulliGenerator> bernoulliGenerator) : _settings{ settings }, _bernoulliGenerator{ bernoulliGenerator } {}

	public:

		static auto factory(shared_ptr<Settings> settings, shared_ptr<BernoulliGenerator> bernoulliGenerator) {
			return new DropoutLayer(settings, bernoulliGenerator);
		}

		size_t GetRows() const override {
			return _rows;
		}

		size_t GetCols() const override {
			return _cols;
		}

		void Initialise(double rate, size_t rows, size_t cols) {
			_rate = rate;
			_rows = rows;
			_cols = cols;
		}

		unique_ptr<Matrix> Apply(unique_ptr<Matrix> input) override {

			auto output = input->Copy();

			unique_ptr<Matrix> droppedMatrix;
			if (_settings->RunMode == RunMode::Training) {
				droppedMatrix = _bernoulliGenerator->GetBernoulliVector(*input, _rate);
			}
			else {
				droppedMatrix = input->Copy();

				for (auto& m : *droppedMatrix) {
					m = _rate;
				}
			}

			*output = *(*input ^ *droppedMatrix);
			return output;
		}

		unique_ptr<Matrix> dLoss_dInput(const Matrix& dLoss_dOutput) const override {
			return dLoss_dOutput.Copy();
		}
	};
}