#include <string>
#include <iostream>
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

using namespace std;

void SaveFrame(AVFrame *pFrame, int width, int height, int iFrame) {
    FILE *pFile;
    char szFilename[32];
    int  y;

    cout<<"start sws_scale"<<endl;
    // Open file
    sprintf(szFilename, "frame%d.ppm", iFrame);
    pFile=fopen(szFilename, "wb");
    if(pFile==NULL)
    {
        cout<<"pFile is null"<<endl;
        return;
    }

    // Write header
    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    // Write pixel data
    for(y=0; y<height; y++)
        fwrite(pFrame->data[0]+y*pFrame->linesize[0], 1, width*3, pFile);

    // Close file
    fclose(pFile);
}


int main(int argc, char *argv[])
{
    if(argc <= 1){
        cout<<"you should input file name \n"<<endl;
        return 0;
    }

    string filename(argv[1]);

    av_register_all();

    AVFormatContext *pFormatCtx = NULL;

    int err = avformat_open_input(&pFormatCtx,argv[1],NULL,NULL);
    if(err < 0)
        return -1;
    err = avformat_find_stream_info(pFormatCtx, NULL);
    if(err < 0){
        return -1;
    }

    AVCodecContext *pCodecCtx;
    int videoStream = -1;
    for(unsigned int i=0; i<pFormatCtx->nb_streams; i++)
        if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO)
        {
            videoStream=i;
            break;
        }
    if(videoStream==-1)
        return -1;

    pCodecCtx=pFormatCtx->streams[videoStream]->codec;

    AVCodec *pCodec;
    pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
    if(pCodec==NULL) {
        fprintf(stderr, "Unsupported codec!\n");
        return -1; // Codec not found
    }
    // Open codec
    if(avcodec_open2(pCodecCtx, pCodec, NULL)<0)
        return -1; // Could not open codec

    AVFrame *pFrame;
    pFrame=av_frame_alloc();
    AVFrame *pFrameRGB;
    pFrameRGB=av_frame_alloc();
    if(pFrameRGB==NULL)
        return -1;

    uint8_t *buffer;
    int numBytes;
    numBytes=avpicture_get_size(PIX_FMT_RGB24, pCodecCtx->width,
            pCodecCtx->height);
    buffer=(uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    avpicture_fill((AVPicture *)pFrameRGB, buffer, PIX_FMT_RGB24,
            pCodecCtx->width, pCodecCtx->height);

    int frameFinished;
    AVPacket packet;
    struct SwsContext *img_convert_ctx;

    int i=0;
    while(av_read_frame(pFormatCtx, &packet)>=0) {
        // Is this a packet from the video stream?
        if(packet.stream_index==videoStream) {
            // Decode video frame
            int result;
            avcodec_decode_video2(pCodecCtx,pFrame,&frameFinished,
                    &packet);

            // Did we get a video frame?
            if(frameFinished) {
                img_convert_ctx = sws_getContext(pCodecCtx->width,
                        pCodecCtx->height,
                        pCodecCtx->pix_fmt,
                        pCodecCtx->width,
                        pCodecCtx->height,
                        PIX_FMT_RGB24,
                        SWS_BICUBIC,NULL,
                        NULL,NULL);
                result = sws_scale(img_convert_ctx,
                        (const uint8_t* const*)pFrame->data,
                        pFrame->linesize,
                        0,
                        pCodecCtx->height,
                        pFrameRGB->data,
                        pFrameRGB->linesize);
                printf("get result is %d~~~~~~~~~\n",result);
                // Save the frame to disk
                cout<<"i is "<<i<<endl;
                if(++i<=5)
                    SaveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height,
                            i);
                else if(i > 5)
                    break;
            }
        }

        // Free the packet that was allocated by av_read_frame
        av_free_packet(&packet);
    }

    av_free(buffer);
    av_free(pFrameRGB);

    // Free the YUV frame
    av_free(pFrame);

    // Close the codec
    avcodec_close(pCodecCtx);

    // Close the video file
    avformat_close_input(&pFormatCtx);

    return 0;
}
