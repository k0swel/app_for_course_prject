#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class notification;
}

class notification : public QWidget
{
   Q_OBJECT

public:
   ~notification();
   static notification* create_instance(QString title, QString text, QWidget *parent = nullptr);

private slots:
   void on_pushButton_close_clicked();

private:
   static notification* instance;

   Ui::notification *ui;
   void update_progress_bar();
   explicit notification(QString title, QString text, QWidget *parent = nullptr); // прячем конструктор в приват, потому окно - синглтон
   notification(const notification&) = delete; // удаляем конструктор копирования
   notification& operator=(const notification&) = delete; // удаляем оператор присваивания
   int current_value_progress_bar = 0; // значение прогресс бара
   void close_window(); // закрываем окно при нажатии на клавишу
   QTimer* timer; // таймер для прогресс бара
   QString text; // текст для окна.
   void mousePressEvent(QMouseEvent* event) override; // переопределяем событие клика мыши
   void mouseMoveEvent(QMouseEvent* event) override; // переопределяем событие передвижения мыши
   QPoint last_press_position; // переменная хранит последнюю позицию при нажатии на ЛКМ
};

#endif // NOTIFICATION_H
