#ifndef JSON_MANAGE_H
#define JSON_MANAGE_H
#include <QJsonDocument>
#include <QJsonObject>
#include <filesystem>
#include <fstream>
#include <QString>
#include <filesystem>

bool write_to_json(QString& address, QString& database_name, QString& password, int& port, QString& username);
void read_from_json(QString& address, QString& database_name, QString& password, int& port, QString& username);

#endif // JSON_MANAGE_H
