#include "cfg.h"
#include "lexer.h"
#include "symbol_table.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

// ─────────────────────────────────────────────
//  Run a single SQL query through all stages
// ─────────────────────────────────────────────
void runQuery(const string& query) {
    cout << "\n╔══════════════════════════════════════════════╗\n";
    cout << "║  INPUT: " << query << "\n";
    cout << "╚══════════════════════════════════════════════╝\n";

    // Stage 1: Lex
    Lexer lexer(query);
    vector<Token> tokens = lexer.tokenize();
    Lexer::printTokens(tokens);

    // Stage 2: Symbol table (shared with parser)
    SymbolTable symTable;

    // Stage 3: Parser (FIRST/FOLLOW + parsing table + parse)
    Parser parser(tokens, symTable);
    parser.printFirstFollow();
    parser.printParsingTable();
    parser.parse();

    // Stage 4: Symbol table output
    symTable.print();
}

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    // Print CFG once at startup
    printCFG();

    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║          SQL PARSER — Test Suite             ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";

    // ── Test cases ────────────────────────────
    runQuery("SELECT name, age FROM students WHERE age > 18 AND name = 'Alice' ORDER BY age;");
    runQuery("SELECT * FROM employees GROUP BY department;");
    runQuery("INSERT INTO users (id, name, email) VALUES (1, 'Bob', 'bob@mail.com');");
    runQuery("INSERT INTO orders VALUES (101, 'pen', 50);");
    runQuery("DELETE FROM products WHERE price < 10;");
    runQuery("SELECT FROM orders WHERE;");   // intentional syntax error

    // ── Interactive mode ──────────────────────
    cout << "╔══════════════════════════════════════════════╗\n";
    cout << "║  Interactive Mode  (type 'exit' to quit)     ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";

    string line;
    while (true) {
        cout << "\nSQL> ";
        getline(cin, line);
        if (line == "exit" || line == "quit") break;
        if (!line.empty()) runQuery(line);
    }

    return 0;
}
