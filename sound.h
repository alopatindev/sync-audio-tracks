#ifndef WAV_UTILS_HH
#define WAV_UTILS_HH

#include <complex>
#include <tuple>
#include <list>
#include <vector>
#include <memory>

#include <fftw3.h>

struct Sound {
    Sound(const std::string& filename)
      : channels(0), frames(0), samplerate(0), samples(0x0), fft(0x0), fftsize(0)
    {
        read(filename);
    }

    ~Sound() {
        free(samples);
        fftw_free(fft);
    }

    void addFFT(int fftsize);
    int computeDelay(const Sound& other);

    int channels;
    int frames;
    int samplerate;

private:
    double* samples;
    std::complex<double>* fft;
    int fftsize;

    void read(const std::string& filename);
};

#endif
