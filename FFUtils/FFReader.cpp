//
//  FFReader.cpp
//  FFUtils
//
//  Created by Mac-Mini on 2024/11/27.
//

#include "FFReader.hpp"

FFReader::FFReader() {
    
}

bool FFReader::OpenFile(const char *filePath) {
    this->filePath = filePath;
    int ret = 0;
    if ((ret = avformat_open_input(&ifmt_ctx, filePath, 0, 0)) < 0) {
        fprintf(stderr, "Could not open input file '%s'", filePath);
        return false;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, NULL)) < 0) {
        fprintf(stderr, "Failed to retrieve input stream information");
        return false;
    }
    const struct AVCodec *decode = NULL;
    if (av_find_best_stream(ifmt_ctx, AVMEDIA_TYPE_VIDEO, -1, -1, &decode, 0) < 0) {
        fprintf(stderr, "Failed to av_find_best_stream");
        return false;
    }
    
    streams = ifmt_ctx->streams;
    for (int i = 0; i < ifmt_ctx->nb_streams; i++) {
        AVStream *stream = ifmt_ctx->streams[i];
        if (stream->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            this->stream = stream;
            break;
        }
    }
    if (stream == NULL) {
        stream = streams[0];
    }
    if (decode == NULL) {
        decode = avcodec_find_decoder(stream->codecpar->codec_id);
    }
    
    codec_ctx = avcodec_alloc_context3(decode);
    if (!codec_ctx) {
        fprintf(stderr, "Failed to avcodec_alloc_context3");
        return false;
    }
    ret = avcodec_parameters_to_context(codec_ctx, stream->codecpar);
    if (ret < 0) {
        fprintf(stderr, "Failed to avcodec_parameters_to_context");
        return false;
    }
    codec_ctx->pkt_timebase = stream->time_base;
    if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO) {
        codec_ctx->framerate = av_guess_frame_rate(ifmt_ctx, stream, NULL);
    }
    /* Open decoder */
    ret = avcodec_open2(codec_ctx, decode, NULL);
    if (ret < 0) {
        fprintf(stderr, "Failed to avcodec_open2");
        return false;
    }
    
    if (!packet) {
        packet = av_packet_alloc();
        if (!packet) {
            fprintf(stderr, "Could not allocate AVPacket\n");
        }
    } else {
        av_packet_unref(packet);
    }
    
    return true;
}

void FFReader::SetTimeRange(int64_t start, int64_t end) {
    m_start = start;
    m_end = end;
}

AVFrame* FFReader::ReaderVideoFrame() {
    AVFrame *frame = NULL;
    while (true) {
        AVPacket *packet = av_packet_alloc();
        int ret = av_read_frame(ifmt_ctx, packet);
        if (packet->stream_index != 0) {
            continue;
        }
        if (ret < 0) {
            av_packet_unref(packet);
            av_packet_free(&packet);
            return nullptr;
        }
        ret = avcodec_send_packet(codec_ctx, packet);
        if (ret == AVERROR(EAGAIN)) {
            av_packet_unref(packet);
            av_packet_free(&packet);
            continue;
        }
        if (ret == AVERROR_EOF || ret != 0) {
            av_packet_unref(packet);
            av_packet_free(&packet);
            return nullptr;
        }
        frame = av_frame_alloc();
        ret = avcodec_receive_frame(codec_ctx, frame);
        if (ret == 0) {
            break;
        }
        if (ret == AVERROR(EAGAIN)) {
            av_frame_unref(frame);
            av_frame_free(&frame);
            continue;
        }
        if (ret == AVERROR_EOF || ret != 0) {
            av_frame_unref(frame);
            av_frame_free(&frame);
            return nullptr;
        }
    }
    return frame;
}

FFReader::~FFReader() {
    avformat_close_input(&ifmt_ctx);
    ifmt_ctx = NULL;
}
