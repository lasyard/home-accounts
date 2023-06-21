#include "Wrapper.h"

#include "CsvExceptions.h"
#include "CsvParser.h"

#include "str.h"
#include "utils.h"

Wrapper::Wrapper(const char *config, ConfigParser *parser)
{
    m_seps[0] = ',';
    m_seps[1] = ' ';
    m_seps[2] = '-';
    const char *p = parse_seps(config, m_seps, SEPS_SEP, sizeof(m_seps) / sizeof(char));
    int cols = count_chars(p, COLS_SEP) + 1;
    m_types = new ColumnType[cols];
    m_map = new int[cols];
    for (int i = 0; !is_line_end(*p) && i < cols; ++i) {
        struct string s;
        p = parse_string(p, &s, COLS_SEP);
        if (!parser(&s, &m_types[i], &m_map[i])) {
            m_types[i] = IGNORE;
            m_map[i] = -1;
        }
        // Skip the sep.
        if (*p == COLS_SEP) {
            ++p;
        }
    }
    m_cols = cols;
}

Wrapper::~Wrapper()
{
    delete m_types;
    delete m_map;
}

CsvParser *Wrapper::createParser() const
{
    auto parser = new CsvParser(m_cols, m_types, getPtr);
    parser->setSeparator(m_seps[0]);
    parser->setNumSeparator(m_seps[1]);
    parser->setDateSeparator(m_seps[2]);
    return parser;
}

void *Wrapper::getPtr(void *data, int i)
{
    Wrapper *wrapper = static_cast<Wrapper *>(data);
    int index = wrapper->m_map[i];
    return wrapper->getDataPtr(wrapper->m_data, index);
}
