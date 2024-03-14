// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() : Gx_height(3), Gx_width(3), Gx_data(nullptr), Gy_height(3), Gy_width(3), Gy_data(nullptr){
    //Creating Gx matrix
    const double* gx_data1[3] = {
    new double[3]{-1.0, 0.0, 1.0},
    new double[3]{-2.0, 0.0, 2.0},
    new double[3]{-1.0, 0.0, 1.0},
    };
    // Allocate memory for the matrix
    Gx_data = new double*[Gx_height];
    for (int i = 0; i < Gx_height; ++i) {
        Gx_data[i] = new double[Gx_width];
    }
    // Copy data from Gx Matrix to data
    for (int i = 0; i < Gx_height; ++i) {
        for (int j = 0; j < Gx_width; j++) {
            Gx_data[i][j] = gx_data1[i][j];
        }
    }
    //Creating Gy matrix
    const double* gy_data1[3] = {
    new double[3]{-1.0, -2.0, -1.0},
    new double[3]{0.0, 0.0, 0.0},
    new double[3]{1.0, 2.0, 1.0},
    };
    // Allocate memory for the matrix
    Gy_data = new double*[Gy_height];
    for (int i = 0; i < Gy_height; ++i) {
        Gy_data[i] = new double[Gy_width];
    }
    // Copy data from Gy Matrix to data
    for (int i = 0; i < Gy_height; ++i) {
        for (int j = 0; j < Gy_width; j++) {
            Gy_data[i][j] = gy_data1[i][j];
        }
    }
}

// Destructor
EdgeDetector::~EdgeDetector() {
    // Deallocate Gx memory
    if (Gx_data != nullptr) {
        for (int i = 0; i < Gx_height; ++i) {
            delete[] Gx_data[i];
        }
        delete[] Gx_data;
        Gx_data = nullptr;
    }
    // Deallocate Gy memory
    if (Gy_data != nullptr) {
        for (int i = 0; i < Gy_height; ++i) {
            delete[] Gy_data[i];
        }
        delete[] Gy_data;
        Gy_data = nullptr;
    }
}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    //Convolving images with Gx and Gy
    Convolution EdgeDetectorConv_Gx(Gx_data,3,3,1,true);
    Convolution EdgeDetectorConv_Gy(Gy_data,3,3,1,true);
    ImageMatrix convolvedImage_Gx = EdgeDetectorConv_Gx.convolve(input_image);
    ImageMatrix convolvedImage_Gy = EdgeDetectorConv_Gy.convolve(input_image);
    ImageMatrix magnitudeMatrix(input_image.get_height(), input_image.get_width());

    //Creating a matrix with magnitude values
    for (int i = 0; i < input_image.get_height(); ++i) {
        for (int j = 0; j < input_image.get_width(); ++j) {
            double pixelGx = convolvedImage_Gx.get_data(i,j);
            double pixelGy = convolvedImage_Gy.get_data(i,j);
            double magnitude = sqrt((pixelGx*pixelGx)+(pixelGy*pixelGy));
            magnitudeMatrix.set_data(i,j,magnitude);
        }
    }

    //Calculating average
    double avg_sum = 0.0;
    for (int i = 0; i < input_image.get_height(); ++i) {
        for (int j = 0; j < input_image.get_width(); ++j) {
            avg_sum = avg_sum + magnitudeMatrix.get_data(i,j);
        }
    }
    double mul = (input_image.get_height()*input_image.get_width());
    double avg = avg_sum / (input_image.get_height()*input_image.get_width());

    //Finding magnitudes passing the treshhold
    std::vector<std::pair<int, int>> result;
    for (int i = 0; i < input_image.get_height(); ++i) {
        for (int j = 0; j < input_image.get_width(); ++j) {
            if (magnitudeMatrix.get_data(i,j)>avg){
                result.push_back(std::make_pair(i, j));
            }
        }
    }
    
    return result;
}

