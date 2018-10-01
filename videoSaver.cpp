/*
 * Copyright (c) 2001 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

 /**
  * @file
  * video encoding with libavcodec API example
  *
  * @example encode_video.c
  */
#include "videoSaver.h"

#undef exit

#define STREAM_DURATION   5.0 /* 5 seconds stream duration */
#define STREAM_FRAME_RATE 25 /* 25 images/s */
#define STREAM_NB_FRAMES  ((int)(STREAM_DURATION * STREAM_FRAME_RATE))
#define STREAM_PIX_FMT PIX_FMT_YUV420P /* default pix_fmt */

static int sws_flags = SWS_BICUBIC;

const char *filename = "mydbg/video2.avi";
const char *filename2 = "mydbg/video2av.avi";
const char *codec_name = "libx264rgb";
const AVCodec *codec;
AVCodecID codec_id = AV_CODEC_ID_H264;
AVCodecContext *c = NULL;
AVFormatContext *avFormatContext;
AVOutputFormat *avOutputFormat;
AVStream *video_avstream;
int i, ret, x, y;
FILE *f;
AVFrame *frame;
AVPacket *pkt;
AVPacket pkt2;
uint8_t endcode[] = { 0, 0, 1, 0xb7 };
/*
	AVFormatContext::filename
	AVStream::codec
	avcodec_encode_video2
*/

