#ifndef CONNECTIONPOOL_H
#define CONNECTIONPOOL_H
/********************/
//数据库连接池——单例模式
/********************/
#include <QtSql>
#include <QQueue>
#include <QString>
#include <QMutex>
#include <QMutexLocker>

class ConnectionPool
{
public:
    static ConnectionPool& getInstance();//返回实例的引用
    QSqlDatabase openConnection();//从连接池获取连接
    void closeConnection(QSqlDatabase connection);//连接放回连接池
    void releasePool();//释放连接池
    ~ConnectionPool();//释放队列
private:
    ConnectionPool();

    QSqlDatabase createConnection(const QString connName);//创建数据库连接
    QQueue<QString> usedConnections;//已使用数据库连接名
    QQueue<QString> unusedConnections;//未使用数据库连接名

    //数据库信息
    QString hostName;
    qint32 port;
    QString databaseName;
    QString userName;
    QString passWord;
    QString databaseType;

    bool testConnection;//取得连接时测试连接是否有效
    QString testConnectionSql;//测试连接的SQL语句

    int maxWaitTime;//获取连接的最大等待时间
    int waitInterval;//每次尝试获取连接的等待间隔
    int maxConnections;//连接池最大连接数

    static QMutex hMutex;//互斥锁
    static QWaitCondition waitConnection;//等待可用连接
    static ConnectionPool *instance;//唯一实例
};

#endif // CONNECTIONPOOL_H
