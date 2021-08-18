#pragma once

#include "../Models/Matrix.h"
#include <stdio.h>
#include <stdlib.h>

using namespace Models;

class ILogger {

public:
	virtual void LogMatrix(Matrix* matrix) = 0;
	virtual void LogMessage(const char* message...) = 0;
	virtual void LogMessageWithoutDate(const char* message) = 0;
	virtual void LogNumber(double number) = 0;
	virtual void LogWhitespace() = 0;
	virtual void LogNewline() = 0;
	virtual void LogDoubleArray(double* array, int length) = 0;
	virtual void LogLine(const char* message) = 0;
};
