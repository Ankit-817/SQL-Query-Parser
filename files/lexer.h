#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <iomanip>
using namespace std;

// ─────────────────────────────────────────────
//  Token Types
// ─────────────────────────────────────────────
enum TokenType {
    // Keywords
    KW_SELECT, KW_FROM, KW_WHERE, KW_INSERT, KW_INTO,
    KW_VALUES, KW_DELETE, KW_AND, KW_OR, KW_NOT,
    KW_ORDER, KW_BY, KW_GROUP,
    // Symbols
    TOK_STAR, TOK_COMMA, TOK_SEMICOLON,
    TOK_LPAREN, TOK_RPAREN,
    TOK_EQ, TOK_GT, TOK_LT,
    // Literals
    TOK_IDENTIFIER, TOK_NUMBER, TOK_STRING,
    // Control
    TOK_EOF, TOK_UNKNOWN
};

// ─────────────────────────────────────────────
//  Token struct
// ─────────────────────────────────────────────
struct Token {
    TokenType type;
    string    value;
    int       line;
};

// ─────────────────────────────────────────────
//  Helper: token type -> readable name
// ─────────────────────────────────────────────
inline string tokenTypeName(TokenType t) {
    switch (t) {
        case KW_SELECT:      return "SELECT";
        case KW_FROM:        return "FROM";
        case KW_WHERE:       return "WHERE";
        case KW_INSERT:      return "INSERT";
        case KW_INTO:        return "INTO";
        case KW_VALUES:      return "VALUES";
        case KW_DELETE:      return "DELETE";
        case KW_AND:         return "AND";
        case KW_OR:          return "OR";
        case KW_NOT:         return "NOT";
        case KW_ORDER:       return "ORDER";
        case KW_BY:          return "BY";
        case KW_GROUP:       return "GROUP";
        case TOK_STAR:       return "STAR(*)";
        case TOK_COMMA:      return "COMMA(,)";
        case TOK_SEMICOLON:  return "SEMICOLON(;)";
        case TOK_LPAREN:     return "LPAREN(()";
        case TOK_RPAREN:     return "RPAREN())";
        case TOK_EQ:         return "EQ(=)";
        case TOK_GT:         return "GT(>)";
        case TOK_LT:         return "LT(<)";
        case TOK_IDENTIFIER: return "IDENTIFIER";
        case TOK_NUMBER:     return "NUMBER";
        case TOK_STRING:     return "STRING";
        case TOK_EOF:        return "EOF";
        default:             return "UNKNOWN";
    }
}

// ─────────────────────────────────────────────
//  Lexer class
// ─────────────────────────────────────────────
class Lexer {
    string input;
    int    pos;
    int    line;

    // ✅ ONLY DECLARATION (no definition here)
    static unordered_map<string, TokenType> keywords;

    void skipWhitespace() {
        while (pos < (int)input.size() && isspace(input[pos])) {
            if (input[pos] == '\n') line++;
            pos++;
        }
    }

public:
    explicit Lexer(const string& src) : input(src), pos(0), line(1) {}

    // ── tokenize ──────────────────────────────
    vector<Token> tokenize() {
        vector<Token> tokens;
        pos  = 0;
        line = 1;

        while (pos < (int)input.size()) {
            skipWhitespace();
            if (pos >= (int)input.size()) break;

            char c = input[pos];

            // single-char symbols
            if (c == '*') { tokens.push_back({TOK_STAR, "*", line}); pos++; continue; }
            if (c == ',') { tokens.push_back({TOK_COMMA, ",", line}); pos++; continue; }
            if (c == ';') { tokens.push_back({TOK_SEMICOLON, ";", line}); pos++; continue; }
            if (c == '(') { tokens.push_back({TOK_LPAREN, "(", line}); pos++; continue; }
            if (c == ')') { tokens.push_back({TOK_RPAREN, ")", line}); pos++; continue; }
            if (c == '=') { tokens.push_back({TOK_EQ, "=", line}); pos++; continue; }
            if (c == '>') { tokens.push_back({TOK_GT, ">", line}); pos++; continue; }
            if (c == '<') { tokens.push_back({TOK_LT, "<", line}); pos++; continue; }

            // string literal
            if (c == '\'') {
                pos++;
                string s;
                while (pos < (int)input.size() && input[pos] != '\'') s += input[pos++];
                pos++;
                tokens.push_back({TOK_STRING, s, line});
                continue;
            }

            // numeric literal
            if (isdigit(c)) {
                string num;
                while (pos < (int)input.size() && (isdigit(input[pos]) || input[pos] == '.'))
                    num += input[pos++];
                tokens.push_back({TOK_NUMBER, num, line});
                continue;
            }

            // identifier / keyword
            if (isalpha(c) || c == '_') {
                string word;
                while (pos < (int)input.size() && (isalnum(input[pos]) || input[pos] == '_'))
                    word += input[pos++];

                string upper = word;
                transform(upper.begin(), upper.end(), upper.begin(), ::toupper);

                if (keywords.count(upper))
                    tokens.push_back({keywords[upper], upper, line});
                else
                    tokens.push_back({TOK_IDENTIFIER, word, line});

                continue;
            }

            // unknown
            tokens.push_back({TOK_UNKNOWN, string(1, c), line});
            pos++;
        }

        tokens.push_back({TOK_EOF, "", line});
        return tokens;
    }

    // ── print tokens ──────────────────────────
    static void printTokens(const vector<Token>& tokens) {
        cout << "\n╔══════════════════════════════════════════════╗\n";
        cout << "║     PART II: Token Stream (Lexer Output)     ║\n";
        cout << "╚══════════════════════════════════════════════╝\n";

        cout << left << setw(6) << "Line"
             << setw(20) << "Type"
             << "Value\n";

        cout << string(46, '-') << "\n";

        for (auto& t : tokens) {
            if (t.type == TOK_EOF) break;

            cout << left << setw(6)  << t.line
                 << setw(20) << tokenTypeName(t.type)
                 << t.value  << "\n";
        }

        cout << "════════════════════════════════════════════════\n\n";
    }
};