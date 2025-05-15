#include "database.h"
#include <QThread>


database::database()
{
   qDebug() << "Вызван конструктор database (" << this << ")";
   this->db = QSqlDatabase::addDatabase("QPSQL"); // инициализируем объект субд
   this->query = QSqlQuery(this->db); // инициализация объекта запросника
}

database::~database() {
   qDebug() << "Вызван деструктор database (" << this << ")";
}

bool database::connect_to_database(QString& ip_address, QString& database_name, int port, QString& username, QString& password)
{
   db.setHostName(ip_address); // устанавливаем адрес сервера, где БД
   db.setPort(port); // устанавливаем порт
   db.setDatabaseName(database_name); // устанавливаем имя базы данных
   db.setUserName(username); // устанавливаем имя пользователя
   db.setPassword(password); // устанавливаем пароль пользователя
   if (!db.open()) {
      qDebug() << QString("Ошибка при открытии БД: %1").arg(db.lastError().text()); // выводим текст ошибки при подключении к БД
      return false;
   }
   qDebug() << QString("Подключение к БД установлено успешно!"); // если мы успешно подключились к БД
   this->database_data = fill_json(ip_address, database_name, port, username, password); // заполняем json-объект
   return true;
}

QSqlDatabase& database::get_db()
{
   return this->db; // возвращаю объект базы данных
}

QJsonObject database::fill_json(QString& ip_address, QString& database_name, int port, QString& username, QString& password) {
   QJsonObject return_value;
   return_value["ip_address"] = ip_address;
   return_value["port"] = port;
   return_value["database_name"] = database_name;
   return_value["port"] = port;
   return_value["username"] = username;
   return_value["password"] = password;
   return return_value;
}

void check_connect_state(QSqlDatabase *db)
{
   while (1) {
      if (db->isOpen()) {
         qDebug() << "База данных подключена!";
      }
      else {
         qDebug() << "База данных не подключена.";
      }
      QThread::currentThread()->sleep(2);
   }
}
