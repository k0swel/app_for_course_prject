#include "table_form.h"
#include "ui_table_form.h"
#include <QFileDialog>
#include <QProcess>
#include <QProcessEnvironment>
#include "notification.h"

table_form::table_form(database* database_object, window_query* window_query, QString table_name, QSqlQueryModel* table_model, QMap<QString, QString> type_fields, QWidget *parent) :
   QWidget(parent),
   ui(new Ui::table_form)
{
   ui->setupUi(this);
   this->setAttribute(Qt::WA_DeleteOnClose); // уничтожаем объект окна при его закрытии.
   ui->tableView_from_database->setModel((QAbstractItemModel*)table_model); // заполняем таблицу информацией из SQL - запроса.
   this->type_fields = type_fields; // сохраняем названия атрибутов и соответствующие им типы.
   this->set_label_info(); // устанавливаем информацию о запросе.
   this->change_header_data(); // изменяем названия заголовков таблицы (добавляем типы данных).
   ui->tableView_from_database->resizeColumnsToContents(); // подстраиваем размер столбцов под содержимое
   ui->tableView_from_database->horizontalHeader()->setStretchLastSection(true); // последний столбец расширяем до ширины виджета таблицы.
   this->resize(this->sizeHint()); // устанавливаем рекомендуемый размер.
   this->db = database_object; // устанавливаем объект базы данных в свойство класса.
   this->table_name = table_name; // присваиваем имя таблицы, которую мы просматриваем.
   this->query_window = window_query; // устанавливаем указатель на предыдущее окно.
   ui->pushButton->setToolTip("Сделать бекап таблицы");
}

table_form::~table_form()
{
   delete ui;
}

void table_form::set_label_info() // установка информации о запросе.
{
   this->setWindowTitle("Результат запроса"); // устанавливаем название окна.
   qInfo() << "Итоговое количество строк: " << ui->tableView_from_database->model()->rowCount() << "; столбцов: " << ui->tableView_from_database->model()->columnCount() << " всего ячеек: " << ui->tableView_from_database->model()->rowCount() * ui->tableView_from_database->model()->columnCount();
   QString&& column_count = QString::number(ui->tableView_from_database->model()->columnCount()); // количество столбцов
   QString&& row_count = QString::number(ui->tableView_from_database->model()->rowCount()); // количество строк
   QString&& elements_count = QString::number(column_count.toInt() * row_count.toInt());
   ui->label_info->setText(QString("Итоговое количество строк: %1 , столбцов: %2 , количество ячеек: %3.").arg(row_count).arg(column_count).arg(elements_count));
}

void table_form::change_header_data() // в заголовки встраиваем типы атрибутов.
{
   if (!this->type_fields.isEmpty()) { // если QMap не пустой
      for (int i = 0; i < ui->tableView_from_database->horizontalHeader()->count(); i++) {
         QString old_name_header_el = ui->tableView_from_database->model()->headerData(i, Qt::Horizontal).toString(); // хранение старого имени атрибута
         QString new_name = QString("\"%1\"\n%2").arg(old_name_header_el).arg(type_fields[old_name_header_el]);
         ui->tableView_from_database->model()->setHeaderData(i, Qt::Horizontal, new_name); // устанавливаем новый заголовок таблицы.
      }
   }
   else {
      return; // выходим из функции.
   }
}

void table_form::closeEvent(QCloseEvent *event)
{
   this->query_window->show(); // показываем предыдущее окно.
   event->accept(); // закрываем текущее окно.
}


void table_form::on_pushButton_clicked() // если хотим сделать бекап
{
   QString path_to_save = QFileDialog::getSaveFileName(this, "Выберите директорию для бекапа", QDir::currentPath(), tr("SQL File .sql (*.sql)"));
   qInfo() << "Путь для сохранения: " << path_to_save;
   if (path_to_save.isEmpty()) return;

   QProcess* terminal = new QProcess(this);
   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   env.insert("PGPASSWORD", db->database_data["password"].toString()); // Пароль пользователя
   terminal->setProcessEnvironment(env);

   terminal->start("pg_dump", {
       "-d", db->database_data["database_name"].toString(),
       "-h", db->database_data["host"].toString(),
       "-p", db->database_data["port"].toString(),
       "-U", db->database_data["username"].toString(),
       "-t", this->table_name,
       "-f", path_to_save,
       "-w"
   });
   if (!terminal->waitForStarted()) {
       qCritical() << "Ошибка запуска pg_dump:" << terminal->errorString();
       notification::create_instance("Ошибка", terminal->errorString()); // вызываем всплывающее уведомление с ошибкой.
       return;
   }

   if (terminal->exitCode() != 0) {
       qCritical() << "Ошибка pg_dump:" << terminal->readAllStandardError();
   } else {
       qInfo() << "Бекап успешно создан:" << path_to_save;
   }

}

