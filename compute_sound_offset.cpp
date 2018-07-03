#include <iostream>
#include <iomanip>

#include "sound.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " good_sound.wav bad_sound.wav" << std::endl;
        exit(-1);
    }

    Sound goodSound(argv[1]);
    Sound badSound(argv[2]);

    const int fftsize = std::max(goodSound.frames, badSound.frames) * 2;
    goodSound.addFFT(fftsize);
    badSound.addFFT(fftsize);

    const int delay = goodSound.computeDelay(badSound);
    const double delaySeconds = double(delay) / double(goodSound.samplerate);
    std::cout << std::fixed << std::setprecision(17) << delaySeconds << std::endl;

    return 0;
}
