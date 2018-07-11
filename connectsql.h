#ifndef CONNECTSQL_H
#define CONNECTSQL_H
#include <QTextStream>
class connectSql{
private:
    QString SIP;
    QString DbName;
    int Port;
    QString UserName;
    QString SPwd;

public:
   // connectSql();
    void setSIP(QString sip);
    void setDbName(QString dbname);
    void setPort(int port);
    void setUserName(QString username);
    void setSPwd(QString spwd);
    QString getSIP();
    QString getDbName();
    int getPort();
    QString getUserName();
    QString getSPwd();
//    connectSql *con;
};

#endif // CONNECTSQL_H
