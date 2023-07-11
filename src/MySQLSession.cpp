//
// Created by wsmrxd on 2023/7/11.
//

#include "MySQLSession.h"

void MySQLSession::test(const char *name, boost::mysql::results& result) {
    static const char* sql = "SELECT * FROM tb_user WHERE username = ?";
    sqlConnection.async_prepare_statement(sql, diag,
        [this, name, &result](boost::system::error_code ec, boost::mysql::statement tempStmt){
            boost::mysql::throw_on_error(ec, diag);
            sqlConnection.async_execute_statement(tempStmt, std::make_tuple(name), result, diag,
                [this](boost::system::error_code ec){
                    boost::mysql::throw_on_error(ec, diag);
                });
        });
}

std::size_t MySQLSession::selectUserByName(std::string_view username) {
    static const char* sql = "SELECT * FROM WebIMGppUsers WHERE username = ?";
    boost::mysql::results result;
    sqlConnection.execute_statement(blockingPrepare(sql), std::make_tuple(username.data()), result);
    return result.rows().size();
}

boost::mysql::statement MySQLSession::blockingPrepare(std::string_view sql) {
    return sqlConnection.prepare_statement(sql);
}
