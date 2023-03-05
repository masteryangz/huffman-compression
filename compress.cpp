#include <vector>
#include "HCTree.hpp"
int main(int argc, char* argv[]){
    if(argc != 3){
        error("Invalid number of arguments");
    }
    FancyInputStream inf(argv[1]);
    vector<int> freqs(256,0);
    int inbyte = 0;
    FancyOutputStream out(argv[2]);
    if((inbyte = inf.read_byte()) == -1){
        return 0;
    }
    freqs[inbyte]++;
    while((inbyte = inf.read_byte())!=-1){
        freqs[inbyte]++;
    }
    HCTree hct;
    hct.build(freqs);
    
    for(unsigned int i = 0; i <= 255; i++){
        //out.write_int(freqs[i]);
        unsigned char first_byte = freqs[i]>>8;
        out.write_byte(first_byte);
        first_byte = freqs[i] & 240;
        first_byte = first_byte>>4;
        out.write_byte(first_byte);
        first_byte = freqs[i] & 15;
        out.write_byte(first_byte);
    }
    inf.reset();
    inbyte = 0;
    // if((inbyte = inf.read_byte())==-1){
    //     return 0;
    // }
    while((inbyte = inf.read_byte())!=-1){
        hct.encode(inbyte, out);
    }
    return 0;
}