//int encodeFileMk2()
//{
//	//av_register_all();
//	
//	//guess format of the output
//	avOutputFormat = av_guess_format(NULL, filename2, NULL);
//	if (!avOutputFormat) {
//		printf("Could not deduce output format from file extension: using MPEG.\n");
//		avOutputFormat = av_guess_format("mpeg", NULL, NULL);
//	}
//	if (!avOutputFormat) {
//		fprintf(stderr, "Could not find suitable output format\n");
//		exit(1);
//	}
//
//	avOutputFormat->video_codec = codec_id;
//
//	// allocate the output media context, avFormatContext
//	avFormatContext = avformat_alloc_context();
//	if (!avFormatContext) {
//		fprintf(stderr, "Memory error\n");
//		exit(1);
//	}
//	// Set the output format of the newly allocated avFormatContext to our avOutputFormat
//	avFormatContext->oformat = avOutputFormat;
//
//	// Set the output filename to the outputfile
//	snprintf(avFormatContext->filename, sizeof(avFormatContext->filename),
//		"%s", filename2);
//
//	/* find the mpeg1video encoder */
//	codec = avcodec_find_encoder(avOutputFormat->video_codec);
//	if (!codec) {
//		fprintf(stderr, "Codec '%s' not found\n", codec_name);
//		exit(1);
//	}
//
//	// add the video stream using the default format codec and initialize the codecs
//	video_avstream = avformat_new_stream(avFormatContext, codec);
//	if (!video_avstream) {
//		fprintf(stderr, "Could not alloc stream\n");
//		exit(1);
//	}
//
//	//Create video stream
//	if (video_avstream->codec == NULL) {
//		fprintf(stderr, "AVStream codec is NULL\n");
//		exit(1);
//	}
//	c = video_avstream->codec;
//	if (!c) {
//		fprintf(stderr, "Could not allocate video codec context\n");
//		exit(1);
//	}
//
//	/* put sample parameters */
//	c->bit_rate = 400000;
//	/* resolution must be a multiple of two */
//	c->width = 1280;
//	c->height = 720;
//	/* frames per second */
//	AVRational avr = { 1, 30 };
//	c->time_base = avr;
//	c->gop_size = 10; /* emit one intra frame every ten frames */
//	c->max_b_frames = 1;
//	c->pix_fmt = AV_PIX_FMT_RGB24;
//
//	if (codec_id == AV_CODEC_ID_H264)
//		av_opt_set(c->priv_data, "preset", "slow", 0);
//
//	/* open it */
//	if (avcodec_open2(c, codec, NULL) < 0) {
//		fprintf(stderr, "Could not open codec\n");
//		exit(1);
//	}
//
//	video_avstream->codec = c;
//	frame = av_frame_alloc();
//	if (!frame) {
//		fprintf(stderr, "Could not allocate video frame\n");
//		exit(1);
//	}
//	frame->format = c->pix_fmt;
//	frame->width = c->width;
//	frame->height = c->height;
//
//	/* the image can be allocated by any means and av_image_alloc() is
//	 * just the most convenient way if av_malloc() is to be used */
//	ret = av_image_alloc(frame->data, frame->linesize, c->width, c->height,
//		c->pix_fmt, 32);
//	if (ret < 0) {
//		fprintf(stderr, "Could not allocate raw picture buffer\n");
//		exit(1);
//	}
//
//	// some formats want stream headers to be separate
//	if (avFormatContext->oformat->flags & AVFMT_GLOBALHEADER)
//		c->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//
//	avformat_write_header(avFormatContext, NULL);
//
//	return 0;
//}
//
//int drawFrameMk2(unsigned int* data, int iter) {
//	av_init_packet(&pkt2);
//	pkt2.data = NULL;    // packet data will be allocated by the encoder
//	pkt2.size = 0;
//
//	fflush(stdout);
//	int got_output, write_ret;
//	/* prepare a dummy image */
//	/* Y */
//	for (y = 0; y < c->height; y++) {
//		for (x = 0; x < c->width; x++) {
//			frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
//		}
//	}
//
//	frame->pts = iter;
//
//	/* encode the image */
//	ret = avcodec_encode_video2(c, &pkt2, frame, &got_output);
//	if (ret < 0) {
//		fprintf(stderr, "Error encoding frame\n");
//		exit(1);
//	}
//
//	if (got_output) {
//		/* write the compressed frame in the media file */
//		write_ret = av_write_frame(avFormatContext, &pkt2);
//		if (write_ret < 0) {
//			fprintf(stderr, "Error writing frame\n");
//			exit(1);
//		}
//
//		av_free_packet(&pkt2);
//	}
//	else {
//		printf("got_output false: %d", i);
//	}
//	return 0;
//}
//
//int finishEncodingFileMk2() {
//	/* add sequence end code to have a real MPEG file */
//	av_write_trailer(avFormatContext);
//
//	avcodec_close(c);
//	av_free(c);
//	av_freep(&frame->data[0]);
//	av_frame_free(&frame);
//
//	/* free the streams */
//	for (i = 0; i < avFormatContext->nb_streams; i++) {
//		av_freep(&avFormatContext->streams[i]->codec);
//		av_freep(&avFormatContext->streams[i]);
//	}
//
//	if(!(avOutputFormat->flags & AVFMT_NOFILE)) {
//		/* close the output file */
//		avio_close(avFormatContext->pb);
//	}
//
//	/* free the stream */
//	av_free(avFormatContext);
//
//	//free up
//	avformat_free_context(avFormatContext);
//	avcodec_free_context(&c);
//	av_frame_free(&frame);
//	av_packet_free(&pkt);
//
//	return 0;
//}
//
//static void encodeMk2(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
//	FILE *outfile)
//{
//	int ret = 0;
//
//	/* send the frame to the encoder */
//	//TODO BROKEN PRINT if (frame)
//	//TODO BROKEN PRINT 	printf("Send frame %3"PRId64"\n", frame->pts);
//
//	ret = avcodec_send_frame(enc_ctx, frame);
//	if (ret < 0) {
//		fprintf(stderr, "Error sending a frame for encoding\n");
//		exit(1);
//	}
//
//	while (ret >= 0) {
//		ret = avcodec_receive_packet(enc_ctx, pkt);
//		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
//			return;
//		else if (ret < 0) {
//			fprintf(stderr, "Error during encoding\n");
//			exit(1);
//		}
//
//		//TODO BROKEN PRINT  printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
//		av_write_frame(avFormatContext, pkt);
//		av_packet_unref(pkt);
//	}
//}

