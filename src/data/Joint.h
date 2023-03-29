#ifndef _DATA_JOINT_H_
#define _DATA_JOINT_H_

#include <functional>

#include "TypeTraits.h"

template <typename DT, typename ST> class Joint
{
    typedef std::function<DT(ST)> LookupCallback;
    typedef std::function<ST(DT)> RevLookupCallback;

public:
    Joint() : m_lookup(nullptr), m_revLookup(nullptr)
    {
    }

    Joint(LookupCallback lookup, RevLookupCallback revLookup) : m_lookup(lookup), m_revLookup(revLookup)
    {
    }

    Joint(const Joint &obj) : m_lookup(obj.m_lookup), m_revLookup(obj.m_revLookup)
    {
    }

    Joint(const Joint &&obj) : m_lookup(std::move(obj.m_lookup)), m_revLookup(std::move(obj.m_revLookup))
    {
    }

    virtual ~Joint()
    {
    }

    const Joint &operator=(const Joint &obj)
    {
        m_lookup = obj.m_lookup;
        m_revLookup = obj.m_revLookup;
        return *this;
    }

    const Joint &operator=(const Joint &&obj)
    {
        m_lookup = std::move(obj.m_lookup);
        m_revLookup = std::move(obj.m_revLookup);
        return *this;
    }

    DT lookup(ST s)
    {
        if (m_lookup != nullptr) {
            return m_lookup(s);
        }
        return TypeTraits<DT>::zero();
    }

    ST revLookup(DT d)
    {
        if (m_revLookup != nullptr) {
            return m_revLookup(d);
        }
        return TypeTraits<ST>::zero();
    }

private:
    LookupCallback m_lookup;
    RevLookupCallback m_revLookup;
};

#endif /* _DATA_JOINT_H_ */
