#include <sqlite3.h>
#include <raylib.h>
#include <iostream>
#include <string>
#include <vector>

int callback(void* data, int argc, char** argv, char** azColName) {
    std::vector<std::string>* results = static_cast<std::vector<std::string> *>(data);
    int stringCounter = 0;
    std::string row = "";
    for (int i = 0; i < argc; i++) {
        row += azColName[i];
        row += ": ";
        row += argv[i] ? argv[i] : "NULL";
        row += "\n";
        stringCounter++;
        if (stringCounter == 4) {
            row += "\n";
            stringCounter = 0;
        }
    }

    results->push_back(row);

    return 0;
}

int main() {
    std::string DbTableName;
    std::cout << "enter full name of a table"<<std::endl;
    std::cin >> DbTableName;
    sqlite3* db;
    int tableDb = sqlite3_open("E:/dev/test_db.db", &db); // ”казать путь к базе данных тут

    if (tableDb) {
        std::cerr << "Could not open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return -1;
    }

    std::string sqlQuery = "SELECT * FROM " + DbTableName;
    std::vector<std::string> results;
    char* errMsg = nullptr;

    tableDb = sqlite3_exec(db, sqlQuery.c_str(), callback, &results, &errMsg);

    if (tableDb != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "cppDbGraphical");

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        std::string output = "";

        for (const auto& row : results) {
            output += row;
        }

        DrawText(output.c_str(), 10, 10, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
