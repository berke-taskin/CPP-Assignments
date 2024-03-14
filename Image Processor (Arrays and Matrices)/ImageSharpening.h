#ifndef IMAGE_SHARPENING_H
#define IMAGE_SHARPENING_H

#include "ImageMatrix.h"
#include "Convolution.h"

class ImageSharpening {
    public:
        ImageSharpening(); // default constructor
        ~ImageSharpening(); 

        ImageMatrix sharpen(const ImageMatrix& input_image, double k);  

    private:
        // add your private functions and variables 
        double** blurring_kernel; // Kernel for blurring
        int kernel_height; // Height of the kernel
        int kernel_width; // Width of the kernel
        int stride; //Stride value
        bool padding; //Zero-padding boolean
};

#endif // IMAGE_SHARPENING_H



