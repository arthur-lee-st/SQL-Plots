#include "postgresql1.h"

void Postgresql1::postgresql_turn_on(bool* db_on_off) {
    *db_on_off = true; //set db to on
}

pqxx::result Postgresql1::postgresql_body(char text[256], pqxx::result* result_raw, pqxx::result* result_for_table, bool* query_success) {
    /**
     * run queries
    */
    pqxx::work txn{*conn_ptr};
    try {
        *result_raw = txn.exec(text);

        //if result_raw is a success, then have result_for_table be set to it. result_for_table will then be grabbed by window4's populate()
        //this means result_for_table only gets updated with intended data.
        if(result_raw->query() != "" && std::string(text).length() != 0){
            result_for_table = result_raw;
            txn.commit();
            *query_success = true;
        }
        return *result_for_table;
    }
    catch(const pqxx::syntax_error& e) {
        std::cout << e.what() << std::endl;
        *query_success = false;
    }
    catch(const pqxx::sql_error& e) {
        std::cout << e.what() << std::endl;
        *query_success = false;
    }
    return *result_for_table;
}

void Postgresql1::postgresql_turn_off(bool* db_on_off){
    /**
     * disconnect from db
    */
    conn_ptr->disconnect();
    *db_on_off = false; //set db to off
}