#pragma once

#include <X11/X.h>

namespace com::github::pvoid::layouthelper {

template<typename T>
class xref_t {
public:
    xref_t() : value_(nullptr) {}
    xref_t(T* value) : value_(value) {}
    ~xref_t() {
        if (value_ != nullptr) {
            XFree(value_);
        }
    }
    operator T*() const { return value_; }

    T** ref() { return &value_; }
private:
    T* value_;
};

}