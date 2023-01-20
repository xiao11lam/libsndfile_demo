#include <stdlib.h>
#include <stdio.h>
#include "sndfile.h"
#include <memory.h>
#include <ostream>
#include <iostream>
#include "sndfile.hh"
using namespace std;


#define RAW_BUFF_SIZE 1024

int main(int argv, const char *args[]) {
    if (argv!=2) {
        printf("please input wav file\n");
        exit(0);
    }

    SF_INFO info;

    SNDFILE *in = sf_open(args[1], SFM_READ, &info);
    if (!in) {
        fprintf(stderr, "cannot open file %s\n", args[1]);
        return 1;
    }


    char *p = const_cast<char *>(strrchr(args[1], '.'));
    int len = p-args[1];
    char *output_file = static_cast<char *>(calloc(len + 5, sizeof(char)));

    strncpy(output_file, args[1], len);
    strcpy(output_file+len, ".pcm");

    SF_INFO onfo;
    onfo.channels = 1;
    onfo.samplerate = info.samplerate;
    onfo.format = SF_FORMAT_RAW | SF_FORMAT_PCM_16 ;
    onfo.sections = 0;
    onfo.seekable = 1;
    SNDFILE *out = sf_open(output_file, SFM_WRITE, &onfo);
    if (!out) {
        fprintf(stderr, "cannot open file %s\n", output_file);
        sf_close(in);
        return 2;
    }

    free(output_file);

    char buff[RAW_BUFF_SIZE];
    sf_count_t read = 0;
    do {
        read = sf_read_raw(in, buff, RAW_BUFF_SIZE);
        if (read >0) {
            sf_write_raw(out, buff, read);
        }
    } while (read >0);

    sf_close(in);
    sf_close(out);

///////////////////////////////// Read_File /////////////////////////////
    SndfileHandle snd(args[1]);
    //【1】获取文件信息
    cout << "帧长：" << snd.frames() << endl;//帧数为每个通道单独的帧数之和
    // 帧长和matalab读取的一样。
    cout << "通道数:" << snd.channels() << endl;
    cout << "格式信息：" << snd.format() << endl; //格式的枚举分量参见文档api部分

    //【2】读取PCM全部数据
    //pcm-16每个通道每帧为一个short的长度
    short *buffer = new short[snd.frames()*snd.channels()]; //帧数为每个通道单独的帧数之和
    int read_count = 0;
    while (read_count < snd.frames()*snd.channels()) {
        //注意读取的长度不要超过缓冲区大小，避免越界
        const int read_limit = (snd.frames()*snd.channels() - read_count) > 1024
                               ? 1024
                               : (snd.frames()*snd.channels() - read_count);
        const int read_len = snd.read(buffer+read_count, read_limit);
        read_count += read_len;
    }
    cout << "read count(short):" << read_count << endl;
///////////////////////////////// Write_File An Empty /////////////////////////////

    static const int Buffer_Len = 1024 * 10+100;
    const char *filepath = "create_pcm16.wav";
    const int format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    const int channels = 2;
    const int samplerate = 11025;

    SndfileHandle write_snd(filepath, SFM_WRITE, format,channels,samplerate);

    const short write_buffer[Buffer_Len*channels] = {1};
    int write_count = 0;
    while (write_count < Buffer_Len*channels) {
        const int write_limit = (Buffer_Len*channels - write_count) > 1024
                                ? 1024
                                : (Buffer_Len*channels - write_count);
        const int write_len = write_snd.write(write_buffer, write_limit);
        write_count += write_len;
    }

//    system("pause");
    return 0;
//    system("pause");
//    return 0;
}
