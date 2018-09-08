#ifndef SOUND_H
#define SOUND_H

#include <complex>
#include <vector>

#include <fftw3.h>
#include <sndfile.h>

using Complex = std::complex<double>;

class Sound {
public:
    Sound(const std::string& filename);

    sf_count_t computeDelayInFrames(const Sound& other) const;
    double computeDelayInSeconds(const Sound& other) const;

private:
    Complex* computeFFT(const sf_count_t fftSize) const;
    Complex* newBuffer(const sf_count_t fftSize) const;
    sf_count_t argmax(const Complex* const buffer, const sf_count_t size) const;

    int channels;
    sf_count_t frames;
    int sampleRate;
    std::vector<double> samples;
};

#endif
