#include <ctype.h>

#include "money.h"

#include "int.h"
#include "utils.h"

const char *parse_money(const char *buf, money_t *data, char sep, int scale, char num_sep)
{
    money_t num = 0;
    const char *p = skip_space(buf);
    bool decimal = false;
    bool pos;
    p = parse_sign(p, &pos);
    for (; *p != sep && !is_line_end(*p); ++p) {
        if (!is_space(*p)) {
            if (isdigit(*p)) {
                num = num * 10 + digittoint(*p);
            } else if (*p == '.') {
                decimal = true;
                ++p;
                break;
            } else if (*p != num_sep) { // allow delimiter of numbers
                return NULL;
            }
        }
    }
    num *= scale;
    if (decimal) {
        // fraction part.
        scale /= 10;
        for (; *p != sep && !is_line_end(*p); ++p) {
            if (!is_space(*p)) {
                if (isdigit(*p)) {
                    // too many digits.
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
