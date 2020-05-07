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

    Instruction *i(new Mov("mov ax, ds"));
    qDebug() << i->getCodeLine() << i->process();
    i->setCodeLine("mov ax, es");
    qDebug() << i->getCodeLine() << i->process();
    i->setCodeLine("mov ax, cs");
    qDebug() << i->getCodeLine() << i->process();
    i->setCodeLine("mov ax, ss");
    qDebug() << i->getCodeLine() << i->process();
    i->setCodeLine("mov ss, ax");
    qDebug() << i->getCodeLine() << i->process();
    i->setCodeLine("mov ds, bx");
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

