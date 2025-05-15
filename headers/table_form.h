#ifndef TABLE_FORM_H
#define TABLE_FORM_H

#include <QWidget>
#include <QMouseEvent>
#include <QSqlQueryModel>
#include "database.h"
#include "window_query.h"
#include <QCloseEvent>

namespace Ui {
class table_form;
}

class table_form : public QWidget
{
   Q_OBJECT

public:
   explicit table_form(database* db, window_query* query_window, QString table_name, QSqlQueryModel* table_model, QMap<QString, QString> type_fields, QWidget *parent = nullptr);
   ~table_form();


private slots:
   void on_pushButton_clicked(); // кнопка бекапа базы данных.

private:
   // ПЕРЕМЕННЫЕ
   Ui::table_form *ui;
   short int time_in_msec; // время выполнения SQL-запроса в мс.
   QMap<QString, QString> type_fields; // хэш-таблица, где хранятся имена атрибутов и соответствующие им значения типов данных.
   database* db; // объект базы данных.
   QString table_name; // имя таблицы, которая находится в виджете.
   window_query* query_window; // окно с вводом sql-команд.
   // ФУНКЦИИ
   void set_label_info(); // устанавливаем текст-статистику запроса.
   void change_header_data(); // изменяем названия заголовок таблицы.
   void closeEvent(QCloseEvent* event) override;
};

#endif // TABLE_FORM_H
