﻿#include <math.h>
#include <iostream>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuda_array.h"
#include <vector>
#include <stdlib.h>
#include <stddef.h>
#include "Matrix_Operations.h"

__global__ void matrixMultiplicationKernel(double* A, double* B, double* C, int Acols, int Bcols) {
    int ROW = blockIdx.y * blockDim.y + threadIdx.y;
    int COL = blockIdx.x * blockDim.x + threadIdx.x;

    double tmpSum = 0;

    if (ROW < Acols && COL < Bcols) {
        // each thread computes one element of the block sub-matrix
        for (auto i = 0; i < Acols; i++) {
            tmpSum += A[ROW * Acols + i] * B[i * Bcols + COL];
        }

        C[ROW * Bcols + COL] = tmpSum;
    }
}

__global__ void matrixDoubleMultiplicationKernel(double* A, double* B, double* C, int Acols, int Arows) {
    int ROW = blockIdx.y * blockDim.y + threadIdx.y;
    int COL = blockIdx.x * blockDim.x + threadIdx.x;

    if (ROW < Acols && COL < Arows) {
        C[ROW * Acols + COL] = C[ROW * Acols + COL] * *B;
    }
}

void internalMatrixMultiply(double* A, double* B, double* C, int Acols, int Bcols) {

    // declare the number of blocks per grid and the number of threads per block
    // use 1 to 512 threads per block
    dim3 threadsPerBlock(Acols, Acols);
    dim3 blocksPerGrid(1, 1);
    if (Acols * Acols > 512) {
        threadsPerBlock.x = 512;
        threadsPerBlock.y = 512;
        blocksPerGrid.x = static_cast<int>(ceil(double(Acols) / double(threadsPerBlock.x)));
        blocksPerGrid.y = static_cast<int>(ceil(double(Acols) / double(threadsPerBlock.y)));
    }

    matrixMultiplicationKernel<<<blocksPerGrid, threadsPerBlock>>>(A, B, C, Acols, Bcols);
    cudaDeviceSynchronize();
}

void internalMatrixDoubleMultiply(double* A, double* B, double* C, int Acols, int Arows) {

    // declare the number of blocks per grid and the number of threads per block
    // use 1 to 512 threads per block
    dim3 threadsPerBlock(Acols, Arows);
    dim3 blocksPerGrid(1, 1);
    if (Acols * Arows > 512) {
        threadsPerBlock.x = 512;
        threadsPerBlock.y = 512;
        blocksPerGrid.x = static_cast<int>(ceil(double(Acols) / double(threadsPerBlock.x)));
        blocksPerGrid.y = static_cast<int>(ceil(double(Arows) / double(threadsPerBlock.y)));
    }

    matrixMultiplicationKernel<<<blocksPerGrid, threadsPerBlock>>>(A, B, C, Acols, Arows);
    cudaDeviceSynchronize();
}

void cuda_matrix_multiply(const Matrix& A, const Matrix& B, Matrix& C) {
    C.Initialise(A.GetRows(), B.GetCols(), false);

    cuda_array<double> d_A(A.GetCValues().size());
    cuda_array<double> d_B(B.GetCValues().size());
    cuda_array<double> d_C(C.GetCValues().size());

    d_A.set(A.GetCValues());
    d_B.set(B.GetCValues());

    internalMatrixMultiply(d_A.getData(), d_B.getData(), d_C.getData(), A.GetCols(), B.GetCols());

    d_C.get(C.GetCValues().data(), C.GetSize());
}

void multiply_matrix_and_double(const Matrix& A, const double B, Matrix& C) {

    C.Initialise(A.GetRows(), A.GetCols(), false);

    cuda_array<double> d_A(A.GetCValues().size());
    cuda_array<double> d_B(1);
    cuda_array<double> d_C(C.GetCValues().size());

    d_A.set(A.GetCValues());

    std::vector<double> bVector{ B };
    d_B.set(bVector);

    internalMatrixMultiply(d_A.getData(), d_B.getData(), d_C.getData(), A.GetCols(), A.GetRows());
    d_C.get(C.GetCValues().data(), C.GetSize());
}
