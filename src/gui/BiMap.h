#ifndef _HA_GUI_BI_MAP_H_
#define _HA_GUI_BI_MAP_H_

#include <map>

template <typename K, typename V, auto INVALID_K, auto INVALID_V> class BiMap
{
public:
    BiMap() = default;
    virtual ~BiMap() = default;

    std::pair<K, V> set_kv(const K &k, const V &v)
    {
        auto old_k = v_k(v);
        auto old_v = k_v(k);
        if (old_k != INVALID_K) {
            m_kv.erase(old_k);
        }
        if (old_v != INVALID_V) {
            m_vk.erase(old_v);
        }
        m_kv[k] = v;
        m_vk[v] = k;
        return {old_k, old_v};
    }

    V erase_k(const K &k)
    {
        auto old_v = k_v(k);
        if (old_v != INVALID_V) {
            m_vk.erase(old_v);
        }
        m_kv.erase(k);
        return old_v;
    }

    K erase_v(const V &v)
    {
        auto old_k = v_k(v);
        if (old_k != INVALID_K) {
            m_kv.erase(old_k);
        }
        m_vk.erase(v);
        return old_k;
    }

    bool has_k(const K &k) const
    {
        return m_kv.find(k) != m_kv.end();
    }

    bool has_v(const V &v) const
    {
        return m_vk.find(v) != m_vk.end();
    }

    V k_v(const K &k) const
    {
        auto it = m_kv.find(k);
        return (it != m_kv.end()) ? it->second : V(INVALID_V);
    }

    K v_k(const V &v) const
    {
        auto it = m_vk.find(v);
        return (it != m_vk.end()) ? it->second : K(INVALID_K);
    }

    void clear()
    {
        m_kv.clear();
        m_vk.clear();
    }

    std::size_t size() const
    {
        return m_kv.size();
    }

    bool empty() const
    {
        return m_kv.empty();
    }

private:
    std::map<K, V> m_kv;
    std::map<V, K> m_vk;
};

#endif /* _HA_GUI_BI_MAP_H_ */
