// -------------------------------------------------------------------------

#ifndef FX_EXCEPTION_HPP
#define FX_EXCEPTION_HPP

#include <string>
#include <cstring>
#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <cstdarg>
#include <cstdio>

class FXException {
public:
    FXException(const char *format, ...) {

        std::vector<char> buf;
        buf.reserve(1024);

        va_list args;
        va_start(args, format);

        int written = vsnprintf(nullptr, 0, format, args);
        va_end(args);

        if (written >= 0) {
            buf.resize(written + 1);
            va_start(args, format);
            vsnprintf(buf.data(), buf.size(), format, args);
            va_end(args);
        }

        message = std::string(buf.data(), buf.size() - 1);
    }

private:
    std::string message;
};

#endif
