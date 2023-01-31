#include <string>

struct Migration
{
    std::string name;
    std::string sql;
};

const Migration migrations[] = {
    {"2023_01_31_17_50__initial.sql", R"(
        CREATE TABLE users (
            id INTEGER PRIMARY KEY, 
            name TEXT
        );
    )"},
    {"2023_01_31_17_50__add_email_column.sql", R"(
        ALTER TABLE users ADD COLUMN email TEXT;
    )"},
    {"2023_01_31_17_50__add_password_column.sql", R"(
        ALTER TABLE users ADD COLUMN password TEXT;
    )"},
    {"ei väliä", R"(
        ALTER TABLE users ADD COLUMN age INTEGER;
        ALTER TABLE users ADD COLUMN height INTEGER;
    )"},
    // copy table to new table
    {"2023_01_31_17_50__copy_users_to_users2.sql", R"(
        CREATE TABLE users2 AS SELECT id, name FROM users;

        -- drop old table
        DROP TABLE users;

        -- rename new table
        ALTER TABLE users2 RENAME TO users;
    )"},
    // add password column
    {"2023_01_31_17_50__add_password_column1.sql", R"(
        ALTER TABLE "users" ADD COLUMN password TEXT;
    )"},

};
