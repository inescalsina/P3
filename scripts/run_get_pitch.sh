#!/bin/bash

# Put here the program (maybe with path)
GETF0="get_pitch"

for fwav in pitch_db/train/*.wav; do
    ff0=${fwav/.wav/.f0}
    echo "$GETF0 $fwav $ff0 $1 $2----"
    $GETF0 $fwav $ff0 $1 $2 > /dev/null || (echo "Error in $GETF0 $fwav $ff0 $1 $2"; exit 1)
done

exit 0
