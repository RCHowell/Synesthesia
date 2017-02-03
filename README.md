# Synethesia
The process:

1. Use ffmpeg to split videos to frames (bitmaps).

2. Loop through frames.

3. Discrete Fourier Transform (with fftw) on respective time interval for the wavefile.

4. Filter and output new frames corresponding to the audio analysis.

5. Use ffmpeg to encode an mp4 with the respective audio.

Project with end goal of generating music videos


Examples:
![Example](Successful/jellyfish.gif)
![Example](Successful/dark2.gif)
![Example](Successful/waveWarm.gif)
![Example](Successful/funkyIguana.gif)
![Example](Successful/moving.gif)

R. Conner Howell 2016
