#!/bin/bash

set -e

DEFAULT_DURATION_LIMIT_SECS=900

if [ $# -lt 3 ]; then
    echo "usage: $0 good_sound.wav bad_sound.wav shifted_good_sound.wav [duration_limit]"
    echo "where"
    echo "  good_sound.wav - taken from audio recorder"
    echo "  bad_sound.wav - taken from camera"
    echo "  shifted_good_sound.wav - synchronized version of good_sound.wav"
    echo "  duration_limit - max seconds to analyze, default is ${DEFAULT_DURATION_LIMIT_SECS}, 0 means no limit"
    exit 1
fi

GOOD_SOUND="$1"
BAD_SOUND="$2"
OUTPUT="$3"
DURATION_LIMIT="${4:-${DEFAULT_DURATION_LIMIT_SECS}}"

SCRIPT_DIR="$(dirname $(realpath $0))"
COMPUTE_OFFSET_SH="${SCRIPT_DIR}/compute-sound-offset.sh"
OFFSET=$("${COMPUTE_OFFSET_SH}" "${GOOD_SOUND}" "${BAD_SOUND}" "${DURATION_LIMIT}")
echo "offset is ${OFFSET} seconds"

SHIFT_AUDIO_SH="${SCRIPT_DIR}/shift-audio.sh"
"${SHIFT_AUDIO_SH}" "${GOOD_SOUND}" "${OFFSET}" "${OUTPUT}"

echo ok
