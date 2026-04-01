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
        .money_sigil = NULL,
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

TEST_CASE("parse_money with money_sigil")
{
    struct parser_options opts = {
        .sep = ',',
        .num_sep = ' ',
        .date_sep = '-',
        .money_sigil = "$",
        .money_prec = 2,
        .money_scale = 100,
    };
    const char *s = " $123.45, $-100.50";
    money_t num;
    const char *p = parse_money(s, &num, &opts);
    CHECK(p != NULL);
    CHECK(num == 12345);
    s = p + 1;
    p = parse_money(s, &num, &opts);
    CHECK(p != NULL);
    CHECK(num == -10050);
}

TEST_CASE("parse_money with money_sigil ¥")
{
    struct parser_options opts = {
        .sep = ',',
        .num_sep = ' ',
        .date_sep = '-',
        .money_sigil = "¥",
        .money_prec = 2,
        .money_scale = 100,
    };
    const char *s = " ¥123.45, ¥-100.50";
    money_t num;
    const char *p = parse_money(s, &num, &opts);
    CHECK(p != NULL);
    CHECK(num == 12345);
    s = p + 1;
    p = parse_money(s, &num, &opts);
    CHECK(p != NULL);
    CHECK(num == -10050);
}
