#include "utils.h"

#include "str.h"

int count_chars(const char *buf, char ch)
{
    int count = 0;
    for (const char *p = skip_space(buf); !is_line_end(*p); ++p) {
        if (*p == ch) {
            ++count;
        }
    }
    return count;
}

char parse_sep(const struct string *str)
{
    if (string_cstrcmp_nc(str, "TAB") == 0) {
        return '\t';
    } else if (string_cstrcmp_nc(str, "COMMA") == 0) {
        return ',';
    } else if (string_cstrcmp_nc(str, "HYPHEN") == 0) {
        return '-';
    } else if (string_cstrcmp_nc(str, "SLASH") == 0) {
        return '/';
    } else if (string_cstrcmp_nc(str, "COLON") == 0) {
        return ':';
    } else if (string_cstrcmp_nc(str, "VERBAR") == 0) {
        return '|';
    }
    return '\0';
}

const char *parse_seps(const char *buf, char *seps, int limit)
{
    const char *p;
    int i = 0;
    for (p = buf; i < limit && !is_line_end(*p);) {
        struct string s;
        p = parse_string(p, &s, CONFIG_SEP_SEP);
        if (*p == CONFIG_SEP_SEP) {
            char sep = parse_sep(&s);
            if (sep != '\0') {
                seps[i] = sep;
            }
            ++i;
            // skip the SEP.
            ++p;
        } else {
            p = s.buf;
            break;
        }
    }
    return p;
}
