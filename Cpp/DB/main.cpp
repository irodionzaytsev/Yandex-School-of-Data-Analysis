#include <iostream>
#include <sqlite3.h>

std::string result;

class DBWrapper {

public:
    DBWrapper(const std::string& database_name) {
        int rc = sqlite3_open(database_name.c_str(), &db);
    }

    static int callback (void *NotUsed, int argc, char **argv, char **azColName) {
        result = argv[1];
        /*
         * for(int i = 0; i < argc; ++i){
            std::cout << azColName[i] << " = " << argv[i] << "\n";
            printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
        }*/
        return 0;
    }

    std::string getValue(const std::string& id) {
        result = "not found";
        char* zErrMsg = 0;
        std::string request = "select * from test_table where id = " + id;
        int rc = sqlite3_exec(db, request.c_str(), callback, 0, &zErrMsg);
        return result;
    }

    ~DBWrapper() {
        sqlite3_close(db);
    }

    sqlite3* db;
};

int main() {
    DBWrapper dbWrapper("test.db");
    std::string id;
    std::cin >> id;
    std::string result = dbWrapper.getValue(id);
    std::cout << result << "\n";
    return 0;
}
