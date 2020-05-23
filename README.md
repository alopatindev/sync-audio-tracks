# Sync Audio Tracks
This code is based on [Miles McCoo's cross-correlation implementation](https://web.archive.org/web/20170916044116/https://nerd.mmccoo.com/2017/06/19/automatically-aligning-multiple-videoaudio-clips-in-kdenlive/).
It calculates a delay between two audios and produces a shifted audio.

## Supported Formats
They depend on how SoX and FFmpeg were built for your OS ([more details](https://github.com/alopatindev/sync-audio-tracks/issues/2#issuecomment-421603812)). If it didn't work with some format for you — try WAV as experiment.

## Installation
Make sure these dependences are installed:
- bash (tested with 4.4.23)
- bc (tested with 1.06.95)
- ffmpeg (tested with 4.1.3)
- fftw (tested with 3.3.6_p2)
- libsndfile (tested with 1.0.28)
- sox (tested with 14.4.2)

On Debian some packages may need to be installed together with `-dev` packages (for instance `fftw` with `fftw-dev`).

Now compile it with
```
make -j
```

## Usage
```
./sync-audio-tracks.sh good_audio_from_recorder.wav bad_audio_from_camera.wav out.wav
```

In this example `out.wav` is a shifted version of `good_audio_from_recorder.wav`

## License
[Apache 2.0](LICENSE.txt)

Copyright (C) 2018—2020 Alexander Lopatin
