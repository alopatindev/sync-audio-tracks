# Sync Audio Tracks
This code is based on [Miles McCoo's cross-correlation implementation](https://web.archive.org/web/20170916044116/https://nerd.mmccoo.com/2017/06/19/automatically-aligning-multiple-videoaudio-clips-in-kdenlive/).
It calculates a delay between two audios and produces a shifted audio.

## Known Limitations
Paths with spaces and weird characters are **unsupported**.

### Supported Formats
They depend on how SoX and FFmpeg were built for your OS ([more details](https://github.com/alopatindev/sync-audio-tracks/issues/2#issuecomment-421603812)). If it didn't work with some format for you — try WAV as experiment.

## Installation
Make sure these dependences are installed:
- bash (tested with 4.4.23)
- bc (tested with 1.06.95)
- ffmpeg (tested with 4.1.3)
- fftw (tested with 3.3.6_p2)
- libsndfile (tested with 1.0.28)
- python3 (tested with 3.6.10)
- sox (tested with 14.4.2)

On Debian some packages may need to be installed together with `-dev` packages (for instance `fftw` with `fftw-dev`).

Now compile it with
```
make -j
```

## Usage
```
./sync-audio-tracks.sh good_audio_from_recorder.wav bad_audio_from_camera.wav out.wav 900
```

In this example
- `out.wav` is a shifted version of `good_audio_from_recorder.wav`
- `900` is sound duration limit for analysis (in seconds)

## Different speed/framerate/framedrops are not supported
This project is much simpler: it just computes and applies the shift (lossless, without re-encoding) without stretching/spreading parts of audio.
~~However something like [that](https://github.com/kaegi/alass) might will help to implement it.~~
Have a look at [AudioAlign](https://github.com/protyposis/AudioAlign), it seems it supports [these scenarios](https://github.com/protyposis/AudioAlign/issues/12#issuecomment-1872321878).

## Support
I'm currently investing [all my time](https://codonaft.com/why) in FOSS projects.

If you found this repo useful and you want to support me, please
- ⭐ it
- check ⚡ [here](https://codonaft.com/sponsor)

Your support keeps me going ❤️ (◕‿◕)

## License
[Apache 2.0](LICENSE.txt)

Copyright (C) 2018—2020 Alexander Lopatin
