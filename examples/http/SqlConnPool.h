#ifndef SQLCONNPOOL_H
#define SQLCONNPOOL_H

#include <mysql/mysql.h>

#include <queue>
#include <mutex>
//#include <condition_variable>

typedef MYSQL* SqlHandler_t;

class SqlConnPool
{
public:
    static SqlConnPool* Instance();

    bool Init(const char* host, int port,
              const char* user,const char* pwd, 
              const char* dbName, int connSize);
    void ClosePool();

    SqlHandler_t GetConn();
    void FreeConn(SqlHandler_t handle);
    int GetFreeConnCount();

private:
    SqlConnPool();
    ~SqlConnPool();

    int MAX_CONN_;
    int useCount_;
    int freeCount_;

    std::queue<SqlHandler_t> connQue_;
    std::mutex mutex_;
    //std::condition_variable cond_;
};

class SqlConnRAII {
public:
    SqlConnRAII(SqlHandler_t* sql, SqlConnPool* connpool)
    {
        *sql = connpool->GetConn();
        sql_ = *sql;
        connpool_ = connpool;
    }
    
    ~SqlConnRAII() {
        if (sql_) { connpool_->FreeConn(sql_); }
    }

private:
    SqlHandler_t sql_;
    SqlConnPool* connpool_;
};

#endif // SQLCONNPOOL_H