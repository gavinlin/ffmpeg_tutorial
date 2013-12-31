#Overview

these tutorials based on ffmpeg2.1 and SDL1.2

So, before run build.sh, you need to sure that ffmpeg 2.1 and SDL1.2 have installed in your linux system.

##SDL download and install

visit [http://www.libsdl.org/download-1.2.php](http://www.libsdl.org/download-1.2.php) and download source code.
```
tar xzf SDL-1.2.15
cd SDL-1.2.15
./autogen.sh
./configure
make && sudo make install
```

##ffmpeg download and install

``` 
git clone https://github.com/FFmpeg/FFmpeg.git
git checkout remotes/origin/release/2.1
./configure
make
sudo make install  #sudo for ubuntu system
```

ffplay would not be compiled unless SDL1.2 installed.
