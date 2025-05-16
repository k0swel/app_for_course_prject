#include "window_query.h"
#include "ui_window_query.h"
#include <QSqlRecord>
#include <QTableView>
#include <QSqlQueryModel>
#include <table_form.h>
#include "notification.h"
#include "database_window.h"
#include <chrono>
#include <QSqlField>
#include <QSqlResult>

window_query* window_query::instance = nullptr;


// СИНГЛТОН

window_query* window_query::create_instance(database* db,QWidget* parent) {
   if (window_query::instance == nullptr) {
      window_query::instance = new window_query(db,parent); // если объекта класса не существует, то создаём его
   }
   return window_query::instance; // возвращаем единственный объект класса
}

window_query::~window_query()
{
   qDebug() << QString("Удален объект класса window_query( %1 )").arg((quintptr)this); // выводим удаленный объект класса
   delete ui;
}


window_query::window_query(database* db, QWidget *parent) :
   QWidget(parent),
   ui(new Ui::window_query)
{
   ui->setupUi(this);
   this->setWindowTitle("Запрос к базе данных"); // устанавливаем заголовок окна.
   this->setWindowFlags(    Qt::Window |
                           Qt::WindowTitleHint |
                           Qt::WindowCloseButtonHint |
                           Qt::WindowMinimizeButtonHint ); // прячем кнопку сворачивания
   this->db = db; // присваиваем объект базы данных
   this->setAttribute(Qt::WA_DeleteOnClose); // уничтожаем объект при его закрытии
   qDebug() << QString("Создан объект класса window_query( %1 )").arg((qintptr)this); // выводим удаленный объект класса
}


void window_query::mousePressEvent(QMouseEvent *event)  { // обработчик события фиксирования позиции мыши при нажатии
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition() - this->frameGeometry().topLeft();
        event->accept(); // продолжаем выполнять событие
    }
}

void window_query::mouseMoveEvent(QMouseEvent *event)  { // обработчик события перемещения окна
    if (event->buttons() & Qt::LeftButton) {
        move((event->globalPosition() - m_dragPosition).x(), (event->globalPosition() - m_dragPosition).y());
        event->accept(); // продолжаем выполнять событие
    }
}

void window_query::closeEvent(QCloseEvent *event) // обработчик события закрытия окна
{
   window_query::instance = nullptr; // обнуляем указатель на окно
   new database_window(); // открываем главное окно.
   event->accept(); // закрываем окно
}

QString window_query::get_table_name(QString& query)
{
   QString table_name = NULL; // возвращаемая переменная
   if (query.contains("FROM", Qt::CaseInsensitive)) {
      QString query = ui->lineEdit_query->toPlainText();
      QStringList words = query.split(' ', Qt::SkipEmptyParts);
      short int index_from = words.indexOf("FROM",0,Qt::CaseInsensitive); // ищем индекс первого вхождения FROM
      table_name = words[index_from+1]; // получаем имя таблицы
      table_name.remove(';'); // удаляем символ ;
      table_name.remove('"'); // удаляем символ "
   }
   return table_name; // возвращаем имя таблицы
}


void window_query::on_toolButton_close_clicked()
{
   this->close(); // отправляем сигнал на закрытие окна
}


void window_query::on_toolButton_hide_clicked()
{
   double start_opacity = 1; // начальный коэф. прозрачности
   while (start_opacity > 0) {
      start_opacity -= 0.0005; // постепенно уменьшаем коэф. прозрачности
      this->setWindowOpacity(start_opacity);
   }
   this->showMinimized(); // скрываем окно
   this->setWindowOpacity(1); // снова ставим коэф. прозрачности
}

void window_query::on_pushButton_send_query_clicked() // нажата кнопка отправки запроса
{
   QSqlQueryModel* table_model = new QSqlQueryModel(this);
   if (ui->radioButton_select->isChecked()) {
      QString string_query = ui->lineEdit_query->toPlainText();
      if (this->db->query.exec(string_query)) {
         QSqlQuery result(this->db->query); // результат SQL-запроса
         table_model->setQuery(result); // заполняем модель значениями из SQL-запроса.
         QMap<QString, QString> type_field; // map для хранения типов атрибутов
         QString table_name = get_table_name(string_query); // получаем имя таблицы
         if (!table_name.isNull()) { // если имя таблицы не равно NULL
            for (int i = 0; i < result.record().count(); i++) {
               QString field_name = result.record().fieldName(i);
               this->db->query.exec(QString("SELECT data_type FROM information_schema.columns WHERE table_name = '%1' AND column_name = '%2'").arg(table_name).arg(field_name));
               while (this->db->query.next()) {
                  type_field[field_name] = this->db->query.value(0).toString(); // вытаскиваем тип из результата запроса
               }
            }
         }
         table_form* table_window = new table_form(this->db, this, table_name, table_model, type_field); // создаём окно с таблицей
         table_window->show(); // показываем окно на экране.
         this->hide();
      }
      else {
         notification::create_instance("Ошибка запроса", this->db->query.lastError().text()); // выводим уведомление об ошибке.
      }
   }
   else if (ui->radioButton_other->isChecked()) { // если выбрал радиобаттон на поиск исполняемых запросов.
      if (ui->lineEdit_query->toPlainText().contains("select", Qt::CaseInsensitive))
         notification::create_instance("Ошибка", "Вы ввели запрос на выборку данных с настройкой \"Манипулирование данными\"");
      else {
         if (this->db->query.exec(ui->lineEdit_query->toPlainText())) {
            qInfo() << "Запрос OTHER выполнен успешно!";
         }
         else {
            notification::create_instance("Ошибка запроса", this->db->query.lastError().text()); // выводим уведомление об ошибке
         }
      }
   }
}

