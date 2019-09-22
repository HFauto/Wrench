#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QMainWindow>
#include <QTranslator>
#include <QEvent>

namespace Ui {
class Language;
}

class Language : public QMainWindow
{
    Q_OBJECT

public:
    explicit Language(QWidget *parent = nullptr);
    ~Language();


private slots:


    void on_comboBox_activated(int index);


private:
    Ui::Language *ui;


};

#endif // LANGUAGE_H
