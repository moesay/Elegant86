#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser/parser.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    codeEditor->setFont(QFont("Arial", 20));
    ui->setupUi(this);
    setCentralWidget(codeEditor);

    //error \/
    Instruction *i(new Mov("mov [bx+0xfb], ax"));
    qDebug() << i->getCodeLine() << i->process();
    i->setCodeLine("mov bx, [bx+0x0fff]");
    qDebug() << i->getCodeLine() << i->process();
    i->setCodeLine("mov bx, [bx+0x1234]");
    qDebug() << i->getCodeLine() << i->process();
    i->setCodeLine("mov bx, [bx+0x0006]");
    qDebug() << i->getCodeLine() << i->process();
    i->setCodeLine("mov bx, [bx+0xffff]");
    qDebug() << i->getCodeLine() << i->process();
    delete i;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open File");
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open the file.\n"+file.errorString());
        return;
    }
    QTextStream input(&file);
    codeEditor->clear();
    codeEditor->insertPlainText(input.readAll());
}

void MainWindow::on_action_Simulate_triggered() {

}

