#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() : imageSharpening1(ImageSharpening()), edgeDetector1(EdgeDetector()) {
}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    //Sharpening the image
    ImageMatrix SharpenedImage = imageSharpening1.sharpen(img,2);

    //Finding the edge pixels
    std::vector<std::pair<int, int>> edgePixels = edgeDetector1.detectEdges(SharpenedImage);

    //Decoding the message
    DecodeMessage decodeMessage;
    std::string decodedMessage = decodeMessage.decodeFromImage(SharpenedImage, edgePixels);

    return decodedMessage;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    //Sharpening the image
    ImageMatrix SharpenedImage = imageSharpening1.sharpen(img,2);

    //Finding the edge pixels
    std::vector<std::pair<int, int>> edgePixels = edgeDetector1.detectEdges(SharpenedImage);

    //Encoding the message
    EncodeMessage encodeMessage;
    ImageMatrix imgMatrix(encodeMessage.encodeMessageToImage(img, message, edgePixels));

    return imgMatrix;
}
