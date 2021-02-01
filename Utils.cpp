#include "Utils.h"

template <typename T>
std::string  outbits(T x){

    std::stringstream Binary;

    using uchar = unsigned char;

    // point to all the bits as group of char's
    T* ptr{&x};
    char* ch = reinterpret_cast<char*>(ptr);

    // access all the bits
    constexpr int N = sizeof(T)/sizeof(char);
    std::bitset<N*CHAR_BIT> bits;
    for(uchar itr{0}, i=0; i<N; ++i){
        std::bitset<CHAR_BIT> b(*(ch+i));
        for(uchar j=0; j<CHAR_BIT; ++j)
            bits[itr++] = b[j];
    }

    // print bits in the right format
    for(int i=N*CHAR_BIT-1; i>=0; --i){ // taking into account endianness
        Binary<<bits[i];
        if(std::is_integral<T>::value){
            if(i%4==0) Binary<<" ";
        }
        if(std::is_floating_point<T>::value){
            if(i==N*CHAR_BIT-1) Binary<<" ";
            switch(N){
            case 4: if(i==N*CHAR_BIT-9) Binary<<" "; break;
            case 8: if(i==N*CHAR_BIT-12) Binary<<" "; break;
        }
        }
    }

    Binary << std::endl;
    return Binary.str();
}

template std::string outbits<float>(float);

std::string char2binary(int Number){
    std::stringstream Binary;
    Binary << std::bitset<8>(Number) << std::endl;
    return Binary.str();
}
