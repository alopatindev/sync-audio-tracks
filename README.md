# Sync Audio Tracks
This code is based on [Miles McCoo's](https://www.mmccoo.com) [cross-correlation implementation](https://nerd.mmccoo.com/2017/06/19/automatically-aligning-multiple-videoaudio-clips-in-kdenlive/).
It calculates a delay between two audios and produces a shifted audio.

# Usage
`./sync-audio-tracks.sh good_audio_from_recorder.wav bad_audio_from_camera.wav out.wav`

# Dependencies
- boost
- ffmpeg
- fftw
- libsndfile
- sox
