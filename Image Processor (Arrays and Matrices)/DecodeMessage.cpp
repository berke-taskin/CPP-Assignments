// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <bitset>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    //Extracting a binary string from pixels' lsb values
    std::string binaryString;
    for (const auto& pair : edgePixels) {
        int i = pair.first;
        int j = pair.second;
        double value = image.get_data(i,j);
        int intVal = static_cast<int>(value);
        char lsb = '\0';
        std::string binary;
        std::bitset<7> binaryInt(intVal);
        binary = binaryInt.to_string();
        lsb = binary.back();
        binaryString = binaryString + lsb; 
    }

    //Padding the binary string
    int paddingNum = 7 - (binaryString.length() % 7);
    if (paddingNum==7){
        paddingNum = 0;
    }
    std::string how_many_zeroes;
    for (int i = 0; i < paddingNum; i++) {
        how_many_zeroes = how_many_zeroes + '0';
    }
    paddedString = how_many_zeroes + binaryString;

    //Decoding the message according to the corresponding ASCII values
    std::string decodedMessage;
    for (int i = 0; i < paddedString.length(); i += 7) {
        std::string seven_bit = paddedString.substr(i, 7);
        int ascii = 0;
        int pow = 1;
        for(int i = seven_bit.size()-1; i >= 0; i--) {
            if(seven_bit[i] == '1') {
                ascii = ascii + pow;
            }
            pow = pow*2;
        }   
        if (ascii>=127){
            ascii = 126;
        }
        else if (ascii<=32){
            ascii = ascii + 33;
        }
        decodedMessage = decodedMessage + static_cast<char>(ascii);
    }

    return decodedMessage;
}

