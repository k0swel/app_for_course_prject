#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QObject>
#include <QSQlError>
#include <QSqlQuery>
#include <QThread>

class database: public QObject
{
public:
   database();
   ~database();
   bool connect_to_database(QString& ip_address, QString& database_name, int port, QString& username, QString& password);
   QSqlQuery query; // объект запроса к БД
   QSqlDatabase& get_db();
private:
   QSqlDatabase db; // объект СУБД
};

void check_connect_state(QSqlDatabase* db);


#endif // DATABASE_H
