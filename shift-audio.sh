#!/bin/bash

set -e

if [ $# -ne 3 ]; then
    echo "usage: $0 input.wav offset output.wav"
    echo "where"
    echo "  offset is time in seconds (positive or negative number)"
    exit 1
fi

INPUT="$1"
OFFSET="$2"
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

if (( $(echo "${OFFSET} < 0" | bc -l) )) ; then
    remove_beginning "${INPUT}" "${OUTPUT}" "${OFFSET}"
else
    insert_silence "${INPUT}" "${OUTPUT}" "${OFFSET}"
fi
