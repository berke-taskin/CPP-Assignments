#include "EncodeMessage.h"
#include <cmath>


// Default Constructor
EncodeMessage::EncodeMessage() : prime(true) {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    //Character Transformation
    std::string message1 = message;
    for (int i = 0; i < message.length(); i++) {
        //Checking if an index is prime number
        prime = true;
        int ascii = 0;
        if (i < 2){
            prime = false;
        }
        for (int j = 2; j <= i-1; j++){
            if (i % j  == 0){
                prime = false;
            } 
        }
        //Adding corresponding fibonacci number if index is prime
        if (prime==true){
            ascii = message[i]+fibonacci(i);
            if (ascii>=127){
                ascii=126;
            }
            if (ascii<=32){
                ascii=ascii+33;
            }
            message1[i] = static_cast<char>(ascii);
        }
    }

    //Right Circular Shifting
    int shift = message1.length()/2;
    int shifting = message1.size()-shift;
    message1 = message1.substr(shifting) + message1.substr(0, shifting);

    //Message Embedding
    ImageMatrix imgMatrix(img);
    int ind = 0;
    for (const auto& pair : positions) {
        int i = pair.first;
        int j = pair.second;
        double value = imgMatrix.get_data(i,j);
        int message2 = (message1[ind/7] >> (ind%7));
        int lsb = message2 & 1;
        int newValue = (static_cast<int>(value) & 254) + lsb;
        imgMatrix.set_data(i, j, newValue);
        ind++;
    }

    return imgMatrix;
}

int EncodeMessage::fibonacci(const int &num) {
    if (num <= 1) {
        return num;
    }
    return fibonacci(num-1) + fibonacci(num-2);
}
