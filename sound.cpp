#include "sound.h"
#include <fftw3.h>
#include <cassert>
#include <iostream>

Sound::Sound(const std::string& filename) : channels(0), frames(0), sampleRate(0) {
    SF_INFO fileInfo;
    fileInfo.format = 0x0;

    SNDFILE* file = sf_open(filename.c_str(), SFM_READ, &fileInfo);
    if (file == nullptr) {
        std::cout << "unable to open file " << filename << std::endl;
        std::cout << "error " << sf_strerror(file) << std::endl;
    } else {
        samples.resize(fileInfo.frames * fileInfo.channels);

        sf_readf_double(file, samples.data(), fileInfo.frames);
        sf_close(file);

        channels = fileInfo.channels;
        frames = fileInfo.frames;
        sampleRate = fileInfo.samplerate;
    }
}

Complex* Sound::computeFFT(const sf_count_t fftSize) const {
    Complex* in = newBuffer(fftSize);
    Complex* out = newBuffer(fftSize);

    fftw_plan plan = fftw_plan_dft_1d(
        fftSize,
        reinterpret_cast<fftw_complex*>(in),
        reinterpret_cast<fftw_complex*>(out),
        FFTW_FORWARD,
        FFTW_ESTIMATE
    );

    for (sf_count_t i = 0; i < frames; i++) {
        in[i] = samples[i * channels];
    }

    for (sf_count_t i = frames; i < fftSize; i++) {
        in[i] = 0;
    }

    fftw_execute(plan);

    fftw_destroy_plan(plan);
    fftw_free(in);

    return out;
}

sf_count_t Sound::computeDelayInFrames(const Sound& other) const {
    const int fftSize = std::max(frames, other.frames) * 2;

    Complex* fft = computeFFT(fftSize);
    Complex* fftOther = other.computeFFT(fftSize);

    Complex* in = newBuffer(fftSize);
    Complex* out = newBuffer(fftSize);

    fftw_plan plan = fftw_plan_dft_1d(
        fftSize,
        reinterpret_cast<fftw_complex*>(in),
        reinterpret_cast<fftw_complex*>(out),
        FFTW_BACKWARD,
        FFTW_ESTIMATE
    );

    for (sf_count_t i = 0; i < fftSize; i++) {
        in[i] = std::conj(fft[i]) * fftOther[i];
    }
    fftw_execute(plan);

    const sf_count_t frameWithMaxCorrelation = argmax(out, fftSize);

    sf_count_t delayFrames = frameWithMaxCorrelation;
    const bool negativeSign = delayFrames > fftSize / 2;
    if (negativeSign) {
        delayFrames -= fftSize;
    }

    fftw_free(fft);
    fftw_free(fftOther);
    fftw_free(in);
    fftw_free(out);

    return delayFrames;
}

double Sound::computeDelayInSeconds(const Sound& other) const {
    const sf_count_t delayFrames = computeDelayInFrames(other);
    return static_cast<double>(delayFrames) / static_cast<double>(sampleRate);
}

Complex* Sound::newBuffer(const sf_count_t fftSize) const {
    const size_t size = sizeof(Complex) * fftSize;
    return reinterpret_cast<Complex*>(fftw_malloc(size));
}

sf_count_t Sound::argmax(const Complex* const buffer, const sf_count_t size) const {
    sf_count_t maxIndex = 0;
    double maxValue = std::abs(buffer[maxIndex]);

    for (sf_count_t i = 1; i < size; i++) {
        const double magnitude = std::abs(buffer[i]);
        if (magnitude > maxValue) {
            maxIndex = i;
            maxValue = magnitude;
        }
    }

    return maxIndex;
}
