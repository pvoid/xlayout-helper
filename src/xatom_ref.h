#pragma once

#include "xdisplay_ref.h"
#include "xwindow_ref.h"
#include "xref.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include <string>
#include <optional>

namespace com::github::pvoid::layouthelper {

class xwindow_ref_t;
class xdisplay_ref_t;

#define XA_UTF8_STRING ((Atom) 316)

class xatom_ref_t {
    friend class xwindow_ref_t;
public:
    operator std::optional<std::string>() const {
        if (atom_ == None || window_ == None || display_ == nullptr) {
            return {};
        }
        
        Atom actual_type;
        int actual_format;
        long unsigned int items, size;
        xref_t<unsigned char> value;

        auto status = XGetWindowProperty(display_, window_, atom_, 0L, sizeof(Atom), False, AnyPropertyType, 
            &actual_type, &actual_format, &items, &size, value.ref());

        if (status != Success || (actual_type != XA_STRING && actual_type != XA_UTF8_STRING)) {
            return {};
        }

        std::string result;
        result = reinterpret_cast<const char *>(static_cast<unsigned char *>(value));

        return result;
    }

    operator std::optional<u_int64_t>() const {
        if (atom_ == None || window_ == None || display_ == nullptr) {
            return {};
        }

        Atom actual_type;
        int actual_format;
        long unsigned int items, size;
        xref_t<unsigned char> value;

        auto status = XGetWindowProperty(display_, window_, atom_, 0L, sizeof(Atom), False, AnyPropertyType, 
            &actual_type, &actual_format, &items, &size, value.ref());

        if (status != Success || actual_type != XA_CARDINAL || items != 1) {
            return {};
        }

        if (actual_format == 32) {
            int32_t result = 0;
            memcpy(&result, value, 4);
            return result;
        }

        return {};
    }
private:
    xatom_ref_t(Display* display, Window window, const char* name) : display_(display), window_(window), atom_(None) {
        atom_ = XInternAtom(display, name, True);
    }

private:
    Display* display_;
    Window window_;
    Atom atom_;
};

}
