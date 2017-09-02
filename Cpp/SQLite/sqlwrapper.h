#include <sqlite3.h>

#include <string>
#include <memory>

#include <map>
#include <vector>
#include <list>

#include <iostream>

#include<cstdlib>
#include<cstring>

namespace sqlite {

class Statement;
class Transaction;
class RowSet;
class Row;

class Database {
public:
    Database(const std::string& filename);

    void Execute(const std::string& query);

    std::unique_ptr<Statement> PrepareStatement(const std::string& query);
    std::unique_ptr<Transaction> BeginTransaction();

    ~Database();

private:
    sqlite3* db_;
};

class Statement {
public:
    void Bind(int index, int param);
    void Bind(int index, const std::string& param);

    std::unique_ptr<RowSet> Execute();

    Statement(sqlite3_stmt* statement, Database* database);

    int getIndex(const std::string& pattern) const;

    sqlite3_stmt* getStatement();

private:
    sqlite3_stmt* statement_;
    Database* database_;
};

class Row {
public:
    int GetColumnsCount() const;

    int GetInt(int i) const;
    std::string GetString(int i) const;

    Row () {}

    Row& operator=(const Row& other);

    void addColumn(const int index, const int value);

    void addColumn(const int index, const std::string& value);

    std::string toString() const;

private:
    std::map<int, std::string> values_;
};

class RowSet {
public:
    class RowIterator {
    public:
        RowIterator& operator++();
        Row operator*() const;
        Row* operator->() const;

        bool operator==(const RowIterator& rhs) const;
        bool operator!=(const RowIterator& rhs) const;

        RowIterator();
        RowIterator(RowSet* rowSet, int index);

    private:
        int element_index_;
        RowSet* owner_set_;
    };

    void addRow(const Row& row);

    RowIterator begin();

    RowIterator end();

    std::string toString();

private:
    std::vector<Row> rows;
};

class Transaction {
    public:
        ~Transaction();

        void Commit();

        void Abort();

        Transaction(Database* owner_database);

        void Execute(const std::string& query);

        std::unique_ptr<RowSet> Execute(std::unique_ptr<Statement> statement);

    private:
        Database* owner_database_;
        bool isTransactionActive;
    };

    std::unique_ptr<Transaction> Database::BeginTransaction() {
        return std::unique_ptr<Transaction>(new Transaction(this));
    }

    Database::Database(const std::string &filename) {
        char *zErrMsg = 0;
        int rc;
        rc = sqlite3_open(filename.c_str(), &db_);
        if(rc) {
            throw std::runtime_error("Database " + filename + " cannot be opened\n");
        }

    }

    Database::~Database() {
        sqlite3_close(db_);
    }

