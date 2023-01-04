#include "sqlhelper.h"
#include "common/config.h"
#include <QSqlError>
#include <QSqlTableModel>
#include <QTableView>

SqlHelper* SqlHelper::m_sql_helpher_instance=nullptr;
QMutex SqlHelper::m_mutex;

SqlHelper *SqlHelper::getInstance() {
    if (m_sql_helpher_instance == nullptr) {
        m_mutex.lock();
        if (m_sql_helpher_instance == nullptr) {
            m_sql_helpher_instance = new SqlHelper();
        }
        m_mutex.unlock();
    }
    return m_sql_helpher_instance;
}

SqlHelper::SqlHelper()
{
    createConnection();
}


void SqlHelper::createConnection()
{
    connection = QSqlDatabase::addDatabase("QSQLITE");
    connection.setDatabaseName(DB_FILE);
    if (!connection.open()) {
        qDebug()<<"Cannot open database :"<<DB_FILE;
    }
}

void SqlHelper::closeConnection()
{
    if(connection.isOpen()){
        connection.close();
    }
}


void SqlHelper::deleteDb()
{

}

QVector<QString> SqlHelper::getAllTableName()
{
    /*
    SQLite数据库中一个特殊的表叫 sqlite_master，sqlite_master的结构。
    CREATE TABLE sqlite_master (
    type TEXT,
    name TEXT,
    tbl_name TEXT,
    rootpage INTEGER,
    sql TEXT
    );
    我们可以通过查询这个表来获取数据库所有的表名：
    SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;
    */
    QVector<QString> result;
    QString sqlStr = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name";
    QSqlQuery query(connection);
    if(!query.exec(sqlStr)){
        lastSqlString = sqlStr;
        lastErrorText = query.lastError().text();
        qDebug()<<"SQL string :" + lastSqlString +"\n"+ "Error text:"+lastErrorText;
    }
    //遍历表名

    while ( query.next() ) {
        result.append(query.value(0).toString());
    }
    return result;

}
/**
 * @brief SqlHelp::createTable 创建表
 * @param tableName 表名称
 * @param fdNameTypePairs 字段名，字段值类型对。比如表字段为 id，name，类型分别为int primary key,varchar
 * 那么 fdNameTypePairs 数组的值依次设置为"id","int primary key","name","varchar"
 */
void SqlHelper::createTable(QString tableName, QVector<QString> fdNameTypePairs)
{
    /**
    QVector<QString> fdDeclareList;
    fdDeclareList<<"id"<<"int primary key";
    fdDeclareList<<"name"<<"varchar";
    fdDeclareList<<"color"<<"varchar";
    fdDeclareList<<"picture"<<"varchar";
    fdDeclareList<<"barcode"<<"int";
    fdDeclareList<<"stage"<<"varchar";
    h.createTable("recipe",fdDeclareList);
    */
    //query1.exec("create table student (id int primary key, name varchar(20))");
    //检查表是否已存在
    QVector<QString> existTables = getAllTableName();
    if(existTables.indexOf(tableName) >= 0){
        return;
    }
    QString createStr = "create table " + tableName;
    QString nameTypeStr ;
    if( fdNameTypePairs.count() % 2 ){
        qDebug()<<"fdNameTypePairs count error";
    }
    for (int i = 0; i < fdNameTypePairs.count(); i+=2) {
        nameTypeStr += fdNameTypePairs.at(i) + " " + fdNameTypePairs.at(i+1) + ",";
    }
    nameTypeStr = nameTypeStr.mid(0,nameTypeStr.length() -1);//remove last ","
    QString sqlStr = createStr + " (" + nameTypeStr +")";
    execSql(sqlStr);
}
/**
 * @brief SqlHelp::deleteTable 删除表
 * @param tableName
 */
void SqlHelper::deleteTable(QString tableName)
{
    QString sqlStr = "drop table " + tableName;
    execSql( sqlStr) ;
}
/**
 * @brief SqlHelp::addRecord 向表中添加一条记录
 * @param tableName 操作的表名
 * @param fdNameValuePairs 字段名，字段值对。比如添加项为 id=1 name="zhangshan"
 * 那么fdNameValuePairs数组的值依次设置为"id","1","name","'zhangshan'",注意字符串类型的值要加上'
 */
