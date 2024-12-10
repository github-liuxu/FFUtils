//
//  FFWriter.hpp
//  FFUtils
//
//  Created by Mac-Mini on 2024/12/10.
//

#ifndef FFWriter_hpp
#define FFWriter_hpp

#include <stdio.h>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/mem.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
}

class FFWriter {
    
    
public:
    FFWriter();
    void OpenFile(const char *filePath);
    void Start();
    void AddVideoStream();
    void AddAudioStream();
    void WriteVideoFrame(AVFrame *frame, int64_t pts);
    void WriteAudioFrame(AVFrame *frame, int64_t pts);
    void WriteTrailer();
    ~FFWriter();
private:
    AVFormatContext *ofmt_ctx = NULL;
    const AVOutputFormat *ofmt = NULL;
    AVStream **streams;
    AVStream *video_stream = NULL, *audio_stream = NULL;
    AVPacket *pkt = NULL;
    AVCodecContext *encodec_ctx = NULL, *audio_encodec_ctx = NULL;
    const AVCodec *video_encodec = NULL, *audio_encodec = NULL;
    const char *out_filename;
};

#endif /* FFWriter_hpp */
