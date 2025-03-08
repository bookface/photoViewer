//-*-c++-*-
//-*- coding: utf-8 -*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■

/*$C gcc -std=c++20 -DTESTING_GETONE getOne.cpp -o getOne -Isqlite -Lsqlite -l:libsqlitelib.a -lstdc++ */
#include <sqlite3.h>
#include <iostream>
#include <sstream>

#include "table.h"

static std::string databasePath(DB_FILE_NAME);
static std::string tableName(TABLE_NAME);
static sqlite3* db;

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
bool openDatabase(void)
{
    int rc = sqlite3_open(databasePath.c_str(), &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return false;
    }
    return true;
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
unsigned char *getOne(void)
{
    char* errmsg = nullptr;
    std::stringstream ss;
    ss << "SELECT * FROM " << tableName << " ORDER BY RANDOM() LIMIT 1;";
    std::string inq = ss.str();
    int rc = sqlite3_exec(db, inq.c_str(), 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        std::cout << "Cannot open database: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return (unsigned char *)"";
    }

    sqlite3_stmt *stmt;
    rc  = sqlite3_prepare_v2(db, inq.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cout << "Error preparing statement: " << sqlite3_errmsg(db) << "\n";
        sqlite3_close(db);
        return (unsigned char *)"";
        
    }
    static unsigned char *name = nullptr;
    if ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        name = (unsigned char *)sqlite3_column_text(stmt, 0);
    // cout << "name " << name << "\n";
        return name;
    }
    return name;
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
#ifdef TESTING_GETONE
int main(int argc, char * argv[])
{
    if (openDatabase()) {
        for (int i = 0; i < 10; ++i) {
            std::cout << getOne() << "\n";
        }
    }
    return 0;
}
#endif
