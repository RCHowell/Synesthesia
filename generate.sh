#!/bin/bash

# clear frames folder
rm ./frames/*
rm ./output/*
# take video .mp4 and generate frames (60fps)
# output frames to frames directory
echo "Generating Frames..."
ffmpeg -i ./videos/jellyfish.mp4 -r 20 ./frames/frame-%06d.bmp

# counter to increment output filenames
COUNTER=1
THRESHOLD=0
INCREASE=0
# for each file in frames directory, run the filter
echo "Running Filter..."
for f in ./frames/*; do
    # create file name of format "out-xxxxxxx.bmp"
    outFile=$(printf "out-%07d.bmp" $COUNTER)
    # run filter for each frame
    # square filter version ./filter "$f" "./output/$outFile.bmp" $THRESHOLD $PORTION
    ./2.0/filter "$f" "./output/$outFile" 20
    let COUNTER=COUNTER+1
    
    # update threshold
    if [ $THRESHOLD -lt 0 ]
    then
	    let INCREASE=1
    elif [ $THRESHOLD -gt 250 ]
    then
	    let INCREASE=0
    else
        let INCREASE=INCREASE
    fi

    if [ $INCREASE -eq 1 ]
    then
        let THRESHOLD=THRESHOLD+1
    else
        let THRESHOLD=THRESHOLD-1
    fi
    # done updating threshold
done
echo "Filter done"
# convert output frames to gif
echo "Generating GIF..."
convert -delay 3 -loop 0 ./output/*.bmp result.gif
echo "GIF done"

# clean frames and output directory
rm ./frames/*
rm ./output/*

echo "Done"