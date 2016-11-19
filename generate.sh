#!/bin/bash

# clear frames folder
rm ./frames/*
rm ./output/*
# take video .mp4 and generate frames (60fps)
# output frames to frames directory
echo "Generating Frames..."
ffmpeg -i ./video_cheetah.mp4 -r 60 ./frames/frame-%06d.bmp

# counter to increment output filenames
COUNTER=1
THRESHOLD=450
INCREASE=0
PINCREASE=1
PORTION=1
# for each file in frames directory, run the filter
echo "Running Filter..."
for f in ./frames/*; do
    # create file name of format "out-xxxxxxx.bmp"
    outFile=$(printf "out-%07d.bmp" $COUNTER)
    # run filter for each frame
    # square filter version ./filter "$f" "./output/$outFile.bmp" $THRESHOLD $PORTION
    ./filter "$f" "./output/$outFile.bmp" $THRESHOLD
    let COUNTER=COUNTER+1
    
    # update threshold
    if [ $THRESHOLD -lt 400 ]
    then
	    let INCREASE=1
    elif [ $THRESHOLD -gt 1400 ]
    then
	    let INCREASE=0
    else
        let INCREASE=INCREASE
    fi

    if [ $INCREASE -eq 1 ]
    then
        let THRESHOLD=THRESHOLD+8
    else
        let THRESHOLD=THRESHOLD-8
    fi
    # done updating threshold


    if [ $PORTION -lt 3 ]
    then
	    let PINCREASE=1
    elif [ $PORTION -gt 100 ]
    then
	    let PINCREASE=0
    else
        let PINCREASE=PINCREASE
    fi

    if [ $PINCREASE -eq 1 ]
    then
        let PORTION=PORTION+2
    else
        let PORTION=PORTION-2
    fi
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