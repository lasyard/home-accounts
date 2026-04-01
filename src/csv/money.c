#include <ctype.h>
#include <stddef.h>
#include <string.h>

#include "money.h"

#include "int.h"
#include "parser_options.h"
#include "utils.h"

static bool starts_with(const char *str, const char *prefix)
{
    while (*prefix != '\0') {
        if (*str != *prefix) {
            return false;
        }
        ++str;
        ++prefix;
    }
    return true;
}

const char *parse_money(const char *buf, money_t *data, const struct parser_options *options)
{
    money_t num = 0;
    const char *p = skip_space(buf);
    if (options->money_sigil != NULL && starts_with(p, options->money_sigil)) {
        p += strlen(options->money_sigil);
        p = skip_space(p);
    }
    bool decimal = false;
    bool pos;
    int scale = options->money_scale;
    p = parse_sign(p, &pos);
    for (; *p != options->sep && !is_line_end(*p); ++p) {
        if (!is_space(*p)) {
            if (isdigit(*p)) {
                num = num * 10 + digittoint(*p);
            } else if (*p == '.') {
                decimal = true;
                ++p;
                break;
            } else if (*p != options->num_sep) {
                return NULL;
            }
        }
    }
    num *= scale;
    if (decimal) {
        scale /= 10;
        for (; *p != options->sep && !is_line_end(*p); ++p) {
            if (!is_space(*p)) {
                if (isdigit(*p)) {
                    if (scale < 1) {
                        return NULL;
                    }
                    num += digittoint(*p) * scale;
                    scale /= 10;
                } else {
                    return NULL;
                }
            }
        }
    }
    *data = (pos ? num : -num);
    return p;
}

char *output_money(char *buf, money_t data, int prec, int scale)
{
    if (data < 0) {
        *(buf++) = '-';
        data = -data;
    }
    char *p = output_int(buf, data / scale);
    *(p++) = '.';
    p = output_int_len(p, data % scale, prec);
    return p;
}
