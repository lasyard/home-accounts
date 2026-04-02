#ifndef _HA_GUI_BI_MAP_H_
#define _HA_GUI_BI_MAP_H_

#include <map>

template <typename K, typename V> class BiMap
{
public:
    BiMap() = default;
    virtual ~BiMap() = default;

    /**
     * @brief Set the kv object
     *
     * @param k
     * @param v
     * @return std::pair<const K *, const V *> old key and value, or nullptr if there is no old key or value
     */
    std::pair<const K *, const V *> set_kv(const K &k, const V &v)
    {
        auto *old_k = erase_old_k(v);
        auto *old_v = erase_old_v(k);
        m_kv[k] = v;
        m_vk[v] = k;
        return {old_k, old_v};
    }

    const V *erase_k(const K &k)
    {
        auto old_v = erase_old_v(k);
        m_kv.erase(k);
        return old_v;
    }

    const K *erase_v(const V &v)
    {
        auto old_k = erase_old_k(v);
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

    const V *k_v(const K &k) const
    {
        auto it = m_kv.find(k);
        return (it != m_kv.end()) ? &it->second : nullptr;
    }

    const K *v_k(const V &v) const
    {
        auto it = m_vk.find(v);
        return (it != m_vk.end()) ? &it->second : nullptr;
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

    const K *erase_old_k(const V &v)
    {
        auto *old_k = v_k(v);
        if (old_k != nullptr) {
            m_kv.erase(*old_k);
        }
        return old_k;
    }

    const V *erase_old_v(const K &k)
    {
        auto old_v = k_v(k);
        if (old_v != nullptr) {
            m_vk.erase(*old_v);
        }
        return old_v;
    }
};

#endif /* _HA_GUI_BI_MAP_H_ */
