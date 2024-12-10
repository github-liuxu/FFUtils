//
//  main.cpp
//  FFUtils
//
//  Created by Mac-Mini on 2024/7/31.
//

#include <iostream>
#include "FFReader.hpp"
#include "FFWriter.hpp"
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include "transcode.h"
#include <libavutil/mem.h>
#include <libavutil/timestamp.h>
#include <libavformat/avformat.h>
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    const char* filename = "/Users/liudongxu/金晨卜卦.mp4";
    const char* dts = "/Users/liudongxu/123.mp4";
//    transcode(filename, dts);
//    remux(filename, dts);
    
    FFReader *reader = new FFReader();
    reader->OpenFile(filename);
    AVFrame *frame = reader->ReaderVideoFrame();
    
    FFWriter *write = new FFWriter();
    write->OpenFile(dts);
    write->AddVideoStream();
//    write->Start();
    write->WriteVideoFrame(frame, 0);
    write->WriteTrailer();
    
//    const AVOutputFormat *ofmt = NULL;
//    AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;
//    AVPacket *pkt = NULL;
//    const char *in_filename, *out_filename;
//
//    in_filename  = filename;
//    out_filename = dts;
//
//    pkt = av_packet_alloc();
//    if (!pkt) {
//        fprintf(stderr, "Could not allocate AVPacket\n");
//        return 1;
//    }
//    int ret = 0;
//    if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) {
//        fprintf(stderr, "Could not open input file '%s'", in_filename);
//    }
//
//    if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) {
//        fprintf(stderr, "Failed to retrieve input stream information");
//    }
//    AVStream *stream = ifmt_ctx->streams[0];
//    const AVCodec *dec = avcodec_find_decoder(stream->codecpar->codec_id);
//    AVCodecContext *codec_ctx, *encodec_ctx;
//    if (!dec) {
//        fprintf(stderr, "Failed to find decoder for stream 0\n");
//    }
//    codec_ctx = avcodec_alloc_context3(dec);
//    if (!codec_ctx) {
//        fprintf(stderr, "Failed to allocate the decoder context for stream 0\n");
//    }
//    ret = avcodec_open2(codec_ctx, dec, NULL);
//    if (ret < 0) {
//        printf("aaa");
//    }
//    ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
//    if (ret < 0) {
//        fprintf(stderr, "Failed to copy decoder parameters to input decoder context "
//               "for stream 0\n");
//        return ret;
//    }
//
//    /* Inform the decoder about the timebase for the packet timestamps.
//     * This is highly recommended, but not mandatory. */
//    codec_ctx->pkt_timebase = stream->time_base;
//    //av_dump_format(ifmt_ctx, 0, in_filename, 0);
//
//    avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
//    if (!ofmt_ctx) {
//        fprintf(stderr, "Could not create output context\n");
//        ret = AVERROR_UNKNOWN;
//    }
//
//    ofmt = ofmt_ctx->oformat;
//
//    AVStream *out_stream;
//    AVStream *in_stream = ifmt_ctx->streams[0];
//    AVCodecParameters *in_codecpar = in_stream->codecpar;
//    const AVCodec *endec = avcodec_find_decoder(AV_CODEC_ID_HEVC);
//    out_stream = avformat_new_stream(ofmt_ctx, endec);
//    if (!out_stream) {
//        fprintf(stderr, "Failed allocating output stream\n");
//        ret = AVERROR_UNKNOWN;
//    }
//    
//    encodec_ctx = avcodec_alloc_context3(endec);
//    if (!encodec_ctx) {
//        fprintf(stderr, "Failed to allocate the decoder context for stream 0\n");
//    }
//    ret = avcodec_open2(encodec_ctx, endec, NULL);
//    if (ret < 0) {
//        printf("aaa");
//    }
//    ret = avcodec_parameters_from_context(out_stream->codecpar, encodec_ctx);
//    if (ret < 0) {
//        fprintf(stderr, "Failed to copy decoder parameters to input decoder context "
//               "for stream 0\n");
//        return ret;
//    }
//
//    ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
//    if (ret < 0) {
//        fprintf(stderr, "Could not open output file '%s'", out_filename);
//    }
//
//    ret = avformat_write_header(ofmt_ctx, NULL);
//    if (ret < 0) {
//        fprintf(stderr, "Error occurred when opening output file\n");
//    }
//    int frameIndex = 0;
//    while (1) {
//        AVStream *in_stream, *out_stream;
//        ret = av_read_frame(ifmt_ctx, pkt);
//        if (ret < 0)
//            break;
//
//        in_stream  = ifmt_ctx->streams[0];
//        if (pkt->stream_index != 0) {
//            av_packet_unref(pkt);
//            continue;
//        }
//        
//        if (frameIndex == 1) {
//            break;
//        }
//        
//        ret = avcodec_send_packet(codec_ctx, pkt);
//        if (ret < 0) {
//            break;
//        }
//        AVFrame *frame = av_frame_alloc();
//        ret = avcodec_receive_frame(codec_ctx, frame);
//        if (ret < 0) {
//            break;
//        }
//        
//        ret = avcodec_send_frame(encodec_ctx, frame);
//        if (ret < 0) {
//            break;
//        }
//        
//        ret = avcodec_receive_packet(encodec_ctx, pkt);
//        if (ret < 0) {
//            break;
//        }
//        
//        pkt->stream_index = 0;
//        out_stream = ofmt_ctx->streams[pkt->stream_index];
//
//        /* copy packet */
//        av_packet_rescale_ts(pkt, in_stream->time_base, out_stream->time_base);
//        pkt->pos = -1;
//
//        ret = av_interleaved_write_frame(ofmt_ctx, pkt);
//        frameIndex++;
//        /* pkt is now blank (av_interleaved_write_frame() takes ownership of
//         * its contents and resets pkt), so that no unreferencing is necessary.
//         * This would be different if one used av_write_frame(). */
//        if (ret < 0) {
//            fprintf(stderr, "Error muxing packet\n");
//            break;
//        }
//    }
//
//    av_write_trailer(ofmt_ctx);
//
//    av_packet_free(&pkt);
//
//    avformat_close_input(&ifmt_ctx);
//
//    /* close output */
//    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
//        avio_closep(&ofmt_ctx->pb);
//    avformat_free_context(ofmt_ctx);
//
//    if (ret < 0 && ret != AVERROR_EOF) {
//        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
//        return 1;
//    }
    
    return 0;
}
