//
// Created by yuntianming on 2024/7/2.
//

#ifndef FXFRAMETEST_ZLIBHELPER_H
#define FXFRAMETEST_ZLIBHELPER_H

#include <iostream>
#include <string>
#include <zlib.h>
#include <vector>
#include <memory>

namespace FFrame
{
// Function to compress data using zlib
    std::string compressData(const std::string& data) {
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            return "";
        }

        zs.next_in = (Bytef*)data.data();
        zs.avail_in = data.size();

        int ret;
        char outbuffer[32768];
        std::string outstring;

        do {
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = deflate(&zs, Z_FINISH);

            if (outstring.size() < zs.total_out) {
                outstring.append(outbuffer, zs.total_out - outstring.size());
            }
        } while (ret == Z_OK);

        deflateEnd(&zs);

        if (ret != Z_STREAM_END) {
            return "";
        }

        return outstring;
    }

    std::string compressDataEx(const std::string& data) {
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        std::unique_ptr<z_stream, decltype(&deflateEnd)> stream_guard(&zs, deflateEnd);

        if (deflateInit(&zs, Z_BEST_COMPRESSION) != Z_OK) {
            return "";
        }

        zs.next_in = (Bytef*)data.data();
        zs.avail_in = data.size();

        int ret;
        std::vector<char> outbuffer(data.size() * 2); // Start with an initial size double that of input data

        zs.next_out = reinterpret_cast<Bytef*>(outbuffer.data());
        zs.avail_out = outbuffer.size();

        do {
            ret = deflate(&zs, Z_FINISH);

            if (ret == Z_OK && zs.avail_out == 0) { // Increase buffer size dynamically
                outbuffer.resize(outbuffer.size() * 2);
                zs.next_out = reinterpret_cast<Bytef*>(outbuffer.data() + zs.total_out);
                zs.avail_out = outbuffer.size() - zs.total_out;
            }
        } while (ret == Z_OK);

        if (ret != Z_STREAM_END) {
            return "";
        }

        outbuffer.resize(zs.total_out); // Shrink the buffer to its actual size
        return std::string(outbuffer.begin(), outbuffer.end());
    }


    std::string decompressData(const std::string& data) {
        z_stream zs;
        memset(&zs, 0, sizeof(zs));

        if (inflateInit(&zs) != Z_OK) {
            return "";
        }

        zs.next_in = (Bytef*)data.data();
        zs.avail_in = data.size();

        int ret;
        char outbuffer[32768];
        std::string outstring;

        do {
            zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
            zs.avail_out = sizeof(outbuffer);

            ret = inflate(&zs, Z_NO_FLUSH);

            if (ret == Z_STREAM_ERROR) {
                inflateEnd(&zs);
                return "";
            }

            switch (ret) {
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    inflateEnd(&zs);
                    return "";
            }

            // Calculate the size of data decompressed in this iteration
            size_t decompressed_size = sizeof(outbuffer) - zs.avail_out;

            // Append the decompressed data to the output string
            if (decompressed_size > 0)
            {
                outstring.append(outbuffer, decompressed_size);
            }

        } while (ret != Z_STREAM_END);

        if (inflateEnd(&zs) != Z_OK) {
            return "";
        }

        return outstring;
    }

}

#endif //FXFRAMETEST_ZLIBHELPER_H
