    #ifndef EDGE_DETECTOR_H
    #define EDGE_DETECTOR_H

    #include "ImageMatrix.h"
    #include "Convolution.h"
    #include <vector>

    class EdgeDetector {
        public:
            EdgeDetector(); 
            ~EdgeDetector(); 
            
            std::vector<std::pair<int, int>> detectEdges(const ImageMatrix& input_image);

        private:
            // add your private member variables and functions 
            double** Gx_data; // 2D array to store Gx data
            int Gx_height; // Height of the Gx matrix
            int Gx_width; // Width of the Gx matrix
            double** Gy_data; // 2D array to store Gy data
            int Gy_height; // Height of the Gy matrix
            int Gy_width; // Width of the Gy matrix
    };

    #endif // EDGE_DETECTOR_H


