#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void keyReleaseEvent(QKeyEvent * event);
    void mostrarMatriz();
public slots:
    void printMatriz();
    void cleanMatriz();
    void clearPath();
    void setPI();
    void setPF();
    void marcarMatriz();
    void path();
private slots:
    void contextMenuRequest(QPoint pos);
    void on_actionGuardar_obstaculos_triggered();

    void on_actionCargar_obstaculos_triggered();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *myModel;
    int size=20;
    QPoint m_pointInicio;
    QPoint m_poinFinal;
    QList<QPoint> m_path;
    int ** m_matriz;

};

#endif // MAINWINDOW_H
