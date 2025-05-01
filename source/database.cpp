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
   return true;
}

QSqlDatabase& database::get_db()
{
   return this->db; // возвращаю объект базы данных
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
