#include <QCoreApplication>
#include <QApplication>
#include "database_window.h"
#include "table_form.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   database_window* main_window = new database_window; // показываем главное окно программы.
   return a.exec();
}
