#include "doctest/doctest.h"

#include <cstddef>

#include "money.h"

#include "parser_options.h"

TEST_CASE("parse_money")
{
    struct parser_options opts = {
        .sep = ',',
        .num_sep = ' ',
        .date_sep = '-',
        .money_prec = 2,
        .money_scale = 100,
    };
    const char *s = " 123.45, -1 030.478 5";
    money_t num;
    const char *p = parse_money(s, &num, &opts);
    CHECK(p - s == 7);
    CHECK(num == 12345);
    s = p + 1;
    opts.money_scale = 10000;
    p = parse_money(s, &num, &opts);
    CHECK(p - s == 13);
    CHECK(num == -10304785);
}
