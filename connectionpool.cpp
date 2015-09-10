#include "connectionpool.h"
#include <QDebug>

QMutex ConnectionPool::hMutex;
QWaitCondition ConnectionPool::waitConnection;
ConnectionPool* ConnectionPool::instance = NULL;

ConnectionPool::ConnectionPool()
{
    hostName = "127.0.0.1";
    port = 3306;
    databaseName = "permission";
    userName = "root";
    passWord = "swust306";
    databaseType = "QMYSQL";

    testConnection = true;
    testConnectionSql = "SELECT 1";

    maxConnections = 10;
    waitInterval = 200;
    maxWaitTime = 1000;
}

ConnectionPool::~ConnectionPool()
{
    //删除所有连接
    foreach (QString connName, unusedConnections) {
        QSqlDatabase::removeDatabase(connName);
    }

    foreach (QString connName, usedConnections) {
        QSqlDatabase::removeDatabase(connName);
    }
}

ConnectionPool& ConnectionPool::getInstance()
{
    if(instance == NULL)
    {
        //线程同步（保证instance互斥）
        QMutexLocker locker(&hMutex);
        if(instance == NULL)
            instance = new ConnectionPool();
    }
    return *instance;
}

void ConnectionPool::releasePool()
{
    //线程同步（保证intance互斥）
    QMutexLocker locker(&hMutex);
    delete instance;
    instance = NULL;
}

QSqlDatabase ConnectionPool::openConnection()
{
    QString connName;
    //线程同步（保证used和unused队列互斥）
    QMutexLocker locker(&hMutex);
    //已创建的连接数
    int connCount = unusedConnections.size() + usedConnections.size();

    //如果连接已经用完，等待一定时间看是否有空闲连接
    for(int i=0; i<maxWaitTime && unusedConnections.size()==0 &&
        connCount == maxConnections; i+= waitInterval)
    {
        //等待已有连接空闲
        waitConnection.wait(&hMutex, waitInterval);
        //重新计算??已经到达最大了还会减少？？
        connCount = usedConnections.size() + unusedConnections.size();
    }

    if(unusedConnections.size() > 0)
    {
        //有可复用的连接
        connName = unusedConnections.dequeue();
    }
    else if(connCount < maxConnections)
    {
        //没有复用连接，但未达到最大连接数，则创建新的连接
        connName = QString("Connection-%1").arg(connCount + 1);
    }
    else
    {
        //已经达到最大连接且没有空闲连接
        qDebug()<<"Cannot create more connection!";
        return QSqlDatabase();
    }

    //获取连接
    QSqlDatabase db = createConnection(connName);

    //测试连接有效就放入used队列
    if(db.isOpen())
        usedConnections.enqueue(connName);

    return db;
}

void ConnectionPool::closeConnection(QSqlDatabase connction)
{
    QString connName = connction.connectionName();

    //如果是连接池创建的连接则放入unused队列，否则不处理
    if(usedConnections.contains(connName))
    {
        QMutexLocker locker(&hMutex);
        usedConnections.removeOne(connName);
        unusedConnections.enqueue(connName);
        //激活某个等待线程
        waitConnection.wakeOne();
    }
}

QSqlDatabase ConnectionPool::createConnection(const QString connName)
{
    if(QSqlDatabase::contains(connName))
    {
        //连接已存在，则复用
        QSqlDatabase db1 = QSqlDatabase::database(connName);

        if(testConnection)
        {
            //返回连接前测试，如果连接断开，重新建立连接
            qDebug()<<"Test on borrow for "<<connName;
            QSqlQuery query(testConnectionSql, db1);
            if(query.lastError().type() != QSqlError::NoError && !db1.open())
            {
                qDebug()<<"Open database error:"<<db1.lastError().text();
                return QSqlDatabase();
            }
        }
        return db1;
    }

    //创建新的连接
    QSqlDatabase db = QSqlDatabase::addDatabase(databaseType, connName);
    db.setHostName(hostName);
    db.setPort(port);
    db.setUserName(userName);
    db.setPassword(passWord);

    if(!db.open())
    {
        qDebug()<<"Open database error:"<<db.lastError().text();
        return QSqlDatabase();
    }
    return db;
}


