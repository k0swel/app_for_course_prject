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
   explicit table_form(QSqlQueryModel* table_model, QWidget *parent = nullptr);
   ~table_form();

private slots:
   void on_toolButton_close_clicked();

   void on_toolButton_hide_clicked();

private:
   Ui::table_form *ui;
   // ПЕРЕОПРЕДЕЛЕНИЕ СОБЫТИЙ
   QPoint last_mouse_position;
   void mousePressEvent(QMouseEvent *event); // переопределяем событие зажатия клавиши мышки на экране
   void mouseMoveEvent(QMouseEvent* event); // отслеживаем движение мыши с зажатой клавишей
};

#endif // TABLE_FORM_H
