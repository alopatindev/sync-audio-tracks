#include "sound.h"

#include <fftw3.h>
#include <sndfile.h>

#include <iostream>

void Sound::read(const std::string& filename) {
    SF_INFO sfinfo;
    sfinfo.format = 0x0;

    SNDFILE* sfile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    if (sfile == NULL) {
        std::cout << "unable to open file " << filename << std::endl;
        std::cout << "error " << sf_strerror(sfile) << std::endl;
        exit(-1);
    }

    samples = (double*) malloc(sizeof(double) * sfinfo.frames * sfinfo.channels);
    sf_readf_double(sfile, samples, sfinfo.frames);
    sf_close(sfile);

    channels = sfinfo.channels;
    frames = sfinfo.frames;
    samplerate = sfinfo.samplerate;
}

void Sound::addFFT(int fftsize) {
    std::complex<double>* in = (std::complex<double>*) fftw_malloc(sizeof(std::complex<double>) * fftsize);
    std::complex<double>* out = (std::complex<double>*) fftw_malloc(sizeof(std::complex<double>) * fftsize);
    fftw_plan p = fftw_plan_dft_1d(
        fftsize,
        reinterpret_cast<fftw_complex*>(in),
        reinterpret_cast<fftw_complex*>(out),
        FFTW_FORWARD,
        FFTW_ESTIMATE
    );

    for(int i = 0; i < frames; i++) {
        in[i] = samples[i * channels];
    }

    for(int i = frames; i < fftsize; i++) {
        in[i] = 0;
    }

    fftw_execute(p);

    this->fft = out;
    this->fftsize = fftsize;

    fftw_destroy_plan(p);
    fftw_free(in);
}

int Sound::computeDelay(const Sound& other) {
    std::complex<double>* in = (std::complex<double>*) fftw_malloc(sizeof(std::complex<double>) * fftsize);
    std::complex<double>* out = (std::complex<double>*) fftw_malloc(sizeof(std::complex<double>) * fftsize);

    fftw_plan p_backward = fftw_plan_dft_1d(
        fftsize,
        reinterpret_cast<fftw_complex*>(in),
        reinterpret_cast<fftw_complex*>(out),
        FFTW_BACKWARD,
        FFTW_ESTIMATE
    );

    for (int i = 0; i < fftsize; i++) {
        in[i] = std::conj(fft[i]) * other.fft[i];
    }
    fftw_execute(p_backward);

    int maxid = 0;
    double maxmag = std::abs(out[maxid]);
    for (int i = 1; i < fftsize; i++) {
        const double mag = std::abs(out[i]);
        if (mag > maxmag) {
            maxid = i;
            maxmag = mag;
        }
    }

    fftw_free(in);
    fftw_free(out);

    return maxid > fftsize / 2 ? maxid - fftsize : maxid;
}
