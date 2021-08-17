#ifndef INIREADER_H
#define INIREADER_H

#include <QSettings>
#include <QDebug>
#include <memory>

class IniReader : public QSettings
{
public:
    IniReader();
    QString GetPath();
    QMap <QString, QMap<QString, QString> > GetValueList();
    void ReadValueList();
    QString ReadValue(QString group, QString key);

    void SetPath(QString in_path);
    void ChangeValue(QString group, QString key, QString value);
    void AddValue   (QString group, QString key, QString value);
    void ClearValue (QString group, QString key);
    void SaveFile(QString in_path);

private:
    QString path;
    std::unique_ptr<QSettings> sett;
    QMap <QString, QMap<QString, QString> > List;
};

#endif // INIREADER_H
