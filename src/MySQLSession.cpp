//
// Created by wsmrxd on 2023/7/11.
//

#include "MySQLSession.h"

std::string MySQLSession::test() {
    static const char* sql = "SELECT * FROM tb_user";
    boost::mysql::results results;
    sqlConnection.query(sql, results);
    return results.rows().at(0).at(1).get_string();
}
