#pragma once
#include "lexer.h"
#include "symbol_table.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

// ─────────────────────────────────────────────
//  Recursive-Descent Top-Down Parser
//  (PART III: includes FIRST/FOLLOW + parsing
//   table display, then actual parse)
// ─────────────────────────────────────────────
class Parser {
    vector<Token> tokens;
    int           pos;
    SymbolTable&  symTable;
    vector<string> errors;

    // ── token helpers ─────────────────────────
    Token& cur()              { return tokens[pos]; }
    bool   check(TokenType t) { return cur().type == t; }

    bool match(TokenType t) {
        if (check(t)) { pos++; return true; }
        return false;
    }

    void expect(TokenType t, const string& msg) {
        if (!match(t)) {
            errors.push_back("Syntax Error near '" + cur().value + "': " + msg);
            // panic-mode recovery: skip to next ';' or EOF
            while (!check(TOK_SEMICOLON) && !check(TOK_EOF)) pos++;
        }
    }

    // ── grammar rules ─────────────────────────

    // value -> id | NUM | STR
    string parseValue() {
        if (check(TOK_IDENTIFIER)) {
            string v = cur().value; pos++;
            symTable.insert(v, "value", "identifier");
            return v;
        }
        if (check(TOK_NUMBER)) {
            string v = cur().value; pos++;
            symTable.insert(v, "value", "number");
            return v;
        }
        if (check(TOK_STRING)) {
            string v = cur().value; pos++;
            symTable.insert(v, "value", "string");
            return v;
        }
        errors.push_back("Syntax Error: expected value, got '" + cur().value + "'");
        pos++;
        return "";
    }

    // expr -> id op value
    void parseExpr(const string& table = "") {
        if (!check(TOK_IDENTIFIER)) {
            errors.push_back("Syntax Error: expected column name, got '" + cur().value + "'");
            pos++;
            return;
        }
        string col = cur().value; pos++;
        symTable.insert(col, "column", "identifier", table);

        if (!check(TOK_EQ) && !check(TOK_GT) && !check(TOK_LT)) {
            errors.push_back("Syntax Error: expected =, > or <, got '" + cur().value + "'");
        } else {
            pos++; // consume operator
        }
        parseValue();
    }

    // condition -> expr [AND/OR condition] | NOT condition
    void parseCondition(const string& table = "") {
        if (check(KW_NOT)) {
            pos++;
            parseCondition(table);
        } else {
            parseExpr(table);
            if (check(KW_AND) || check(KW_OR)) {
                pos++;
                parseCondition(table);
            }
        }
    }

    // id_list -> id { , id }
    vector<string> parseIdList(const string& table = "",
                               const string& kind  = "column") {
        vector<string> ids;
        if (!check(TOK_IDENTIFIER)) {
            errors.push_back("Syntax Error: expected identifier, got '" + cur().value + "'");
            return ids;
        }
        ids.push_back(cur().value);
        symTable.insert(cur().value, kind, "identifier", table);
        pos++;
        while (check(TOK_COMMA)) {
            pos++;
            if (!check(TOK_IDENTIFIER)) {
                errors.push_back("Syntax Error: expected identifier after ','");
                break;
            }
            ids.push_back(cur().value);
            symTable.insert(cur().value, kind, "identifier", table);
            pos++;
        }
        return ids;
    }

    // val_list -> value { , value }
    void parseValList() {
        parseValue();
        while (check(TOK_COMMA)) { pos++; parseValue(); }
    }

    // SELECT col_list FROM id [WHERE] [ORDER BY] [GROUP BY] ;
    void parseSelect() {
        pos++; // consume SELECT
        if (check(TOK_STAR)) {
            pos++;
        } else {
            parseIdList();
        }
        expect(KW_FROM, "expected FROM after column list");

        string tbl;
        if (check(TOK_IDENTIFIER)) {
            tbl = cur().value;
            symTable.insert(tbl, "table", "identifier");
            pos++;
        } else {
            errors.push_back("Syntax Error: expected table name after FROM");
        }
        if (check(KW_WHERE)) { pos++; parseCondition(tbl); }
        if (check(KW_ORDER)) {
            pos++;
            expect(KW_BY, "expected BY after ORDER");
            if (check(TOK_IDENTIFIER)) { symTable.insert(cur().value, "column", "identifier", tbl); pos++; }
        }
        if (check(KW_GROUP)) {
            pos++;
            expect(KW_BY, "expected BY after GROUP");
            if (check(TOK_IDENTIFIER)) { symTable.insert(cur().value, "column", "identifier", tbl); pos++; }
        }
        expect(TOK_SEMICOLON, "expected ';' at end of SELECT");
    }

    // INSERT INTO id [(id_list)] VALUES (val_list) ;
    void parseInsert() {
        pos++; // consume INSERT
        expect(KW_INTO, "expected INTO after INSERT");

        string tbl;
        if (check(TOK_IDENTIFIER)) {
            tbl = cur().value;
            symTable.insert(tbl, "table", "identifier");
            pos++;
        } else {
            errors.push_back("Syntax Error: expected table name after INTO");
        }
        if (check(TOK_LPAREN)) {
            pos++;
            parseIdList(tbl, "column");
            expect(TOK_RPAREN, "expected ')' after column list");
        }
        expect(KW_VALUES, "expected VALUES");
        expect(TOK_LPAREN, "expected '(' before values");
        parseValList();
        expect(TOK_RPAREN, "expected ')' after values");
        expect(TOK_SEMICOLON, "expected ';' at end of INSERT");
    }

