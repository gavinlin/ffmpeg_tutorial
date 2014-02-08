g++ -o tutorial01 tutorial1.cpp -D__STDC_CONSTANT_MACROS $(pkg-config --libs --cflags libavutil libavformat libavcodec libswscale)
g++ -o tutorial02 tutorial2.cpp -D__STDC_CONSTANT_MACROS $(pkg-config --libs --cflags libavutil libavformat libavcodec libswscale sdl)
g++ -o tutorial03 tutorial3.cpp -D__STDC_CONSTANT_MACROS $(pkg-config --libs --cflags libavutil libavformat libavcodec libswscale sdl) -fpermissive
