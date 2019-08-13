#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>
#include <X11/Xatom.h>

#include <csignal>

#include <iostream>
#include <iomanip>
#include <cstring>

#include "xdisplay_ref.h"
#include "layouts_holder.h"

using com::github::pvoid::layouthelper::xref_t;
using com::github::pvoid::layouthelper::xatom_ref_t;
using com::github::pvoid::layouthelper::xdisplay_ref_t;
using com::github::pvoid::layouthelper::layouts_holder_t;

volatile sig_atomic_t do_work = true;

int main(int argc, char** argv) {
    xdisplay_ref_t display;

    int kb_event_type = display.fetch_kb_event_type();
    if (kb_event_type == None) {
        return -1;
    }

    layouts_holder_t layout_holder;

    while(do_work) {
        auto focus = display.focus_window();
        focus.listen_keyboard_layout(kb_event_type, layout_holder);
    }

    return 0;
}