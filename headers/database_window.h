#ifndef DATABASE_WINDOW_H
#define DATABASE_WINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QMessageBox>
#include "database.h"


namespace Ui {
class database_window;
}

class database_window : public QWidget
{
   Q_OBJECT

public:
   explicit database_window(QWidget *parent = nullptr);


   ~database_window();


private slots:
   void on_toolButton_close_clicked(); // кнопка закрытия окна

   void on_toolButton_hide_clicked(); // кнопка скрытия окна

   void on_pushButton_connect_clicked(); // кнопка подключения к БД

   void on_pushButton_pressed();

   void on_pushButton_released();

private:
   Ui::database_window *ui;


   // ДЛЯ ПЕРЕМЕЩЕНИЯ ОКНА
   void mousePressEvent(QMouseEvent *event) override; // обрабатываем событие нажатия кнопки мыши
   void mouseMoveEvent(QMouseEvent* event) override; // обрабатываем событие перемещения мыши

   QPointF m_dragPosition;

   database* dms; // объект базы данных

   void fill_input_fields(); // заполнение полей ввода входа в бд данными из кэша
};

#endif // DATABASE_WINDOW_H
