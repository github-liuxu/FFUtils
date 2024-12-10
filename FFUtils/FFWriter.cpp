//
//  FFWriter.cpp
//  FFUtils
//
//  Created by Mac-Mini on 2024/12/10.
//

#include "FFWriter.hpp"

FFWriter::FFWriter() {
    
}

void FFWriter::OpenFile(const char *filePath) {
    out_filename = filePath;
    const char *format_name = NULL;
    int ret = avformat_alloc_output_context2(&ofmt_ctx, NULL, format_name, filePath);
    if (!ofmt_ctx) {
        fprintf(stderr, "Could not create output context\n");
        return;
    }
    if (ret < 0) {
        if (!format_name) {
            ret = avformat_alloc_output_context2(&ofmt_ctx, nullptr, "mp4", filePath);
        }
    }
}

void FFWriter::Start() {
    encodec_ctx = avcodec_alloc_context3(video_encodec);
    AVCodecParameters *codecpar = video_stream->codecpar;
    if (!encodec_ctx) {
        fprintf(stderr, "Failed to allocate the decoder context for stream\n");
    }
    codecpar->width = 1280;
    codecpar->height = 720;
    codecpar->format = AV_PIX_FMT_YUV420P;
    int ret = avcodec_open2(encodec_ctx, video_encodec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Failed to avcodec_open2\n");
    }
    
    ret = avcodec_parameters_from_context(video_stream->codecpar, encodec_ctx);
    if (ret < 0) {
        fprintf(stderr, "Failed to copy decoder parameters to input decoder context "
               "for stream 0\n");
    }
    
    
    audio_encodec_ctx = avcodec_alloc_context3(audio_encodec);
    if (!audio_encodec_ctx) {
        fprintf(stderr, "Failed to allocate the decoder context for stream 1\n");
    }
    ret = avcodec_open2(audio_encodec_ctx, audio_encodec, NULL);
    if (ret < 0) {
        fprintf(stderr, "Failed to avcodec_open2 audio_encodec_ctx\n");
    }
    
    ret = avcodec_parameters_from_context(audio_stream->codecpar, audio_encodec_ctx);
    if (ret < 0) {
        fprintf(stderr, "Failed to copy decoder parameters to input decoder context "
               "for stream\n");
    }

    if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) {
        ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
        if (ret < 0) {
            fprintf(stderr, "Could not open output file '%s'", out_filename);
        }
    }

    av_dump_format(ofmt_ctx, 0, out_filename, 1);
    ret = avformat_write_header(ofmt_ctx, NULL);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file\n");
    }
}

void FFWriter::AddVideoStream() {
    video_encodec = avcodec_find_decoder(AV_CODEC_ID_H264);
    video_stream = avformat_new_stream(ofmt_ctx, video_encodec);
    if (!video_stream) {
        fprintf(stderr, "Failed allocating output video stream\n");
        return;
    }
}

void FFWriter::AddAudioStream() {
    audio_encodec = avcodec_find_decoder(AV_CODEC_ID_AAC);
    audio_stream = avformat_new_stream(ofmt_ctx, audio_encodec);
    if (!audio_stream) {
        fprintf(stderr, "Failed allocating output audio stream\n");
        return;
    }
}

void FFWriter::WriteVideoFrame(AVFrame *frame, int64_t pts) {
    
}

void FFWriter::WriteTrailer() {
    
}

FFWriter::~FFWriter() {
    avformat_free_context(ofmt_ctx);
    ofmt_ctx = NULL;
}
