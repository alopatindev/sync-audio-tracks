#!/bin/bash

set -e

if [ $# -ne 3 ]; then
    echo "usage: $0 good_sound.wav bad_sound.wav shifted_good_sound.wav"
    echo "where"
    echo "  good_sound.wav - taken from audio recorder"
    echo "  bad_sound.wav - taken from camera"
    echo "  shifted_good_sound.wav - synchronized version of good_sound.wav"
    exit 1
fi

GOOD_SOUND="$1"
BAD_SOUND="$2"
OUTPUT="$3"

abs ()
{
    echo "${1#-}"
}

insert_silence ()
{
    sox --no-show-progress --no-dither "$1" "$2" pad "$3" 0
}

remove_beginning ()
{
    offset=$(abs "$3")
    sox --no-show-progress --no-dither "$1" "$2" trim "$offset"
}

COMPUTE_OFFSET_SH="$(dirname $(realpath $0))/compute-sound-offset.sh"
OFFSET=$("${COMPUTE_OFFSET_SH}" "${GOOD_SOUND}" "${BAD_SOUND}")
echo "offset is ${OFFSET} seconds"

if (( $(echo "${OFFSET} < 0" | bc -l) )) ; then
    remove_beginning "${GOOD_SOUND}" "${OUTPUT}" "${OFFSET}"
else
    insert_silence "${GOOD_SOUND}" "${OUTPUT}" "${OFFSET}"
fi

echo ok
