#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QObject>
#include <QSQlError>
#include <QSqlQuery>
#include <QThread>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonObject>

class database: public QObject
{
public:
   database();
   ~database();
   bool connect_to_database(QString& ip_address, QString& database_name, int port, QString& username, QString& password);
   QSqlQuery query; // объект запроса к БД
   QSqlDatabase& get_db();
   QJsonObject database_data; // информация о БД (адрес, порт, имя пользователя, имя базы данных, пароль пользователя)
private:
   QSqlDatabase db; // объект СУБД
   QJsonObject fill_json(QString& ip_address, QString& database_name, int port, QString& username, QString& password); // заполняем database_data
};

void check_connect_state(QSqlDatabase* db);


#endif // DATABASE_H
