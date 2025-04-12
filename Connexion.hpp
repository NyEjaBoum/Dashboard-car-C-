#ifndef CONNEXION_HPP
#define CONNEXION_HPP

#include <iostream>
#include <vector>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset_metadata.h>
#include <cppconn/exception.h>

using namespace std;

class Connexion {
public:
    Connexion(const string& user, const string& password, const string& dbname, const string& host = "localhost", int port = 3306);
    ~Connexion();

    bool connecter();
    vector<vector<string>> executeQuery(const string& query);
    bool executeUpdate(const string& query);
    void closeConnection();
    sql::Connection* getConnection();

private:
    string user, password, dbname, host;
    int port;
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;
};

#endif // CONNEXION_HPP