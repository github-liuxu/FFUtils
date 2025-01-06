//
//  main.cpp
//  FFUtils
//
//  Created by Mac-Mini on 2024/7/31.
//

#include <iostream>
#include "VideoReader.hpp"
#include "VideoWriter.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    const char* filename = "/Users/liudongxu/金晨卜卦.mp4";
    const char* dts = "/Users/liudongxu/111123.mp4";
    // transcode(filename, dts);
//    remux(filename, dts);
    remove(dts);
    VideoReader *reader = new VideoReader();
    reader->Open(filename);
    reader->SetRange(0, 1000000);
    AVFrame *frame = reader->ReadAVFrame();
    
    VideoWriter *write = new VideoWriter();
    write->OpenFile(dts);
    write->StartFileWriter(1920, 1080);
    while (true) {
        AVFrame *frame = reader->ReadAVFrame();
        if (frame == nullptr) {
            break;
        }
        bool suc = write->WriterAVFrame(frame);
    }
    write->WriterTrailer();
    delete write;

    return 0;
}
