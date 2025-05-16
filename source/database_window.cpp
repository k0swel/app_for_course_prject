#include "database_window.h"
#include "ui_database_window.h"
#include <QMessageBox>
#include "database.h"
#include "window_query.h"
#include "notification.h"
#include "json_manage.h"



database_window::database_window(QWidget *parent) :
   QWidget(parent),
   ui(new Ui::database_window)
{
   ui->setupUi(this);
   this->setWindowTitle("Подключение к базе данных"); // заголовок
   this->setWindowFlags(    Qt::Window |
                           Qt::WindowTitleHint |
                           Qt::WindowCloseButtonHint |
                           Qt::WindowMinimizeButtonHint ); // прячем кнопку сворачивания
   connect(this, &database_window::destroyed, this, &QObject::deleteLater); // при закрытии окна уничтожаем его объект.
   this->dms = new database; // инициализируем объект БД
   fill_input_fields(); // заполняем поля ввода из json файла.
   this->show();
}

void database_window::mousePressEvent(QMouseEvent *event)  { // обработчик события фиксирования позиции мыши при нажатии
    if (event->button() == Qt::LeftButton) {
        m_dragPosition = event->globalPosition() - this->frameGeometry().topLeft();
        event->accept(); // продолжаем выполнять событие
    }
}

void database_window::mouseMoveEvent(QMouseEvent *event)  { // обработчик события перемещения окна
    if (event->buttons() & Qt::LeftButton) {
        move((event->globalPosition() - m_dragPosition).x(), (event->globalPosition() - m_dragPosition).y());
        event->accept(); // продолжаем выполнять событие
    }
}

// заполнение полей ввода входа в бд данными из кэша
void database_window::fill_input_fields()
{
   std::filesystem::path path_to_json_file("./login_data.json");
   if (std::filesystem::exists(path_to_json_file)) {
      QString address, username, database_name, password;
      int port;
      read_from_json(address,database_name,password,port,username);
      ui->lineEdit_database_name->setText(database_name);
      ui->lineEdit_ip_database->setText(address);
      ui->lineEdit_user->setText(username);
      ui->lineEdit_port->setText(QString::number(port));
      ui->lineEdit_password->setText(password);
   }
}

database_window::~database_window()
{
   delete ui;
   delete this->dms; // удаляем объект базы данных
}


// нажатие кнопки подключения к базе данных
void database_window::on_pushButton_connect_clicked()
{
   QString&& address = ui->lineEdit_ip_database->text(); // узнаём IP-адрес
   int port = ui->lineEdit_port->text().toInt(); // узнаём порт
   qDebug() << "port = " << port;
   QString&& username = ui->lineEdit_user->text(); // узнаём имя пользователя
   QString&& password = ui->lineEdit_password->text(); // узнаём пароль пользователя
   QString&& database_name = ui->lineEdit_database_name->text(); // узнаём имя базы данных
   if (dms->connect_to_database(address, database_name, port, username, password)) {
      window_query::create_instance(this->dms)->show(); // если подключение к БД успешно
      this->close(); // закрываем текущее окно.

      if (ui->checkBox_remember_me->isChecked()) {
         if (!write_to_json(address, database_name, password, port, username)) // если данные от БД не записались в JSON
         {
            notification::create_instance("Ошибка", "Произошла ошибка при записи в JSON-файл.");
         }
      }
   }
   else {
      notification::create_instance("Ошибка при подключении", dms->get_db().lastError().text());
   }
}

// при зажатом pushbutton отображаем наш пароль
void database_window::on_pushButton_pressed()
{
   ui->lineEdit_password->setEchoMode(QLineEdit::EchoMode::Normal);
}



// при отжатом pushbutton прячем наш пароль
void database_window::on_pushButton_released()
{
   ui->lineEdit_password->setEchoMode(QLineEdit::EchoMode::Password);
}

