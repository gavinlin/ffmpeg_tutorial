g++ -o tutorial01 tutorial1.cpp -D__STDC_CONSTANT_MACROS $(pkg-config --libs --cflags libavutil libavformat libavcodec libswscale)
