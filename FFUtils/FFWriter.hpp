//
//  FFWriter.hpp
//  FFUtils
//
//  Created by Mac-Mini on 2024/12/10.
//

#ifndef FFWriter_hpp
#define FFWriter_hpp

#include <stdio.h>
//extern "C" {
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libavutil/mem.h>
//#include <libavutil/opt.h>
//#include <libavutil/pixdesc.h>
//}
//
//class FFWriter {
//    
//    
//public:
//    FFWriter();
//    void OpenFile(const char *filePath);
//    void Start();
//    void AddVideoStream();
//    void AddAudioStream();
//    void WriteVideoFrame(AVFrame *frame, int64_t pts);
//    void WriteAudioFrame(AVFrame *frame, int64_t pts);
//    void WriteTrailer();
//    ~FFWriter();
//private:
//    AVFormatContext *ofmt_ctx = NULL;
//    const AVOutputFormat *ofmt = NULL;
//    AVStream **streams;
//    AVStream *video_stream = NULL, *audio_stream = NULL;
//    AVPacket *pkt = NULL;
//    AVCodecContext *encodec_ctx = NULL, *audio_encodec_ctx = NULL;
//    const AVCodec *video_encodec = NULL, *audio_encodec = NULL;
//    const char *out_filename;
//};

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

#include <iostream>
#include <stdexcept>

class FFWriter {
private:
    AVFormatContext* formatContext = nullptr;
    AVCodecContext* codecContext = nullptr;
    AVStream* videoStream = nullptr;
    const AVCodec* codec = nullptr;
    bool isFileOpen = false;

public:
    FFWriter() {
//        av_register_all();
    }

    ~FFWriter() {
        if (isFileOpen) {
            WriteTrailer();
        }
        if (codecContext) {
            avcodec_free_context(&codecContext);
        }
        if (formatContext) {
            avformat_free_context(formatContext);
        }
    }

    void OpenFile(const char* filePath) {
        // Create format context
        if (avformat_alloc_output_context2(&formatContext, nullptr, nullptr, filePath) < 0) {
            throw std::runtime_error("Failed to allocate format context.");
        }

        // Open the output file
        if (!(formatContext->oformat->flags & AVFMT_NOFILE)) {
            if (avio_open(&formatContext->pb, filePath, AVIO_FLAG_WRITE) < 0) {
                throw std::runtime_error("Failed to open output file.");
            }
        }

        isFileOpen = true;
    }

    void AddVideoStream() {
        // Find the encoder
        codec = avcodec_find_encoder(AV_CODEC_ID_H264);
        if (!codec) {
            throw std::runtime_error("H.264 encoder not found.");
        }

        // Create the video stream
        videoStream = avformat_new_stream(formatContext, codec);
        if (!videoStream) {
            throw std::runtime_error("Failed to create video stream.");
        }

        // Allocate codec context
        codecContext = avcodec_alloc_context3(codec);
        if (!codecContext) {
            throw std::runtime_error("Failed to allocate codec context.");
        }

        // Set codec parameters
        codecContext->codec_id = AV_CODEC_ID_H264;
        codecContext->bit_rate = 400000;
        codecContext->width = 1920;
        codecContext->height = 1080;
        codecContext->time_base = {1, 30}; // FPS
        codecContext->framerate = {30, 1};
        codecContext->gop_size = 10;
        codecContext->max_b_frames = 1;
        codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

        if (formatContext->oformat->flags & AVFMT_GLOBALHEADER) {
            codecContext->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
        }

        // Open the codec
        if (avcodec_open2(codecContext, codec, nullptr) < 0) {
            throw std::runtime_error("Failed to open codec.");
        }

        // Copy codec parameters to stream
        if (avcodec_parameters_from_context(videoStream->codecpar, codecContext) < 0) {
            throw std::runtime_error("Failed to copy codec parameters to stream.");
        }

        videoStream->time_base = codecContext->time_base;
    }

    void WriteVideoFrame(AVFrame* frame, int64_t pts) {
        if (!isFileOpen) {
            throw std::runtime_error("Output file is not open.");
        }

        frame->pts = pts;

        AVPacket *pkt = av_packet_alloc();
        int ret = avcodec_send_frame(codecContext, frame);
        if (ret < 0) {
            throw std::runtime_error("Error sending frame to encoder.");
        }
        ret = avcodec_receive_packet(codecContext, pkt);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return; // No packet ready yet
        } else if (ret < 0) {
            throw std::runtime_error("Error receiving packet from encoder.");
        }

        pkt->stream_index = videoStream->index;
        pkt->pts = av_rescale_q(pkt->pts, codecContext->time_base, videoStream->time_base);
        pkt->dts = av_rescale_q(pkt->dts, codecContext->time_base, videoStream->time_base);
        pkt->duration = av_rescale_q(pkt->duration, codecContext->time_base, videoStream->time_base);

        if (av_interleaved_write_frame(formatContext, pkt) < 0) {
            throw std::runtime_error("Error writing packet.");
        }

        av_packet_unref(pkt);
    }

    void WriteTrailer() {
        if (isFileOpen) {
            av_write_trailer(formatContext);
            if (!(formatContext->oformat->flags & AVFMT_NOFILE)) {
                avio_close(formatContext->pb);
            }
            isFileOpen = false;
        }
    }
};



#endif /* FFWriter_hpp */
