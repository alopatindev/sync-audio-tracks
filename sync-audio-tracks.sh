#!/bin/env bash

set -e

if [ $# -ne 3 ]; then
    echo "usage: $0 good_sound.wav bad_sound.wav output.wav"
    echo "where"
    echo "  good_sound.wav - taken from audio recorder"
    echo "  bad_sound.wav - taken from camera"
    exit 1
fi

GOOD_SOUND="$1"
BAD_SOUND="$2"
OUTPUT="$3"

downsample_and_cut ()
{
    ffmpeg -y -hide_banner -loglevel error -i "$1" -ab 8 -ar 8000 -t 900 -map 0:a -ac 1 -vn "$2"
}

TMPDIR=$(mktemp -d /var/tmp/tmp.XXXXXXXXXX)
COMPUTE_OFFSET="$(dirname $0)/compute_sound_offset"

TMP_GOOD_SOUND="${TMPDIR}/${GOOD_SOUND}.wav"
TMP_BAD_SOUND="${TMPDIR}/${BAD_SOUND}.wav"
downsample_and_cut "${GOOD_SOUND}" "${TMP_GOOD_SOUND}"
downsample_and_cut "${BAD_SOUND}" "${TMP_BAD_SOUND}"

OFFSET=$(${COMPUTE_OFFSET} "${TMP_GOOD_SOUND}" "${TMP_BAD_SOUND}")
echo "offset is ${OFFSET} seconds"
sox --no-show-progress --no-dither "${GOOD_SOUND}" "${OUTPUT}" pad "${OFFSET}" 0

rm -rfv "${TMPDIR}"
