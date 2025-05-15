#include "json_manage.h"

#define FILE_NAME "./login_data.json"

bool write_to_json(QString &address, QString &database_name, QString &password, int &port, QString &username)
{
   QJsonDocument doc_json;
   QJsonObject main_object;
   // заполняем json-объект
   main_object.insert("address", QJsonValue(address));
   main_object.insert("database_name", QJsonValue(database_name));
   main_object.insert("password", QJsonValue(password));
   main_object.insert("port", QJsonValue(port));
   main_object.insert("username", QJsonValue(username));
   doc_json.setObject(main_object); // присваиваем документу главный json-объект.

   std::fstream json_file;
   json_file.open(FILE_NAME, std::ios_base::out);
   if (json_file.is_open()) {
      json_file.write(doc_json.toJson(), doc_json.toJson().size()); // записываем в json_файл информацию о бд.
      json_file.flush(); // сбрасываем буфер.
      json_file.close(); // закрываем файл.
      return true;
   }
   else {
      return false;
   }
}

void read_from_json(QString& address, QString& database_name, QString& password, int& port, QString& username)
{
   std::fstream json_file; // создаём объект чтения json
   json_file.open(FILE_NAME, std::ios_base::in); // открываем json-файл
   QString json_text; //  QString для хранения JSON
   std::string json_temp_text;
   while (getline(json_file, json_temp_text)) {
      json_text+=json_temp_text;
   }
   json_file.close(); // закрываем файл.
   QJsonObject main_obj = QJsonDocument::fromJson(json_text.toUtf8()).object(); // форматируем QString в JSON
   address = main_obj.take("address").toString(); // вытаскиваем из JSON адрес
   database_name = main_obj.take("database_name").toString(); // вытаскиваем из JSON имя базы данных
   password = main_obj.take("password").toString(); // вытаскиваем из JSON пароль
   port = main_obj.take("port").toInt(); // вытаскиваем из JSON порт
   username = main_obj.take("username").toString(); // вытаскиваем из JSON юзернейм.
   return;
}
