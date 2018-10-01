#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
\
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
//
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
}

static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
	FILE *outfile);

static void encodeMk2(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
	FILE *outfile);

int encodeFile();
int drawFrame(unsigned int* data, int iter);
int finishEncodingFile();

int encodeFileMk2();
int drawFrameMk2(unsigned int* data, int iter);
int finishEncodingFileMk2();