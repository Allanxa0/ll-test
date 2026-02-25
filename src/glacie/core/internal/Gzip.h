#pragma once
#include "zlib.h"
#include <iostream>

namespace gzip {
inline bool decompress(const std::string& compressed_str, std::string& decompressed_str) {
    z_stream d_stream = {0}; 
    d_stream.zalloc   = Z_NULL;
    d_stream.zfree    = Z_NULL;
    d_stream.opaque   = Z_NULL;
    d_stream.avail_in = 0;
    d_stream.next_in  = Z_NULL;
    if (inflateInit2(&d_stream, MAX_WBITS + 16) != Z_OK) { return false; }

    d_stream.next_in  = (Bytef*)compressed_str.data();
    d_stream.avail_in = compressed_str.size();

    char        outbuffer[1];
    std::string outstring;

    do {
        d_stream.next_out  = reinterpret_cast<Bytef*>(outbuffer);
        d_stream.avail_out = sizeof(outbuffer);

        inflate(&d_stream, Z_NO_FLUSH);

        if (d_stream.avail_out == 0) {
            outstring.append(outbuffer, sizeof(outbuffer) - d_stream.avail_out);
            d_stream.next_out  = reinterpret_cast<Bytef*>(outbuffer);
            d_stream.avail_out = sizeof(outbuffer);
        }
    } while (d_stream.avail_in != 0);

    inflateEnd(&d_stream);

    decompressed_str.swap(outstring);
    return true;
}

inline bool compress(const std::string& original_str, std::string& str) {
    z_stream d_stream = {0};
    if (Z_OK != deflateInit2(&d_stream, Z_BEST_COMPRESSION, Z_DEFLATED, MAX_WBITS + 16, 9, Z_DEFAULT_STRATEGY)) {
        return false;
    }
    unsigned long len = compressBound(original_str.size());
    auto*         buf = (unsigned char*)malloc(len);
    if (!buf) { return false; }
    d_stream.next_in   = (unsigned char*)(original_str.c_str());
    d_stream.avail_in  = original_str.size();
    d_stream.next_out  = buf;
    d_stream.avail_out = len;
    deflate(&d_stream, Z_SYNC_FLUSH);
    deflateEnd(&d_stream);
    str.assign((char*)buf, d_stream.total_out);
    free(buf);
    return true;
}
} // namespace gzip