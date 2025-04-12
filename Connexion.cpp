#include "Connexion.hpp"

Connexion::Connexion(const string& user, const string& password, const string& dbname, const string& host, int port)
    : user(user), password(password), dbname(dbname), host(host), port(port), con(nullptr) {}

Connexion::~Connexion() {
    closeConnection();
}

bool Connexion::connecter() {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect("tcp://" + host + ":" + to_string(port), user, password);
        con->setSchema(dbname);
        cout << "Connexion réussie à la base de données : " << dbname << endl;
        return true;
    } catch (sql::SQLException &e) {
        cerr << "Erreur lors de la connexion : " << e.what() << endl;
        return false;
    }
}

vector<vector<string>> Connexion::executeQuery(const string& query) {
    vector<vector<string>> result;
    try {
        if (!con || con->isClosed()) {
            cerr << "Connexion fermée ou non initialisée." << endl;
            return result;
        }
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery(query);

        sql::ResultSetMetaData *meta = res->getMetaData();
        int columnCount = meta->getColumnCount();

        while (res->next()) {
            vector<string> row;
            for (int i = 1; i <= columnCount; i++) {
                string value = res->getString(i);
                if (res->isNull(i)) {
                    row.push_back("NULL");
                } else {
                    row.push_back(value);
                }
            }
            result.push_back(row);
        }

        delete res;
        delete stmt;
        return result;
    } catch (sql::SQLException &e) {
        cerr << "Erreur lors de l'exécution de la requête : " << e.what() << endl;
        return result;
    }
}

bool Connexion::executeUpdate(const string& query) {
    try {
        if (!con || con->isClosed()) {
            cerr << "Connexion fermée ou non initialisée." << endl;
            return false;
        }
        sql::Statement *stmt = con->createStatement();
        stmt->executeUpdate(query);
        delete stmt;
        return true;
    } catch (sql::SQLException &e) {
        cerr << "Erreur lors de l'exécution de l'update : " << e.what() << endl;
        return false;
    }
}

void Connexion::closeConnection() {
    if (con && !con->isClosed()) {
        delete con;
        con = nullptr;
        cout << "Connexion fermée." << endl;
    }
}

sql::Connection* Connexion::getConnection() {
    return con;
}