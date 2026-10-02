#include "lexer.h"

unordered_map<string, TokenType> Lexer::keywords = {
    {"SELECT", KW_SELECT}, {"FROM",   KW_FROM},   {"WHERE",  KW_WHERE},
    {"INSERT", KW_INSERT}, {"INTO",   KW_INTO},   {"VALUES", KW_VALUES},
    {"DELETE", KW_DELETE}, {"AND",    KW_AND},    {"OR",     KW_OR},
    {"NOT",    KW_NOT},    {"ORDER",  KW_ORDER},  {"BY",     KW_BY},
    {"GROUP",  KW_GROUP}
};