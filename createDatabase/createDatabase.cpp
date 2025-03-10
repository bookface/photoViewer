//-*-c++-*-
//-*- coding: utf-8 -*-
// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
/*$C gcc -std=c++20 createDatabase.cpp -o createDatabase -Isqlite -Lsqlite -l:sqlite3.a -lstdc++ */

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <sqlite3.h>
#include "table.h"

namespace fs = std::filesystem;
using namespace std;

std::string directoryPath = "d:/Photos/Photos45 2024";
std::string databasePath = DB_FILE_NAME;
std::string tableName    = TABLE_NAME;
std::string fieldName    = FIELD_NAME;

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
int openOrCreateDatabase(const std::string& dbPath, sqlite3** db) {
    int rc = sqlite3_open(databasePath.c_str(), db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(*db) << std::endl;
        sqlite3_close(*db);
        return rc;
    }
    return SQLITE_OK;
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
int createTable(sqlite3* db)
{
    std::stringstream ss; 
    ss << "CREATE TABLE IF NOT EXISTS "
       << tableName << " ("
       << fieldName
       << " TEXT PRIMARY KEY);";
    std::string sql = ss.str();
    char* errmsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errmsg << std::endl;
        sqlite3_free(errmsg);
    }
    return rc;
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
int insertFileData(sqlite3* db, const fs::path& filePath)
{
    std::string filename = filePath.string();
    std::stringstream ss;
    ss <<  "INSERT OR REPLACE INTO " << tableName << "(" << fieldName << ") VALUES ('" 
       << sqlite3_mprintf("%q", filename.c_str()) << "');"; // "', "
    std::string sql = ss.str();
    char *errmsg;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errmsg << std::endl;
        sqlite3_free(errmsg);
    }
    return rc;
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
int createIndexes(sqlite3* db)
{
    std::stringstream ss;
    ss << "CREATE INDEX IF NOT EXISTS idx_filename ON " << tableName << "(" << fieldName << ");";
    std::string indexSql1 = ss.str();
    char* errmsg = nullptr;
    int rc1 = sqlite3_exec(db, indexSql1.c_str(), 0, 0, &errmsg);
    if (rc1 != SQLITE_OK) {
        std::cerr << "SQL error creating filename index: " << errmsg << std::endl;
        sqlite3_free(errmsg);
    }
    return (rc1 != SQLITE_OK) ? SQLITE_ERROR : SQLITE_OK;
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
void countColumnsInTable(void)
{
    sqlite3* db;
    const char *dbName = "photos.db";

    char* errorMessage = nullptr;
    int exitCode = sqlite3_open(dbName, &db);
    if (exitCode) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

// Prepare the SQL statement to get table info
    std::string sql = "PRAGMA table_info(" + tableName + ");";

    sqlite3_stmt* stmt;
    exitCode = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (exitCode != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    int columnCount = 0;

// Execute the statement and count columns
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        columnCount++;
    }

    std::cout << "The table '" << tableName << "' has " << columnCount << " columns." << std::endl;

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
// list all 
void list(void)
{
    sqlite3* db;
    int rc = sqlite3_open("photos.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    char* errmsg = nullptr;


// select
    std::stringstream ss;
    ss << "SELECT * FROM " << tableName << ";";
    std::string inq = ss.str();
    rc = sqlite3_exec(db, inq.c_str(), 0, 0, &errmsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Error on select: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_stmt *stmt;
    auto exitCode = sqlite3_prepare_v2(db, inq.c_str(), -1, &stmt, nullptr);
    if (exitCode != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        const unsigned char* name = sqlite3_column_text(stmt, 0);
        std::cout << "Name: " << name << "\n";
    }
}

// ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
int main(int argc, char* argv[]) {
    if (argc > 2) {                     // debug, any two arguments
        countColumnsInTable();
        list();
        return 0;
    }

    if (argc > 1) {                     // for db creation
        directoryPath = argv[1];
    }

    sqlite3* db;
    if (openOrCreateDatabase(databasePath, &db) != SQLITE_OK) {
        return 1;
    }

    if (createTable(db) != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
        if (fs::is_regular_file(entry)) {
            if (insertFileData(db, entry.path()) != SQLITE_OK) {
                std::cerr << "Error inserting file: " << entry.path() << std::endl;
            }
        }
    }

    if (createIndexes(db) != SQLITE_OK) {
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}
