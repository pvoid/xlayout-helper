#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>

#include <csignal>

#include <iostream>
#include <iomanip>
#include <cstring>
#include <thread>
#include <chrono>

#include "xdisplay_ref.h"
#include "layouts_holder.h"

using com::github::pvoid::layouthelper::xref_t;
using com::github::pvoid::layouthelper::xatom_ref_t;
using com::github::pvoid::layouthelper::xdisplay_ref_t;
using com::github::pvoid::layouthelper::xwindow_ref_t;
using com::github::pvoid::layouthelper::layouts_holder_t;

int xerror_handler(Display* display, XErrorEvent* event) {
    static char buffer[1024];
    if (event) {
        XGetErrorText(display, event->error_code, buffer, sizeof(buffer));
        std::cout << "Error event: " << buffer << std::endl;
    }
    return 0;
}

int main(int argc, char** argv) {
    XSetErrorHandler(&xerror_handler);
    
    xdisplay_ref_t display;

    int kb_event_type = display.fetch_kb_event_type();
    if (kb_event_type == None) {
        return -1;
    }

    layouts_holder_t layout_holder;

    for(;;) {
        auto focus = display.focus_window();
        if (focus) {
            focus->listen_keyboard_layout(kb_event_type, layout_holder);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    return 0;
}