void SqlHelper::addRecord(QString tableName, QVector<QString> fdNameValuePairs)
{
    //INSERT INTO table_name (列1, 列2,...) VALUES (值1, 值2,....)
    //QString("INSERT INTO %1 ( %2 ) VALUES ( %3 )").arg(tableName).arg(nameStr).arg(valueStr);

    if( fdNameValuePairs.count() % 2 ){
        qDebug()<<"fdNameValuePairs count error";
        return;
    }
    QVector<QString> names;
    QVector<QString> values;
    for (int i = 0; i < fdNameValuePairs.count(); i+=2) {
        names.append(fdNameValuePairs.at(i));
        values.append(fdNameValuePairs.at(i+1));
    }
    QString nameStr;
    QString valueStr;
    //拼接 name
    for (int i = 0; i < names.count(); ++i) {
        nameStr += names.at(i) + ",";
    }
    nameStr = nameStr.mid(0,nameStr.length()-1);//remove last ","
    //拼接 value
    for (int i = 0; i < values.count(); ++i) {
        valueStr += values.at(i) + ",";
    }
    valueStr = valueStr.mid(0,valueStr.length()-1);//remove last ","
    //sqlStr = actionStr + " (" + nameStr +") VALUES (" + valueStr +")";
    QString sqlStr = QString("INSERT INTO %1 ( %2 ) VALUES ( %3 )").arg(tableName).arg(nameStr).arg(valueStr);
    execSql(sqlStr);
}


void SqlHelper::delRecord(QString tableName,QString condition)
{
    //DELETE FROM 表名称 WHERE 列名称 = 值
    QString sqlStr;
    if(condition.isEmpty()){
        sqlStr = QString("DELETE FROM %1").arg(tableName);
    }else{
        sqlStr = QString("DELETE FROM %1 WHERE %2").arg(tableName).arg(condition);
    }
    execSql(sqlStr);
}

void SqlHelper::setRecord(QString tableName, QVector<QString> fdNameValuePairs, QString condition)
{
    //参数说明
    //condition为空时，应用到所有列
    //返回值
    //更新某一行中的若干列
    //UPDATE Person SET Address = 'Zhongshan 23', City = 'Nanjing' WHERE LastName = 'Wilson'
    //QString("UPDATE %1 SET %2 WHERE %3").arg(tableName).arg(nameValueStr).arg(condition);
    QString nameValueStr;
    if( fdNameValuePairs.count() % 2 ){
        qDebug()<<"fdNameValuePairs count error";
        return;
    }
    QVector<QString> names;
    QVector<QString> values;
    for (int i = 0; i < fdNameValuePairs.count(); i+=2) {
        names.append(fdNameValuePairs.at(i));
        values.append(fdNameValuePairs.at(i+1));
    }
    //拼接 name = value
    for (int i = 0; i < names.count(); ++i) {
        nameValueStr += names.at(i) + " = " + values.at(i) + ",";
    }
    nameValueStr = nameValueStr.mid(0,nameValueStr.length()-1);//remove last ","
    QString sqlStr ;
    if(condition.isEmpty()){
        sqlStr = QString("UPDATE %1 SET %2 ").arg(tableName).arg(nameValueStr);
    }else{
        sqlStr = QString("UPDATE %1 SET %2 WHERE %3").arg(tableName).arg(nameValueStr).arg(condition);
    }
    execSql( sqlStr) ;
}

QVector<QString> SqlHelper::getFirstRecord(QString tableName, QVector<QString> fdNames,QString condition)
{
    //condition为空时，应用到所有列
    //SELECT LastName,FirstName FROM Persons
    QVector<QString> fdValues;
    QString nameStr = vectorToString(fdNames, ",");
    QString sqlStr;
    if(condition.isEmpty()){
        sqlStr = QString("SELECT %1 FROM %2 ").arg(nameStr).arg(tableName);
    }else{
        sqlStr = QString("SELECT %1 FROM %2 WHERE %3").arg(nameStr).arg(tableName).arg(condition);
    }
    QSqlQuery query(connection);

    if(!query.exec(sqlStr)){
        lastSqlString = sqlStr;
        lastErrorText = query.lastError().text();
        qDebug()<<"SQL string :" + lastSqlString +"\n"+ "Error text:"+lastErrorText;
    }
    if( query.first() ){
        for (int i = 0; i < fdNames.count(); ++i) {
            fdValues.append(query.value(i).toString());
        }
        return fdValues;
    }
    return fdValues;
}

void SqlHelper::execSql(const QString sqlStr)
{
    if(!connection.isOpen()){
        qDebug()<<"connection is not open";
        return;
    }
    QSqlQuery query(connection);
    if(!query.exec(sqlStr)){
        lastSqlString = sqlStr;
        lastErrorText = query.lastError().text();
        qDebug()<<"SQL string :" + lastSqlString +"\n"+ "Error text:"+lastErrorText;
    }
}

QString SqlHelper::getLastSqlString() const
{
    return lastSqlString;
}

QString SqlHelper::getLastErrorText() const
{
    return lastErrorText;
}

QString SqlHelper::vectorToString(const QVector<QString> &list, QString split) const
{
    QString result;//转化成val1,val2,val3
    for (int i = 0; i < list.count(); ++i) {
        result += list.at(i) + split;
    }
    int index = result.lastIndexOf(split);
    result.remove(index,split.length());
    return result;
}

void SqlHelper::delete_sqlhelper()
{
    closeConnection();
}

