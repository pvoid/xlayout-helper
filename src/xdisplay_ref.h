#pragma once

#include "xwindow_ref.h"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

namespace com::github::pvoid::layouthelper {

class xdisplay_ref_t {
public:
    xdisplay_ref_t() : display_(XOpenDisplay(nullptr)), kb_event_type_(None) {}
    xdisplay_ref_t(const xdisplay_ref_t&) = delete;
    xdisplay_ref_t(xdisplay_ref_t&&) = delete;

    ~xdisplay_ref_t() { 
        if (display_ != None) {
            XCloseDisplay(display_);
            display_ = None;
        }
    }

    xdisplay_ref_t& operator=(const xdisplay_ref_t&) = delete;
    xdisplay_ref_t& operator=(xdisplay_ref_t&&) = delete;

    int fetch_kb_event_type() {
        if (kb_event_type_ != None) {
            return kb_event_type_;
        }

        if (XkbQueryExtension(display_, nullptr, &kb_event_type_, nullptr, nullptr, nullptr) == False) {
            return None;
        }

        XkbSelectEventDetails(display_, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask, XkbGroupStateMask);

        return kb_event_type_;
    }

    xwindow_ref_t root_window() const {
        return xwindow_ref_t(display_, DefaultRootWindow(display_)); 
    }

    xwindow_ref_t focus_window() const { 
        Window focus;
        int revert;

        XGetInputFocus(display_, &focus, &revert);

        return xwindow_ref_t(display_, focus);
    }

private:
    Display* display_;
    int kb_event_type_;
};

}
