#pragma once

#include <map>
#include <optional>

#include <sys/types.h>

namespace com::github::pvoid::layouthelper {

class layouts_holder_t {
public:
    layouts_holder_t() : last_layout_(0) {}
    
    std::optional<int> get_layout_for_process(u_int64_t pid) const {
        auto item = layouts_.find(pid);
        if (item == std::end(layouts_)) {
            return std::optional<int>();
        }
        return std::optional(item->second);
    }

    void save_layout_for_process(u_int64_t pid, int layout) {
        layouts_[pid] = layout;
        last_layout_ = layout;
    }

    int last() const { return last_layout_; }
private:
    std::map<u_int64_t, int> layouts_;
    int last_layout_;
};

}
