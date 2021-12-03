#include "SqlConnPool.h"


SqlConnPool* SqlConnPool::Instance()
{
    static SqlConnPool connPool;
    return &connPool;
}

bool SqlConnPool::Init(const char* host, int port,
            const char* user,const char* pwd, const char* dbName,
            int connSize = 10)
{
    // internal use, not check args
    for (int i = 0; i < connSize; i++) {
        SqlHandler_t sql = nullptr;
        sql = mysql_init(sql);
        if (!sql) {
            //LOG_ERROR("MySql init error!");
            return false;
        }

        sql = mysql_real_connect(sql, host, user, pwd, 
                                 dbName, port, nullptr, 0);
        if (!sql) {
            //LOG_ERROR("MySql Connect error!");
            return false;
        }

        connQue_.push(sql);
    }

    MAX_CONN_ = connSize;
    return true;
}

void SqlConnPool::ClosePool() {
    std::unique_lock<std::mutex> lock(mutex_);

    while (!connQue_.empty()) {
        auto item = connQue_.front();
        connQue_.pop();
        mysql_close(item);
    }
    mysql_library_end();
}

SqlHandler_t SqlConnPool::GetConn() {
    SqlHandler_t sql = nullptr;
    std::unique_lock<std::mutex> lock(mutex_);

    if (connQue_.empty()){
        //LOG_WARN("SqlConnPool busy!");
        return nullptr;
    }
    
    sql = connQue_.front();
    connQue_.pop();
    return sql;
}

void SqlConnPool::FreeConn(SqlHandler_t sql) {
    if (nullptr == sql)
        return;

    std::unique_lock<std::mutex> lock(mutex_);
    connQue_.push(sql);
}

int SqlConnPool::GetFreeConnCount() {
    std::unique_lock<std::mutex> lock(mutex_);
    return connQue_.size();
}

SqlConnPool::SqlConnPool()
            : useCount_(0)
            , freeCount_(0)
{
}

SqlConnPool::~SqlConnPool()
{
    ClosePool();
}
