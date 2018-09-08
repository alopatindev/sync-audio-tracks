#include <iostream>
#include <iomanip>

#include "sound.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " good_sound.wav bad_sound.wav" << std::endl;
        return -1;
    }

    const Sound goodSound(argv[1]);
    const Sound badSound(argv[2]);
    const double delay = goodSound.computeDelayInSeconds(badSound);
    std::cout << std::fixed << std::setprecision(17) << delay << std::endl;

    return 0;
}
