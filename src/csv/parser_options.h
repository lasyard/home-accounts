#ifndef _HA_CSV_PARSER_OPTIONS_H_
#define _HA_CSV_PARSER_OPTIONS_H_

struct parser_options {
    char sep;                // the separator of fields
    char num_sep;            // the separator in numbers
    char date_sep;           // the separator of y/m/d in date
    const char *money_sigil; // the currency symbol to ignore (UTF-8 string)
    int money_prec;          // the precision of money
    int money_scale;         // the scale factor of money
};

#endif /* _HA_CSV_PARSER_OPTIONS_H_ */