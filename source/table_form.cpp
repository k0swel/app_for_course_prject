#include "table_form.h"
#include "ui_table_form.h"

table_form::table_form(QSqlQueryModel* table_model, QMap<QString, QString> type_fields, short int time_msec, QWidget *parent) :
   QWidget(parent),
   ui(new Ui::table_form)
{
   ui->setupUi(this);
   this->setAttribute(Qt::WA_DeleteOnClose); // уничтожаем объект окна при его закрытии.
   ui->tableView_from_database->setModel((QAbstractItemModel*)table_model); // заполняем таблицу информацией из SQL - запроса.
   this->time_in_msec = time_msec; // сохраняем время выполнения SQL-запроса в свойство класса.
   this->type_fields = type_fields; // сохраняем названия атрибутов и соответствующие им типы.
   this->set_label_info(); // устанавливаем информацию о запросе.
   this->change_header_data(); // изменяем названия заголовков таблицы (добавляем типы данных).
   ui->tableView_from_database->resizeColumnsToContents(); // подстраиваем размер столбцов под содержимое
   ui->tableView_from_database->horizontalHeader()->setStretchLastSection(true); // последний столбец расширяем до ширины виджета таблицы.
}

table_form::~table_form()
{
   delete ui;
}

void table_form::set_label_info()
{
   qInfo() << "Итоговое количество строк: " << ui->tableView_from_database->model()->rowCount() << "; столбцов: " << ui->tableView_from_database->model()->columnCount() << " всего ячеек: " << ui->tableView_from_database->model()->rowCount() * ui->tableView_from_database->model()->columnCount();
   QString&& column_count = QString::number(ui->tableView_from_database->model()->columnCount()); // количество столбцов
   QString&& row_count = QString::number(ui->tableView_from_database->model()->rowCount()); // количество строк
   QString&& elements_count = QString::number(column_count.toInt() * row_count.toInt());
   ui->label_info->setText(QString("Итоговое количество строк: %1 , столбцов: %2 , количество ячеек: %3, время выполнения запроса (в мс): %4.").arg(row_count).arg(column_count).arg(elements_count).arg(this->time_in_msec));
}

void table_form::change_header_data()
{
   for (int i = 0; i < ui->tableView_from_database->horizontalHeader()->count(); i++) {
      QString old_name_header_el = ui->tableView_from_database->model()->headerData(i, Qt::Horizontal).toString(); // хранение старого имени атрибута
      QString new_name = QString("\"%1\"\n%2").arg(old_name_header_el).arg(type_fields[old_name_header_el]);
      ui->tableView_from_database->model()->setHeaderData(i, Qt::Horizontal, new_name);
   }
}
