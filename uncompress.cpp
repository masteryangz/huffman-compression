#include <vector>
#include "HCTree.hpp"
int main(int argc, char* argv[]){
    if(argc != 3){
        error("Invalid number of arguments");
    }
    FancyInputStream inf(argv[1]);
    vector<int> freqs(256,0);
    FancyOutputStream out(argv[2]);
    unsigned char rbyte = 0;
    if((rbyte = inf.read_byte()) == 255){
        return 0;
    }
    int frequency = 0;
    frequency = rbyte<<8;
    rbyte = inf.read_byte();
    frequency = frequency + (rbyte<<4);
    rbyte = inf.read_byte();
    frequency = frequency + rbyte;
    freqs[0] = frequency;
    for(unsigned int i = 1; i < 256; i++){
        //freqs[i] = inf.read_int();
        frequency = 0;
        rbyte = inf.read_byte();
        frequency = rbyte<<8;
        rbyte = inf.read_byte();
        frequency = frequency + (rbyte<<4);
        rbyte = inf.read_byte();
        frequency = frequency + rbyte;
        freqs[i] = frequency;
    }
    HCTree hct;
    hct.build(freqs);
    while((rbyte = hct.decode(inf)) !=0){
        //rbyte = hct.decode(inf);
        out.write_byte(rbyte);
    }
    return 0;
}