static void encode(AVCodecContext *enc_ctx, AVFrame *frame, AVPacket *pkt,
	FILE *outfile)
{
	int ret = 0;

	/* send the frame to the encoder */
	//TODO BROKEN PRINT if (frame)
	//TODO BROKEN PRINT 	printf("Send frame %3"PRId64"\n", frame->pts);

	ret = avcodec_send_frame(enc_ctx, frame);
	if (ret < 0) {
		fprintf(stderr, "Error sending a frame for encoding\n");
		exit(1);
	}

	while (ret >= 0) {
		ret = avcodec_receive_packet(enc_ctx, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
			return;
		else if (ret < 0) {
			fprintf(stderr, "Error during encoding\n");
			exit(1);
		}

		//TODO BROKEN PRINT  printf("Write packet %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
		//av_write_frame(avContext, pkt);
		fwrite(pkt->data, 1, pkt->size, outfile);

		av_packet_unref(pkt);
	}
}

int encodeFile()
{
	/* find the mpeg1video encoder */
	codec = avcodec_find_encoder_by_name(codec_name);
	if (!codec) {
		fprintf(stderr, "Codec '%s' not found\n", codec_name);
		exit(1);
	}

	c = avcodec_alloc_context3(codec);
	if (!c) {
		fprintf(stderr, "Could not allocate video codec context\n");
		exit(1);
	}
	
	pkt = av_packet_alloc();
	if (!pkt)
		exit(1);


	/* put sample parameters */
	c->bit_rate = 40000000;
	/* resolution must be a multiple of two */
	c->width = 1280;//352;
	c->height = 720;// 288;
	/* frames per second */
	AVRational avr_time_base = { 1, 30 };
	AVRational avr_framerate = { 1, 30 };
	c->time_base = avr_time_base;
	c->framerate = avr_framerate;

	/* emit one intra frame every ten frames
	 * check frame pict_type before passing frame
	 * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
	 * then gop_size is ignored and the output of encoder
	 * will always be I frame irrespective to gop_size
	 */
	c->gop_size = 10;
	c->max_b_frames = 1;
	c->pix_fmt = AV_PIX_FMT_RGB24;// AV_PIX_FMT_YUV420P;

	if (codec->id == AV_CODEC_ID_H264)
		av_opt_set(c->priv_data, "preset", "slow", 0);
	
	/* open it */
	ret = avcodec_open2(c, codec, NULL);
	if (ret < 0) {
		fprintf(stderr, "Could not open codec.\n");
		exit(1);
	}

	//open target file
	fopen_s(&f, filename, "wb");
	if (!f) {
		fprintf(stderr, "Could not open %s\n", filename);
		exit(1);
	}

	//allocate frame for painting
	fprintf(stderr, "Frame Alloc\n");
	frame = av_frame_alloc();
	if (!frame) {
		fprintf(stderr, "Could not allocate video frame\n");
		exit(1);
	}
	frame->format = c->pix_fmt;
	frame->width = c->width;
	frame->height = c->height;

	fprintf(stderr, "xxx Dimensions %d x %d\n", c->width, c->height);

	ret = av_frame_get_buffer(frame, 30);
	if (ret < 0) {
		fprintf(stderr, "Could not allocate the video frame data\n");
		exit(1);
	}

	fprintf(stderr, "Start Encoding\n");

	return 0;
}

int drawFrame(unsigned int* data, int iter) {
	fflush(stdout);

	/* make sure the frame data is writable */
	ret = av_frame_make_writable(frame);
	if (ret < 0)
		exit(1);

	for (y = c->height - 1; y > 0; y--) {
		for (x = 0; x < c->width; x++) {
			int idx = y * c->width + x;
			int idx2 = (c->height - 1 - y) * c->width + x;
			frame->data[0][idx * 3] = (unsigned char)(data[idx2] & 0xFF);// (unsigned char)(x + y + iter * 3);
			frame->data[0][idx * 3 + 1] = (unsigned char)((data[idx2] >> 8) & 0xFF);// (unsigned char)(x + y + iter * 3);
			frame->data[0][idx * 3 + 2] = (unsigned char)((data[idx2] >> 16) & 0xFF);// (unsigned char)(x + y + iter * 3);
		}
	}
	//linesize[] For video, size in bytes of each picture line.

	frame->pts = iter;
	/* encode the image */
	encode(c, frame, pkt, f);

	return 0;
}

int finishEncodingFile() {
	/* flush the encoder */
	encode(c, NULL, pkt, f);

	/* add sequence end code to have a real MPEG file */
	fwrite(endcode, 1, sizeof(endcode), f);

	fclose(f);

	//free up
	avcodec_free_context(&c);
	av_frame_free(&frame);
	av_packet_free(&pkt);

	return 0;
}