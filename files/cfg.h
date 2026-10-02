#pragma once
#include <iostream>
using namespace std;

/*
 * PART I: Context-Free Grammar (CFG)
 *
 * S           -> SELECT_STMT | INSERT_STMT | DELETE_STMT
 * SELECT_STMT -> SELECT col_list FROM id [WHERE_CLAUSE] [ORDER_BY] [GROUP_BY] ;
 * col_list    -> * | id_list
 * id_list     -> id | id , id_list
 * WHERE_CLAUSE-> WHERE condition
 * condition   -> expr | expr AND condition | expr OR condition | NOT condition
 * expr        -> id op value
 * op          -> = | > | <
 * ORDER_BY    -> ORDER BY id
 * GROUP_BY    -> GROUP BY id
 * INSERT_STMT -> INSERT INTO id ( id_list ) VALUES ( val_list ) ;
 *             | INSERT INTO id VALUES ( val_list ) ;
 * val_list    -> value | value , val_list
 * value       -> id | NUM | STR
 * DELETE_STMT -> DELETE FROM id WHERE expr ;
 */

void printCFG() {
    cout << "\n╔══════════════════════════════════════════════╗\n";
    cout << "║       PART I: Context-Free Grammar (CFG)     ║\n";
    cout << "╚══════════════════════════════════════════════╝\n";
    cout << "S           -> SELECT_STMT | INSERT_STMT | DELETE_STMT\n";
    cout << "SELECT_STMT -> SELECT col_list FROM id [WHERE_CLAUSE] [ORDER_BY] [GROUP_BY] ;\n";
    cout << "col_list    -> * | id_list\n";
    cout << "id_list     -> id | id , id_list\n";
    cout << "WHERE_CLAUSE-> WHERE condition\n";
    cout << "condition   -> expr | expr AND condition | expr OR condition | NOT condition\n";
    cout << "expr        -> id op value\n";
    cout << "op          -> = | > | <\n";
    cout << "ORDER_BY    -> ORDER BY id\n";
    cout << "GROUP_BY    -> GROUP BY id\n";
    cout << "INSERT_STMT -> INSERT INTO id ( id_list ) VALUES ( val_list ) ;\n";
    cout << "            | INSERT INTO id VALUES ( val_list ) ;\n";
    cout << "val_list    -> value | value , val_list\n";
    cout << "value       -> id | NUM | STR\n";
    cout << "DELETE_STMT -> DELETE FROM id WHERE expr ;\n";
    cout << "════════════════════════════════════════════════\n\n";
}
