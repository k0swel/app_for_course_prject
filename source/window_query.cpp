#include "window_query.h"
#include "ui_window_query.h"
#include <QSqlRecord>
#include <QTableView>
#include <QSqlQueryModel>
#include <table_form.h>
#include "notification.h"

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
   this->db = db; // присваиваем объект базы данных
   this->setWindowFlag(Qt::WindowType::FramelessWindowHint); // прячем рамку окна и верхнюю шапку
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
   event->ignore(); // игнорируем дальнейшую обработку события.
   window_query::instance = nullptr; // обнуляем указатель на окно
   this->deleteLater(); // удаляем объект окна из памяти.
}


void window_query::on_toolButton_close_clicked()
{
   this->close(); // отправляем сигнал на закрытие окна
}


void window_query::on_toolButton_hide_clicked()
{
   double start_opacity = 1; // начальный коэф. прозрачности
   for (int i = 0; i < 10000; i++) {
      start_opacity -= 0.0005; // постепенно уменьшаем коэф. прозрачности
      this->setWindowOpacity(start_opacity);
   }
   this->showMinimized(); // скрываем окно
   this->setWindowOpacity(1); // снова ставим коэф. прозрачности
}

void window_query::on_pushButton_send_query_clicked() // нажата кнопка отправки запроса
{
   QSqlQueryModel* table_model = new QSqlQueryModel(this);
   if (this->db->query.exec(ui->lineEdit_query->toPlainText())) {
      qDebug() << "Запрос выполнен успешно!";
      table_model->setQuery(std::move(this->db->query));
      table_form* table_window = new table_form(table_model); // создаём окно с таблицей
      table_window->show(); // показываем окно на экране.
      qDebug() << "Количество колонок в модели: " << table_model->columnCount();
   }
   else {
      notification::create_instance("Ошибка запроса", this->db->query.lastError().text());
   }
}

