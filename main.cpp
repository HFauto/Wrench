#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QSettings>


QTranslator *m_translator=NULL;
QString readSetting();

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    m_translator=new QTranslator;
    QString curLang=readSetting(); //读取语言设置
    if (curLang=="EN")
        m_translator->load("wrenchApp_en.qm");
    else
        m_translator->load("wrenchApp_cn.qm");
    a.installTranslator(m_translator);
    MainWindow w;
    w.show();

    return a.exec();
}
/*
* @projectName   WrenchApp
* @brief         从注册表读取上次设置的语言
* @author        Li
* @date          2019-08-30
*/
QString readSetting()
{//从注册表读取上次设置的语言
    QString organization="WWB-Qt";//用于注册表，
    QString appName="WrenchApp"; //HKEY_CURRENT_USER/WWB-Qt/amp13_1
    QSettings  settings(organization,appName);//创建
    QString Language=settings.value("Language","EN").toString();//读取 saved键的值
    return  Language;
}