    // DELETE FROM id WHERE expr ;
    void parseDelete() {
        pos++; // consume DELETE
        expect(KW_FROM, "expected FROM after DELETE");

        string tbl;
        if (check(TOK_IDENTIFIER)) {
            tbl = cur().value;
            symTable.insert(tbl, "table", "identifier");
            pos++;
        } else {
            errors.push_back("Syntax Error: expected table name after FROM");
        }
        expect(KW_WHERE, "expected WHERE in DELETE");
        parseExpr(tbl);
        expect(TOK_SEMICOLON, "expected ';' at end of DELETE");
    }

public:
    Parser(const vector<Token>& toks, SymbolTable& st)
        : tokens(toks), pos(0), symTable(st) {}

    // ── FIRST & FOLLOW sets ───────────────────
    void printFirstFollow() const {
        cout << "\n╔══════════════════════════════════════════════╗\n";
        cout << "║      PART III: FIRST & FOLLOW Sets           ║\n";
        cout << "╚══════════════════════════════════════════════╝\n";
        cout << left << setw(17) << "Non-Terminal" << "FIRST\n";
        cout << string(50, '-') << "\n";

        auto row = [](const string& nt, const string& first) {
            cout << left << setw(17) << nt << first << "\n";
        };
        row("S",            "{ SELECT, INSERT, DELETE }");
        row("SELECT_STMT",  "{ SELECT }");
        row("INSERT_STMT",  "{ INSERT }");
        row("DELETE_STMT",  "{ DELETE }");
        row("col_list",     "{ *, id }");
        row("id_list",      "{ id }");
        row("WHERE_CLAUSE", "{ WHERE }");
        row("condition",    "{ id, NOT }");
        row("expr",         "{ id }");
        row("op",           "{ =, >, < }");
        row("ORDER_BY",     "{ ORDER }");
        row("GROUP_BY",     "{ GROUP }");
        row("val_list",     "{ id, NUM, STR }");
        row("value",        "{ id, NUM, STR }");

        cout << "\n" << left << setw(17) << "Non-Terminal" << "FOLLOW\n";
        cout << string(50, '-') << "\n";
        row("S",            "{ $ }");
        row("SELECT_STMT",  "{ $ }");
        row("INSERT_STMT",  "{ $ }");
        row("DELETE_STMT",  "{ $ }");
        row("col_list",     "{ FROM }");
        row("id_list",      "{ ), FROM, VALUES }");
        row("WHERE_CLAUSE", "{ ORDER, GROUP, ; }");
        row("condition",    "{ ORDER, GROUP, ; }");
        row("expr",         "{ AND, OR, ; }");
        row("op",           "{ id, NUM, STR }");
        row("ORDER_BY",     "{ GROUP, ; }");
        row("GROUP_BY",     "{ ; }");
        row("val_list",     "{ ) }");
        row("value",        "{ ,, ) }");
        cout << "════════════════════════════════════════════════\n\n";
    }

    // ── Parsing Table ─────────────────────────
    void printParsingTable() const {
        cout << "╔══════════════════════════════════════════════════════════╗\n";
        cout << "║          PART III: Predictive Parsing Table              ║\n";
        cout << "╚══════════════════════════════════════════════════════════╝\n";
        cout << left << setw(17) << "Non-Terminal"
                     << setw(12) << "Input"
                     << "Production\n";
        cout << string(62, '-') << "\n";

        auto row = [](const string& nt, const string& inp, const string& prod) {
            cout << left << setw(17) << nt << setw(12) << inp << prod << "\n";
        };
        row("S",          "SELECT",  "S -> SELECT_STMT");
        row("S",          "INSERT",  "S -> INSERT_STMT");
        row("S",          "DELETE",  "S -> DELETE_STMT");
        row("col_list",   "*",       "col_list -> *");
        row("col_list",   "id",      "col_list -> id_list");
        row("id_list",    "id",      "id_list -> id | id , id_list");
        row("WHERE_CLAUSE","WHERE",  "WHERE_CLAUSE -> WHERE condition");
        row("condition",  "id",      "condition -> expr [AND/OR condition]");
        row("condition",  "NOT",     "condition -> NOT condition");
        row("expr",       "id",      "expr -> id op value");
        row("op",         "=",       "op -> =");
        row("op",         ">",       "op -> >");
        row("op",         "<",       "op -> <");
        row("ORDER_BY",   "ORDER",   "ORDER_BY -> ORDER BY id");
        row("GROUP_BY",   "GROUP",   "GROUP_BY -> GROUP BY id");
        row("val_list",   "id/NUM/STR","val_list -> value | value , val_list");
        row("value",      "id",      "value -> id");
        row("value",      "NUM",     "value -> NUM");
        row("value",      "STR",     "value -> STR");
        cout << "════════════════════════════════════════════════════════════\n\n";
    }

    // ── Main parse ────────────────────────────
    void parse() {
        cout << "╔══════════════════════════════════════════════╗\n";
        cout << "║         PART III: Parsing Result             ║\n";
        cout << "╚══════════════════════════════════════════════╝\n";

        while (!check(TOK_EOF)) {
            if      (check(KW_SELECT)) { cout << "  [->] Parsing SELECT...\n"; parseSelect(); }
            else if (check(KW_INSERT)) { cout << "  [->] Parsing INSERT...\n"; parseInsert(); }
            else if (check(KW_DELETE)) { cout << "  [->] Parsing DELETE...\n"; parseDelete(); }
            else {
                errors.push_back("Syntax Error: unexpected token '" + cur().value + "'");
                pos++;
            }
        }

        if (errors.empty()) {
            cout << "\n  [OK] Parse successful — no syntax errors found.\n";
        } else {
            cout << "\n  [FAIL] Syntax errors detected:\n";
            for (auto& e : errors)
                cout << "    -> " << e << "\n";
        }
        cout << "════════════════════════════════════════════════\n\n";
    }
};