    void Database::Execute(const std::string &query) {
        char* zErrMsg;
        int rc = sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &zErrMsg);
        if (rc != SQLITE_OK) {
            throw std::runtime_error("Could not Execute query " + query + "; error " + zErrMsg);
        }
    }

    std::unique_ptr<Statement> Database::PrepareStatement(const std::string& query) {
        sqlite3_stmt* statement;
        int rc = sqlite3_prepare_v2(db_, query.c_str(), -1, &statement, 0);
        if (rc != SQLITE_OK) {
            throw std::runtime_error(
                    "Statement " + query + " cannot be prepared, error code is " + std::to_string(rc) + "\n");
        }
        return std::unique_ptr<Statement> (new Statement(statement, this));
    }





    Statement::Statement(sqlite3_stmt *statement, Database* database) :
            statement_(statement),
            database_(database) {}

    void Statement::Bind(int index, int param) {
        sqlite3_bind_int(statement_, index, param);
    }

    void Statement::Bind(int index, const std::string& param) {
        sqlite3_bind_text(statement_, index, param.c_str(), -1, SQLITE_TRANSIENT);
    }

    std::unique_ptr<RowSet> Statement::Execute() {
        RowSet* row_set_ptr = new RowSet();
        while (true) {
            int rc = sqlite3_step(statement_);
            if (rc == SQLITE_ROW) {
                Row row;
                int cols_count = sqlite3_data_count(statement_);
                for (int index = 0; index < cols_count; ++index) {
                    int column_type = sqlite3_column_type(statement_, index);
                    if (column_type == SQLITE3_TEXT) {
                        const unsigned char* text = sqlite3_column_text(statement_, index);
                        char* p= (char*)calloc(32, sizeof(char));
                        strcpy(p, reinterpret_cast<const char*>(text));
                        row.addColumn(index, std::string(p));
                    }
                    if (column_type == SQLITE_INTEGER) {
                        row.addColumn(index, sqlite3_column_int(statement_, index));
                    }
                }
                row_set_ptr->addRow(row);
            }  else {
                if (rc == SQLITE_DONE) {
                    sqlite3_reset(statement_);
                }
                break;
            }
        }
        return std::unique_ptr<RowSet>(row_set_ptr);
    }

    sqlite3_stmt* Statement::getStatement() {
        return statement_;
    }

    int Statement::getIndex(const std::string &pattern) const {
        return sqlite3_bind_parameter_index(statement_, pattern.c_str());
    }



    int Row::GetColumnsCount() const {
        return values_.size();
    }

    int Row::GetInt(int index) const {
        if (index >= values_.size() || index < 0) {
            throw std::out_of_range("index " +
                                    std::to_string(index) +
                                    " is out of range " +
                                    std::to_string(values_.size()));
        }
        std::string value = values_.at(index);
        return atoi(values_.at(index).c_str());
    }

    std::string Row::GetString(int index) const {
        if (index >= values_.size() || index < 0) {
            throw std::out_of_range("index " +
                                    std::to_string(index) +
                                    " is out of range " +
                                    std::to_string(values_.size()));
        }
        return values_.at(index);
    }

    Row& Row::operator=(const Row& other) {
        Row row;
        row.values_ = other.values_;
    }

    void Row::addColumn(const int index, const int value)  {
        values_[index] = std::to_string(value);
    }

    void Row::addColumn(const int index, const std::string &value)  {
        values_[index] = value;
    }

    std::string Row::toString() const  {
        std::string row_print = "";
        row_print += "|";
        for (auto it = values_.begin(); it != values_.end(); ++it) {
            row_print += (it->second + "|");
        }
        return row_print;
    }






    void RowSet::addRow(const Row &row)  {
        rows.push_back(row);
    }

    RowSet::RowIterator RowSet::begin()  {
        return RowIterator(this, 0);
    }

    RowSet::RowIterator RowSet::end()  {
        return RowIterator(this, rows.size());
    }

    std::string RowSet::toString()  {
        std::string set_string;
        for (const auto& row : rows) {
            set_string += (row.toString() + "\n");
        }
        return set_string;
    }




    RowSet::RowIterator& RowSet::RowIterator::operator++() {
        ++element_index_;
    }

    Row RowSet::RowIterator::operator*() const {
        return owner_set_->rows[element_index_];
    }

    Row* RowSet::RowIterator::operator->() const {
        return &(owner_set_->rows[element_index_]);
    }

    bool RowSet::RowIterator::operator==(const RowIterator &rhs) const {
        return (element_index_ == rhs.element_index_ &&
                owner_set_ == rhs.owner_set_);
    }

    bool RowSet::RowIterator::operator!=(const RowIterator &rhs) const {
        return (element_index_!= rhs.element_index_ ||
                owner_set_ != rhs.owner_set_);
    }

    RowSet::RowIterator::RowIterator() :
        element_index_(0) {
        owner_set_ = nullptr;
    }

    RowSet::RowIterator::RowIterator(RowSet* rowSet, int element_index) :
            owner_set_(rowSet),
            element_index_(element_index) {}





    Transaction::~Transaction() { // implicitly abort transaction
        if (isTransactionActive && owner_database_ != nullptr) {
            owner_database_->Execute("ROLLBACK;");
        }
    }

    void Transaction::Commit() {
        owner_database_->Execute("END TRANSACTION;");
        isTransactionActive = false;
    }

    void Transaction::Abort() {
        owner_database_->Execute("ROLLBACK;");
        isTransactionActive = false;
    }

    Transaction::Transaction(Database *owner_database)  :
            owner_database_(owner_database) {
        owner_database_->Execute("BEGIN TRANSACTION;");
        isTransactionActive = true;
    }

    void Transaction::Execute(const std::string &query) {
        try {
            owner_database_->Execute(query);
        } catch (std::runtime_error error) {
            Abort();
        }
    }

    std::unique_ptr<RowSet> Transaction::Execute(std::unique_ptr<Statement> statement) {
        try {
            return statement.get()->Execute();
        } catch (std::runtime_error error) {
            Abort();
        }
    }

}
