#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <iostream>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    size=30;
    myModel = new QStandardItemModel(this);
    myModel->setRowCount(size);
    myModel->setColumnCount(size);
    ui->tableView->setModel(myModel);
    m_matriz = new int* [size];
    for (int i = 0; i < size; i++) {
        m_matriz[i] = new int[size];
    }
    connect(ui->tableView,SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_matriz;

}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    int key= event->key();

    if(key==32){
        QItemSelectionModel *selections = ui->tableView->selectionModel();
        QModelIndexList selected = selections->selectedIndexes();

        foreach (QModelIndex index, selected) {
            int row=index.row();
            int col=index.column();
            if(!myModel->item(row,col)){
            QStandardItem *it=new QStandardItem();
            it->setText("");
            it->setBackground(QColor(Qt::transparent));
            myModel->setItem(row,col,it);
        }
        QString s= myModel->item(row,col)->text();

        if(s=="-1"){
            myModel->item(row,col)->setText("");
            myModel->item(row,col)->setBackground(QColor(Qt::transparent));
        }
        else{
            myModel->item(row,col)->setText("-1");
            myModel->item(row,col)->setBackground(QColor(Qt::black));
        }

        }

    }

}


void MainWindow::contextMenuRequest(QPoint pos)
{

    QMenu *menu = new QMenu(this);
    menu->setAttribute(Qt::WA_DeleteOnClose);
    menu->addAction("Mostrar matriz", this, SLOT(printMatriz()));
    menu->addAction("Limpiar matriz", this, SLOT(cleanMatriz()));
    menu->addAction("Punto inicial", this, SLOT(setPI()));
    menu->addAction("Punto Final", this, SLOT(setPF()));
    menu->addAction("Mostrar Camino", this, SLOT(path()));
    menu->popup(ui->tableView->mapToGlobal(pos));
}
void MainWindow::printMatriz(){
    int row=myModel->rowCount();
    int col =myModel->columnCount();
    marcarMatriz();
    mostrarMatriz();
    std::cout<<"Matriz2..\n";
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
           std::cout<<m_matriz[i][j]<<" ";
        }
        std::cout<<std::endl;
    }
}

void MainWindow::marcarMatriz(){

    int row=myModel->rowCount();
    int col =myModel->columnCount();

    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(!myModel->item(i,j)){
                myModel->setItem(i,j,new QStandardItem(" "));
            }
            QString k=myModel->item(i,j)->text();

            if(k==" " || k != "-1"){
                m_matriz[i][j]=-10;
            }
            else{
                m_matriz[i][j]=k.toInt();
            }

        }
    }


    m_matriz[m_pointInicio.rx()][m_pointInicio.ry()]=0;

    int contador=0;
    while(true){
        bool isFinal=true;
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                int actual = m_matriz[i][j];
                if(actual==-10){
                    isFinal=false;
                }
                if(actual==contador){
                    if(i<size-1){
                        int prox=m_matriz[i+1][j];
                        if(prox ==-10){
                            m_matriz[i+1][j]=contador+1;
                        }
                    }
                    if(i>0){
                        int prox=m_matriz[i-1][j];
                        if(prox ==-10){
                            m_matriz[i-1][j]=contador+1;
                        }
                    }
                    if(j<size-1){
                        int prox=m_matriz[i][j+1];
                        if(prox ==-10){
                            m_matriz[i][j+1]=contador+1;
                        }
                    }
                    if(j>0){
                        int prox=m_matriz[i][j-1];
                        if(prox ==-10){
                            m_matriz[i][j-1]=contador+1;
                        }
                    }

                }
            }
        }
        contador ++;
        if(isFinal)break;

    }

}

void MainWindow::mostrarMatriz(){

    int row=myModel->rowCount();
    int col =myModel->columnCount();
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(!myModel->item(i,j)){
                myModel->setItem(i,j,new QStandardItem(""));
            }
            myModel->item(i,j)->setText(QString::number(m_matriz[i][j]));
        }
    }
}

void MainWindow::path(){

    int dI =m_poinFinal.rx();
    int dJ=m_poinFinal.ry();
    clearPath();
    while(true){
        m_path.push_back(QPoint(dI,dJ));
        int valueActual = m_matriz[dI][dJ];
        if(valueActual== 0){
            break;
        }

        if(dI>0){
            if(m_matriz[dI-1][dJ]<valueActual && m_matriz[dI-1][dJ]>=0){
                dI=dI-1;
               continue;
           }
        }

        if(dI<size-1){
            if(m_matriz[dI+1][dJ]<valueActual && m_matriz[dI+1][dJ]>=0){
                dI=dI+1;
                continue;
            }
        }
        if(dJ>0){
            if(m_matriz[dI][dJ-1]<valueActual && m_matriz[dI][dJ-1]>=0){
                dJ=dJ-1;
                continue;
            }
        }

        if(dJ<size-1){
            if(m_matriz[dI][dJ+1]<valueActual && m_matriz[dI][dJ+1]>=0){
                dJ=dJ+1;
                continue;
            }
        }
    }
    QPoint anterior=m_poinFinal;
    foreach (QPoint p, m_path) {
        if(p != m_pointInicio){
            ui->listWidget->addItem(QString("Point(%1,%2)").arg(p.rx()).arg(p.ry()));
            myModel->item(p.rx(),p.ry())->setBackground(QColor(Qt::yellow));
        }

        QPoint r = p-anterior;

        if(r.ry()==1){
            myModel->item(anterior.rx(),anterior.ry())->setIcon(QIcon(":/imagenes/left.png"));
        }
        else if(r.ry()==-1){
            myModel->item(anterior.rx(),anterior.ry())->setIcon(QIcon(":/imagenes/right.png"));

        }else if(r.rx()==1){
            myModel->item(anterior.rx(),anterior.ry())->setIcon(QIcon(":/imagenes/up.png"));

        }else if (r.rx()==-1) {
            myModel->item(anterior.rx(),anterior.ry())->setIcon(QIcon(":/imagenes/down.png"));

        }




        anterior=p;

    }
    m_path.pop_back();
    myModel->item(m_poinFinal.rx(),m_poinFinal.ry())->setBackground(QColor(Qt::red));
}

