//
// Created on 2024/11/22.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef MYAPPLICATION_TRANSCODE_H
#define MYAPPLICATION_TRANSCODE_H
int transcode(const char *srcPath, const char * dtsPath);
int remux(const char *inputFile, const char *outputFile);
int main11(const char *filename, const char *codec_name);
#endif //MYAPPLICATION_TRANSCODE_H
