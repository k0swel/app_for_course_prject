#include "table_form.h"
#include "ui_table_form.h"

table_form::table_form(QSqlQueryModel* table_model, QWidget *parent) :
   QWidget(parent),
   ui(new Ui::table_form)
{
   ui->setupUi(this);
   this->setAttribute(Qt::WA_DeleteOnClose); // уничтожаем объект окна при его закрытии.
   this->setWindowFlag(Qt::WindowType::FramelessWindowHint); // убираем рамки у окна.
   ui->tableView_from_database->setModel((QAbstractItemModel*)table_model); // заполняем таблицу информацией из SQL - запроса.
   ui->tableView_from_database->resizeColumnsToContents(); // подстраиваем размер колонок под содержимое контента.
}

table_form::~table_form()
{
   delete ui;
}

void table_form::mousePressEvent(QMouseEvent *event)
{
   if (event->buttons() & Qt::LeftButton) {
      this->last_mouse_position = event->pos(); // сохраняем позицию мыши
   }
}

void table_form::mouseMoveEvent(QMouseEvent *event)
{
   if (event->buttons() & Qt::LeftButton) {
      this->move(event->globalPosition().toPoint() - this->last_mouse_position); // перемещаем окно
   }
}

void table_form::on_toolButton_close_clicked()
{
   this->close(); // закрываем окно при нажатие на pushbutton
}


void table_form::on_toolButton_hide_clicked()
{
   double start_opacity = 1;
   for (int i = 0; i < 10000; i++) {
      start_opacity -= 0.0005;
      this->setWindowOpacity(start_opacity);
   }
   this->showMinimized();
   this->setWindowOpacity(1); // плавно прячем окно.
}

