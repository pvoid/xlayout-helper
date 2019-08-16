#pragma once

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>

#include <string>
#include <memory>
#include <chrono>
#include <thread>

#include "xatom_ref.h"
#include "layouts_holder.h"

namespace com::github::pvoid::layouthelper {

#define WINDOW_TITLE_PROP "_NET_WM_NAME"
#define WINDOW_PROCESS_ID "_NET_WM_PID"

class xwindow_ref_t {
    friend class xdisplay_ref_t;
public:
    xwindow_ref_t(const xwindow_ref_t&) = default;
    xwindow_ref_t(xwindow_ref_t&&) = default;

    xwindow_ref_t& operator=(const xwindow_ref_t&) = default;
    xwindow_ref_t& operator=(xwindow_ref_t&&) = default;

    operator Window() const { return window_; }

    std::optional<std::string> title() const { 
        if (window_ == None) {
            return {};
        }
        
        return title_prop_;
    }
    
    std::optional<u_int64_t> pid() const { 
        if (window_ == None) {
            return {};
        }

        return pid_prop_; 
    }

    bool operator==(const xwindow_ref_t& src) const {
        return display_ == src.display_ && window_ == src.window_;
    }

    void listen_keyboard_layout(int xkb_event, layouts_holder_t& holder) {
        std::optional<u_int64_t> pid = pid_prop_;

        if (pid) {
            // Try to change layoyt if we have saved one
            auto restored = holder.get_layout_for_process(*pid);
            int layout;
            if (restored.has_value()) {
                layout = restored.value();
            } else {
                layout = holder.last();
                holder.save_layout_for_process(*pid, layout);
            }

            XkbLockGroup(display_, XkbUseCoreKbd, layout);
        }

        // Listen focus
        XSelectInput(display_, window_, FocusChangeMask);

        for(;;) {
            XEvent event;
            // Grab next event
            XNextEvent(display_, &event);

            if (event.type == xkb_event && pid != 0) {
                XkbEvent* keyboard_event = reinterpret_cast<XkbEvent*>(&event);
                if (keyboard_event->any.xkb_type == XkbStateNotify) {
                    int lang = keyboard_event->state.group;
                    if (pid) {
                        holder.save_layout_for_process(*pid, lang);
                    }
                }
                continue;
            }

            if (event.type == FocusOut) {
                return;
            }
        }
    }
private:
    xwindow_ref_t(Display* display, Window window) : display_(display), window_(window), title_prop_(display, window, WINDOW_TITLE_PROP), pid_prop_(display, window, WINDOW_PROCESS_ID) {}
private:
    Display* display_;
    Window window_;
    xatom_ref_t title_prop_;
    xatom_ref_t pid_prop_;
};

}
