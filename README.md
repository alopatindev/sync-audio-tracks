# Sync Audio Tracks
This code is based on [Miles McCoo's](https://www.mmccoo.com) [cross-correlation implementation](https://nerd.mmccoo.com/2017/06/19/automatically-aligning-multiple-videoaudio-clips-in-kdenlive/).
It calculates a delay between two audios and produces a shifted audio.

## Supported Formats
They depend on how SoX and FFmpeg were built for your OS ([more details](https://github.com/alopatindev/sync-audio-tracks/issues/2#issuecomment-421603812)). If it didn't work with some format for you — try WAV as experiment.

## Installation
Make sure these dependences are installed:
- ffmpeg
- fftw
- libsndfile
- sox

Some packages may need to be installed together with `-dev` packages (for instance `fftw` with `fftw-dev` on Debian).

Now compile it with
```
make
```

## Usage
```
./sync-audio-tracks.sh good_audio_from_recorder.wav bad_audio_from_camera.wav out.wav
```

In this example `out.wav` is a shifted version of `good_audio_from_recorder.wav`

## License
[Apache 2.0](LICENSE.txt)

Copyright (C) 2018—2020 Alexander Lopatin
