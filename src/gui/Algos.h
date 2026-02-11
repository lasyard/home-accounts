#ifndef _HA_GUI_ALGOS_H_
#define _HA_GUI_ALGOS_H_

#include <vector>

namespace Algos
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

template <typename T, typename Compare> int BinSearch(const std::vector<T> &arr, int target, Compare cmp)
{
    if (arr.empty()) {
        return -1;
    }
    int lo = 0;
    int hi = static_cast<int>(arr.size()) - 1;
    while (lo <= hi) {
        int mid = lo + ((hi - lo) >> 1);
        int c = cmp(arr[mid], target);
        if (c == 0) {
            return mid;
        }
        if (c < 0) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return -1;
}

} // namespace Algos

#endif /* _HA_GUI_ALGOS_H_ */
