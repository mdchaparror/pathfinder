#include "mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

QString stringFromResource(const QString &resName)
{
    QFile file(resName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream ts(&file);
    return ts.readAll();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    MainWindow w;
    //w.setStyleSheet(stringFromResource("styles/styleDark.qss"));
    w.show();

    return a.exec();
}
