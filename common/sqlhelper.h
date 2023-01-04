#ifndef SQLHELPER_H
#define SQLHELPER_H


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtWidgets>
class SqlHelper {
public:

    void closeConnection();
    void deleteDb();
    void createTable(
        QString tableName,
        QVector<QString> fdNameTypePairs); // fdNameTypePairs 字段名称，类型对列表
    void deleteTable(QString tableName);
    void addRecord(QString tableName, QVector<QString> fdNameValuePairs);
    void delRecord(QString tableName, QString condition);
    void setRecord(QString tableName, QVector<QString> fdNameValuePairs,
                   QString condition);
    QVector<QString> getFirstRecord(QString tableName, QVector<QString> fdNames,
                                    QString condition);
    QVector<QString> getAllTableName();
    QString getLastSqlString() const;
    QString getLastErrorText() const;
    QString vectorToString(const QVector<QString> &list, QString split) const;
    static SqlHelper *getInstance();
    void delete_sqlhelper();

private:
    SqlHelper();
    ~SqlHelper() = default;
    void createConnection();
    void execSql(const QString sqlStr);

    static SqlHelper *m_sql_helpher_instance;
    static QMutex m_mutex;
    QSqlDatabase connection;
    QString lastSqlString;
    QString lastErrorText;
};

#endif // SQLHELPER_H
