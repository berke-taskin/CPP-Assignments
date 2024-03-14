#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() : kernel_height (3), kernel_width (3), stride(1) , padding (true), blurring_kernel(nullptr){
    // Allocate memory for the matrix
    blurring_kernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; ++i) {
        blurring_kernel[i] = new double[kernel_width];
    }
    // Copy data as values of zero
    for (int i = 0; i < kernel_height; ++i) {
        for (int j = 0; j < kernel_width; j++) {
            blurring_kernel[i][j] = 1 / (double) 9;
        }
    }
}

ImageSharpening::~ImageSharpening(){
    // Deallocate memory
    if (blurring_kernel != nullptr) {
        for (int i = 0; i < kernel_height; ++i) {
            delete[] blurring_kernel[i];
        }
        delete[] blurring_kernel;
        blurring_kernel = nullptr;
    }
}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    //Sharpening the image
    Convolution ImageSharpeningConv(blurring_kernel, kernel_height, kernel_width, stride, padding);
    ImageMatrix noisyImage = ImageSharpeningConv.convolve(input_image);
    ImageMatrix subtractedImage = input_image.operator-(noisyImage);
    ImageMatrix multipliedImage = subtractedImage.operator*(k);
    ImageMatrix sharpenedImage = input_image.operator+(multipliedImage);
    for (int i = 0; i < kernel_height; ++i) {
        for (int j = 0; j < kernel_width; j++) {
            if (sharpenedImage.get_data(i,j)<0){
                sharpenedImage.set_data(i,j,0);
            }
            else if (sharpenedImage.get_data(i,j)>255)
                sharpenedImage.set_data(i,j,255);
        }
    }

    return sharpenedImage;
}
