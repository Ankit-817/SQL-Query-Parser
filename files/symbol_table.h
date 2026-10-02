#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

// ─────────────────────────────────────────────
//  Symbol Table Entry
// ─────────────────────────────────────────────
struct SymbolEntry {
    string name;
    string kind;       // "table" | "column" | "value"
    string dataType;   // "identifier" | "number" | "string"
    string belongsTo;  // parent table (empty if kind == "table")
};

// ─────────────────────────────────────────────
//  Symbol Table
// ─────────────────────────────────────────────
class SymbolTable {
    vector<SymbolEntry> entries;

public:
    void insert(const string& name,
                const string& kind,
                const string& dataType,
                const string& belongsTo = "")
    {
        // deduplicate
        for (auto& e : entries)
            if (e.name == name && e.kind == kind && e.belongsTo == belongsTo) return;
        entries.push_back({name, kind, dataType, belongsTo});
    }

    void clear() { entries.clear(); }

    void print() const {
        cout << "\n╔══════════════════════════════════════════════╗\n";
        cout << "║           PART IV: Symbol Table              ║\n";
        cout << "╚══════════════════════════════════════════════╝\n";
        cout << left << setw(20) << "Name"
                     << setw(12) << "Kind"
                     << setw(14) << "DataType"
                     << "BelongsTo\n";
        cout << string(55, '-') << "\n";
        for (auto& e : entries) {
            cout << left << setw(20) << e.name
                         << setw(12) << e.kind
                         << setw(14) << e.dataType
                         << e.belongsTo << "\n";
        }
        cout << "════════════════════════════════════════════════\n\n";
    }
};
