#ifndef _DATA_JOINT_H_
#define _DATA_JOINT_H_

#include <functional>

template <typename T, typename S> class Joint
{
public:
    virtual ~Joint()
    {
    }

    /**
     * @brief Get target value by source value.
     *
     * @param s the source value
     * @return the target value
     */
    virtual T lookup(S s) const = 0;

    /**
     * @brief Get source value by target value.
     *
     * @param t the target value
     * @return the source value
     */
    virtual S revLookup(T t) const = 0;

    virtual void forEachTarget(std::function<bool(T *)> callback) const = 0;

    virtual void forEachSource(std::function<bool(S *)> callback) const = 0;
};

#endif /* _DATA_JOINT_H_ */
