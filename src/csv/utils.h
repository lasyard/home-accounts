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

#if defined(_ANSI_SOURCE) || (defined(_POSIX_C_SOURCE) && !defined(_DARWIN_C_SOURCE))
static inline int digittoint(int c)
{
    if (c >= '0' && c <= '9') {
        return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    }
    if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    }
    return 0;
}
#endif

#ifdef __cplusplus
}
#endif

#endif /* _HA_CSV_UTILS_H_ */
