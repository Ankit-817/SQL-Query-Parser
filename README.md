# SQL Parser

A small C++ command-line project that demonstrates the main stages of parsing a limited SQL-like language. It tokenizes each query, parses it with a recursive-descent parser, prints grammar and FIRST/FOLLOW information, and displays a symbol table. It does not connect to a database or execute SQL statements.

## Features

- Lexer for SQL keywords, identifiers, numbers, single-quoted strings, and basic punctuation/operators.
- Recursive-descent parsing for `SELECT`, `INSERT`, and `DELETE` statements.
- Printed context-free grammar, FIRST/FOLLOW sets, and a predictive parsing table.
- Symbol table entries for tables, columns, and values.
- Built-in sample queries followed by an interactive prompt.

## Requirements

- A C++ compiler with C++17 support.
- On Windows, MinGW-w64 or another toolchain that provides `g++`.
- On Linux or macOS, `g++` (or a compatible compiler executable named `g++`).

## Build and Run

Run the commands from the repository root.

### Windows (PowerShell, MinGW)

```powershell
g++ -std=c++17 files/main.cpp files/lexer.cpp -o sql_parser.exe
.\sql_parser.exe
```

### Linux or macOS

```sh
g++ -std=c++17 files/main.cpp files/lexer.cpp -o sql_parser
./sql_parser
```

The program first processes its built-in examples, including one intentionally invalid query, and then displays the `SQL>` prompt. Enter a query and press Enter to parse it. Type `exit` or `quit` to leave interactive mode.

## Supported SQL Subset

Statements must end with a semicolon.

```sql
SELECT * FROM table_name;
SELECT column_a, column_b FROM table_name WHERE age > 18 ORDER BY age;
SELECT * FROM table_name GROUP BY category;
INSERT INTO table_name (id, name) VALUES (1, 'Ada');
INSERT INTO table_name VALUES (1, 'Ada');
DELETE FROM table_name WHERE id = 1;
```

Conditions support comparisons using `=`, `>`, or `<`, joined with `AND` or `OR`, and the `NOT` keyword. Values can be identifiers, numeric literals, or single-quoted strings. This is a teaching/demo grammar, not a complete SQL implementation; features such as joins, updates, quoted identifiers, and database execution are not provided.

## Project Structure

```text
files/
  cfg.h           Context-free grammar output
  lexer.h         Token definitions and lexer
  lexer.cpp       Keyword table definition
  parser.h        Recursive-descent parser and parsing displays
  symbol_table.h  Symbol table implementation
  main.cpp        Sample queries and interactive command-line loop
```

## Notes

- The parser reports syntax errors in its output; it is intended as an educational demonstration rather than a production SQL validator.
- Compile both `files/main.cpp` and `files/lexer.cpp`. The lexer keyword table is defined in `lexer.cpp` and must be linked into the executable.