#include <sqlite3.h>
#include <string>
#include <iostream>
#include "./migrations.cpp"

int main()
{
    sqlite3 *db;

    // open sqlite db and check if table exists
    sqlite3_open("test.db", &db);
    sqlite3_stmt *stmt;

    sqlite3_exec(
        db, R"(
            CREATE TABLE IF NOT EXISTS migrations (
                name TEXT PRIMARY KEY
            );
        )",
        NULL, NULL, NULL);

    // check if migrations have been run
    for (const auto &migration : migrations)
    {
        int rc = sqlite3_prepare_v2(db, "SELECT name FROM migrations WHERE name=?;", -1, &stmt, NULL);
        if (rc != SQLITE_OK)
        {
            std::cout << "Error preparing migration select statement" << std::endl;
            return 1;
        }
        rc = sqlite3_bind_text(stmt, 1, migration.name.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
        {
            std::cout << "Error binding migration select statement" << std::endl;
            return 1;
        }
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_ROW && rc != SQLITE_DONE)
        {
            std::cout << "Error running migration select statement" << std::endl;
            return 1;
        }
        // read name
        const char *name = (const char *)sqlite3_column_text(stmt, 0);
        bool migrationExists = name != NULL;

        rc = sqlite3_finalize(stmt);
        if (rc != SQLITE_OK)
        {
            std::cout << "Error finalizing migration select statement" << std::endl;
            return 1;
        }

        // if migration has not been run, run it
        if (!migrationExists)
        {
            std::cout << "Migration" << migration.name << " does not exist. Running." << std::endl;
            rc = sqlite3_exec(db, migration.sql.c_str(), NULL, NULL, NULL);
            if (rc != SQLITE_OK)
            {
                std::cout << "Error running migration " << migration.name << std::endl;
                std::cout << sqlite3_errmsg(db) << std::endl;
                return 1;
            }
            rc = sqlite3_prepare_v2(db, "INSERT INTO migrations (name) VALUES (?);", -1, &stmt, NULL);
            if (rc != SQLITE_OK)
            {
                std::cout << "Error preparing insert statement" << std::endl;
                return 1;
            }

            rc = sqlite3_bind_text(stmt, 1, migration.name.c_str(), -1, SQLITE_STATIC);
            if (rc != SQLITE_OK)
            {
                std::cout << "Error binding insert statement" << std::endl;
                return 1;
            }
            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE)
            {
                std::cout << "Error running insert statement" << std::endl;
                return 1;
            }
            rc = sqlite3_finalize(stmt);
            if (rc != SQLITE_OK)
            {
                std::cout << "Error finalizing insert statement" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cout << "Migration " << migration.name << " exists. Skipping." << std::endl;
        }
    }

    sqlite3_close_v2(db);

    return 0;
}