#ifndef _HA_CSV_UTILS_H_
#define _HA_CSV_UTILS_H_

#ifdef __cplusplus
extern "C" {
#endif

static inline bool is_line_end(int c)
{
    return c == '\0' || c == '\r' || c == '\n';
}

static inline bool is_space(int c)
{
    return c == ' ' || c == '\t';
}

static inline const char *skip_space(const char *buf)
{
    const char *p;
    for (p = buf; is_space(*p) && !is_line_end(*p); ++p)
        ;
    return p;
}

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_UTILS_H_ */
