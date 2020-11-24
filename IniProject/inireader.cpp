#include "inireader.h"

IniReader::IniReader()
{

}

QString IniReader::GetPath() {
    return path;
}

void IniReader::SetPath(QString in_path) {
    path = in_path;
    sett.reset(new QSettings(path, QSettings::IniFormat));
    ReadValueList();
    sett.release();
}

QMap <QString, QMap<QString, QString> > IniReader::GetValueList() {
    return List;
}

void IniReader::ReadValueList()
{
    QString text;
    QMap <QString, QString> keysList;
    QStringList group_list = sett->childGroups();
    for (int j = 0; j < group_list.size(); ++j)
    {
        sett->beginGroup(group_list[j]);
        QStringList keys = sett->childKeys();
        for (int i = 0; i < keys.size(); ++i)
        {
            text = sett->value(keys[i], "").toString();
            keysList.insert(keys[i], text);
        }
        sett->endGroup();
        List[group_list[j]] = keysList;
        keysList.clear();
    }
    qDebug() << List;
}

QString IniReader::ReadValue(QString group, QString key)
{
    QMap <QString, QString> keysList;
    QString value;
    if (List.contains(group))
    {
        keysList = List[group];
        if (keysList.contains(key))
             value = keysList[key];
        else qDebug() << "Указанного ключа в файле ini нет.";
    } else qDebug() << "Указанной группы в файле ini нет.";
    return value;
}

void IniReader::ChangeValue(QString group, QString key, QString value)
{
    QMap <QString, QString> keysList;
    if (List.contains(group))
    {
        keysList = List[group];
        if (keysList.contains(key)) {
            keysList[key] = value;
            List[group] = keysList;
        } else qDebug() << "Указанного ключа в файле ini нет.";
    } else qDebug() << "Указанной группы в файле ini нет.";
    qDebug() << List;
}

void IniReader::AddValue(QString group, QString key, QString value)
{
    QMap <QString, QString> keysList;
    if (List.contains(group))
    {
        keysList = List[group];
        if (keysList.contains(key))
            qDebug() << "Указанный ключ в файле ini уже есть.";
        else {
            keysList.insert(key, value);
            List[group] = keysList;
        }
    }
    else {
        keysList.insert(key, value);
        List.insert(group, keysList);
    }
    qDebug() << List;
}

void IniReader::ClearValue(QString group, QString key)
{
    QMap <QString, QString> keysList;
    if (List.contains(group))
    {
        keysList = List[group];
        if (keysList.contains(key)) {
            keysList.remove(key);
            List[group] = keysList;
        } else qDebug() << "Указанного ключа в файле ini нет.";
    } else qDebug() << "Указанной группы в файле ini нет.";
    qDebug() << List;
}

void IniReader::SaveFile(QString in_path)
{
    sett.reset(new QSettings(in_path, QSettings::IniFormat));
    QMap <QString, QString> keysList;
    QStringList group = List.uniqueKeys();
    for (int j = 0; j < group.size(); j++)
    {
        sett->beginGroup(group[j]);
        keysList = List[group[j]];
        QStringList keys = keysList.uniqueKeys();
        for (int i = 0; i < keys.size(); ++i)
            sett->setValue(keys[i], keysList[keys[i]]);
        sett->endGroup();
    }
    qDebug() << List;
    sett.release();
}
