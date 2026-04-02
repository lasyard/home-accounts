#ifndef _HA_GUI_UTILS_H_
#define _HA_GUI_UTILS_H_

#include <regex>
#include <vector>

extern "C" {
int get_line_from_istream(char *buf, size_t len, void *context);
int put_line_to_ostream(const char *buf, size_t len, void *context);
}

inline std::string EscapeRegex(const std::string &str)
{
    return std::regex_replace(str, std::regex(R"([.^$|()\[\]{}*+?\\])"), "\\$&");
}

namespace Utils
{

template <typename T> void MergeRange(std::vector<std::pair<T, T>> &input, std::vector<std::pair<T, T>> &output)
{
    if (input.empty()) {
        return;
    }
    sort(input.begin(), input.end());
    output.push_back(input[0]);
    for (size_t i = 1; i < input.size(); ++i) {
        auto &last = output.back();
        auto &curr = input[i];
        if (curr.first <= last.second) {
            last.second = std::max(last.second, curr.second);
        } else {
            output.push_back(curr);
        }
    }
}

} // namespace Utils

#endif /* _HA_GUI_UTILS_H_ */
