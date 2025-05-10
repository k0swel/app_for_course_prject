#ifndef WINDOW_QUERY_H
#define WINDOW_QUERY_H

#include <QWidget>
#include <QMouseEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QString>
#include "database.h"

namespace Ui {
class window_query;
}

class window_query; // прототип класса



class window_query : public QWidget
{
   Q_OBJECT

public:
   static window_query* create_instance(database* db, QWidget* parent = nullptr);
   ~window_query();

private slots:
   void on_toolButton_close_clicked();

   void on_toolButton_hide_clicked();

   void on_pushButton_send_query_clicked();

private:
   // делаем класс синглтоном
   explicit window_query(database* db, QWidget *parent = nullptr);
   static window_query* instance;
   window_query(const window_query&) = delete; // удаляем конструктор копирования
   window_query& operator=(const window_query&) = delete; // удаляем оператор присваивания
   database* db; // объект базы данных
   Ui::window_query *ui; // объект виджета окна
   QPointF m_dragPosition; // сохранение координаты мыши при нажатии на экран
   void mousePressEvent(QMouseEvent *event) override; // обрабатываем событие нажатия кнопки мыши
   void mouseMoveEvent(QMouseEvent* event) override; // обрабатываем событие перемещения мыши
   void closeEvent(QCloseEvent* event) override; // обрабатываем событие, когда пользователь хочет закрыть окно

   QString get_table_name(QString& query); // имя таблицы SQL
};

#endif // WINDOW_QUERY_H
