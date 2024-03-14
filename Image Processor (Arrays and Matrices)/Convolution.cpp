#include <iostream>

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() : height (3), width (3), stride(1), padding(true), kernelData(nullptr){
    // Allocate memory for the matrix
    kernelData = new double*[height];
    for (int i = 0; i < height; ++i) {
        kernelData[i] = new double[width];
    }
    // Copy data as values of zero
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            kernelData[i][j] = 1 / (double) 9;
        }
    }
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad) : height(kh), width(kw), stride(stride_val), padding(pad){
    // Allocate memory for the kernel
    kernelData = new double*[height];
    for (int i = 0; i < height; ++i) {
        kernelData[i] = new double[width];
    }

    // Copy data from customKernel to kernelData
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; j++) {
            kernelData[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {
    // Deallocate memory
    if (kernelData != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] kernelData[i];
        }
        delete[] kernelData;
        kernelData = nullptr;
    }
}

// Copy constructor
Convolution::Convolution(const Convolution &other) : height(other.height), width(other.width), stride(other.stride), padding(other.padding){
    // Allocate memory for the kernel and copy data
    kernelData = new double*[height];
    for (int i = 0; i < height; ++i) {
        kernelData[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            kernelData[i][j] = other.kernelData[i][j];
        }
    }
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this; // self-assignment check
    }

    // Deallocate old memory
    if (kernelData != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] kernelData[i];
        }
        delete[] kernelData;
        kernelData = nullptr;
    }

    // Copy from other
    height = other.height;
    width = other.width;
    stride = other.stride;
    padding = other.padding;
    kernelData = new double*[height];
    for (int i = 0; i < height; ++i) {
        kernelData[i] = new double[width];
        for (int j = 0; j < width; ++j) {
            kernelData[i][j] = other.kernelData[i][j];
        }
    }

    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    //Calculating output height and width
    int inHeight = input_image.get_height();
    int inWidth = input_image.get_width();
    int outHeight = ((inHeight - height + 2*padding) / (stride))+1;
    int outWidth = ((inWidth - width + 2*padding) / (stride))+1;

    //Creating the convolved matrix
    ImageMatrix convolvedMatrix (outHeight, outWidth);

    //Implementing the convolution formula
    for (int i = 0; i < outHeight; ++i) {
        for (int j = 0; j < outWidth; ++j) {
            int row1 = i*stride - padding;
            int col1 = j*stride - padding;
            int row2 = row1 + height;
            int col2 = col1 + width;
            double sum = 0.0;
            for (int x = row1; x < row2; x++) {
                for (int y = col1; y < col2; y++) {
                    if (x >= 0 && x < inHeight && y >= 0  && y < inWidth) {
                        double mul = kernelData[x-row1][y-col1]*input_image.get_data(x, y);
                        sum = sum + mul;
                    }
                }
            }
            convolvedMatrix.set_data(i,j,sum);
        }   
    }
    
    return convolvedMatrix;
}