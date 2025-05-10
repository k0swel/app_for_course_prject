#ifndef TABLE_FORM_H
#define TABLE_FORM_H

#include <QWidget>
#include <QMouseEvent>
#include <QSqlQueryModel>


namespace Ui {
class table_form;
}

class table_form : public QWidget
{
   Q_OBJECT

public:
   explicit table_form(QSqlQueryModel* table_model, QMap<QString, QString> type_fields, short int time_msec, QWidget *parent = nullptr);
   ~table_form();

private:
   // ПЕРЕМЕННЫЕ
   Ui::table_form *ui;
   short int time_in_msec; // время выполнения SQL-запроса в мс.
   QMap<QString, QString> type_fields; // хэш-таблица, где хранятся имена атрибутов и соответствующие им значения типов данных.

   // ФУНКЦИИ
   void set_label_info(); // устанавливаем текст-статистику запроса.
   void change_header_data(); // изменяем названия заголовок таблицы.
};

#endif // TABLE_FORM_H