void MainWindow::cleanMatriz(){
    myModel->clear();
    myModel->setColumnCount(size);
    myModel->setRowCount(size);
    m_path.clear();
}
void MainWindow::clearPath()
{
    foreach (QPoint p, m_path) {
        QString s= myModel->item(p.rx(),p.ry())->text();
        if(s=="-1"){
             myModel->item(p.rx(),p.ry())->setBackground(QColor(Qt::black));
             myModel->item(p.rx(),p.ry())->setIcon(QIcon(""));

        }else{

            myModel->item(p.rx(),p.ry())->setBackground(QColor(Qt::transparent));
            myModel->item(p.rx(),p.ry())->setIcon(QIcon(""));
        }

    }
    m_path.clear();
    ui->listWidget->clear();
}
void MainWindow::setPI(){
    QModelIndex index = ui->tableView->currentIndex();
    int row=index.row();
    int col=index.column();
    if(myModel->item(m_pointInicio.rx(),m_pointInicio.ry())){
       // myModel->item(m_pointInicio.rx(),m_pointInicio.ry())->setText("");
        //myModel->item(m_pointInicio.rx(),m_pointInicio.ry())->setIcon(QIcon(""));
        myModel->item(m_pointInicio.rx(),m_pointInicio.ry())->setBackground(QColor(Qt::transparent));
    }

    m_pointInicio.setX(row);
    m_pointInicio.setY(col);

    if(!myModel->item(row,col)){
        QStandardItem *it=new QStandardItem();
        //it->setText("0");
        it->setBackground(QColor(Qt::green));
        //it->setIcon(QIcon(":/load.png"));
        myModel->setItem(row,col,it);
    }
    else{
        //myModel->item(row,col)->setText("0");
       // myModel->item(row,col)->setIcon(QIcon(":/load.png"));
        myModel->item(row,col)->setBackground(QColor(Qt::green));
    }
    qDebug()<<" Punto Inicial: "<<m_pointInicio;
    clearPath();
    marcarMatriz();


}
void MainWindow::setPF(){
    QModelIndex index = ui->tableView->currentIndex();
    int row=index.row();
    int col=index.column();
    if(myModel->item(m_poinFinal.rx(),m_poinFinal.ry())){
        myModel->item(m_poinFinal.rx(),m_poinFinal.ry())->setBackground(QColor(Qt::transparent));
    }

    m_poinFinal.setX(row);
    m_poinFinal.setY(col);

    if(!myModel->item(row,col)){
        QStandardItem *it=new QStandardItem();
        it->setBackground(QColor(Qt::red));
        myModel->setItem(row,col,it);
    }
    else{

        myModel->item(row,col)->setBackground(QColor(Qt::red));
    }

    clearPath();

    qDebug()<<" Punto final: "<<m_poinFinal;
}
void MainWindow::on_actionGuardar_obstaculos_triggered()
{
    QString NameFile=QFileDialog::getSaveFileName(this,"Guardar Obstaculos","Obstaculos", "Obstaculos (*.txt)");
       if(!NameFile.isEmpty()){
           QFile file(NameFile);
               file.open(QIODevice::WriteOnly | QIODevice::Text);
               QTextStream out(&file);

               for (int i=0;i<size;i++) {
                   for (int j=0;j<size;j++) {
                   if(myModel->item(i,j)){
                       QString k=myModel->item(i,j)->text();
                       if(k == "-1"){
                            out<<((QString("pi:%1,pj:%2\n").arg(i).arg(j)));

                       }

                   }
               }
               }

               file.close();
       }
}
void MainWindow::on_actionCargar_obstaculos_triggered()
{
    cleanMatriz();
    QRegExp rx("(\\d+)");
    QString NameFile=QFileDialog::getOpenFileName(this,tr("Abrir archivo de Obstaculos"),"", tr("Obstaculos Files (*.txt)"));

        if(NameFile.isEmpty()){
           return;
        }

    QFile file(NameFile);
       if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
           return;

       QTextStream in(&file);

       while (!in.atEnd()) {
           int i,j;
           QString line = in.readLine();
           QStringList lineSplit=line.split(',');
           bool ok=false;

           if(lineSplit.length()==2){
               rx.indexIn(lineSplit.at(0));
               i=rx.cap(0).toInt(&ok);
               rx.indexIn(lineSplit.at(1));
               j=rx.cap(0).toInt(&ok);
               if(ok){
                   if(!myModel->item(i,j)){
                   QStandardItem *it=new QStandardItem();
                   it->setText("-1");
                   it->setBackground(QColor(Qt::black));
                    myModel->setItem(i,j,it);
                   }else{
                       myModel->item(i,j)->setText("-1");
                       myModel->item(i,j)->setBackground(QColor(Qt::black));
                   }
                  // qDebug()<<i<<","<<j;
               }

           }



       }
       file.close();
}
