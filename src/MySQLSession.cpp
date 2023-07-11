//
// Created by wsmrxd on 2023/7/11.
//

#include "MySQLSession.h"
#include <spdlog/spdlog.h>

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

bool MySQLSession::checkUsername(std::string_view username) {
    static const char* sql = "SELECT * FROM WebIMGppUsers WHERE username = ?";
    boost::mysql::results result;
    sqlConnection.execute_statement(blockingPrepare(sql), std::make_tuple(username.data()), result);
    return !result.rows().empty();
}

WebIMGppUser MySQLSession::selectUser(std::string_view username) {
    static const char* sql = "SELECT * FROM WebIMGppUsers WHERE username = ?";
    boost::mysql::results result;
    sqlConnection.execute_statement(blockingPrepare(sql), std::make_tuple(username.data()), result);
    if(result.rows().empty()) return WebIMGppUser();

    return WebIMGppUser{result.rows().at(0).at(0).get_uint64(),
                        result.rows().at(0).at(1).get_string(),
                        result.rows().at(0).at(2).get_string()};
}

bool MySQLSession::addUser(std::string_view username, std::string_view password) {
    static const char* sql = "INSERT INTO WebIMGppUsers (username, password) VALUES (?, ?)";
    boost::mysql::results result;

    try{
        sqlConnection.execute_statement(blockingPrepare(sql),
                                        std::make_tuple(username.data(), password.data()),result);
    }catch (const std::exception& e){
        spdlog::error("Exception thrown on adding user: {}", e.what());
        return false;
    }
    return true;
}

boost::mysql::statement MySQLSession::blockingPrepare(std::string_view sql) {
    return sqlConnection.prepare_statement(sql);
}


