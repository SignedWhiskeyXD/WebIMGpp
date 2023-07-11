//
// Created by wsmrxd on 2023/7/11.
//

#include "MySQLSession.h"

void MySQLSession::test(const char *name, boost::mysql::results& result) {
    static const char* sql = "SELECT * FROM tb_user WHERE username = ?";
    sqlConnection.async_prepare_statement(sql, diag,
        [this, name, &result](boost::system::error_code ec, boost::mysql::statement tempStmt){
            boost::mysql::throw_on_error(ec, diag);
            nextStmt = tempStmt;
            executeTest(name, result);
        });
}

void MySQLSession::executeTest(const char *name, boost::mysql::results& result) {
    std::tuple<std::string> t(name);
    sqlConnection.async_execute_statement(nextStmt, t, result, diag,
        [this](boost::system::error_code ec){
            boost::mysql::throw_on_error(ec, diag);
        });
}
