//
//  FFReader.hpp
//  FFUtils
//
//  Created by Mac-Mini on 2024/11/27.
//

#ifndef FFReader_hpp
#define FFReader_hpp

#include <stdio.h>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavutil/channel_layout.h>
#include <libavutil/mem.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>
}

class FFReader {
public:
    FFReader();
    bool OpenFile(const char *filePath);
    void SetTimeRange(int64_t start, int64_t end);
    AVFrame* ReaderVideoFrame();
    ~FFReader();
private:
    AVFormatContext *ifmt_ctx = NULL;
    AVCodecContext *codec_ctx = NULL;
    const char *filePath = NULL;
    AVStream **streams = NULL;
    AVStream *stream = NULL;
    AVFrame *frame = NULL;
    AVPacket *packet = NULL;
    int64_t m_start = 0;
    int64_t m_end = 0;
};

#endif /* FFReader_hpp */
