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
    std::vector<QString> code {
        "mov ax, bx",
        "mov cx, dx",
        "mov dh, [0xd]",
        "mov [di+bp], ax",
        "mov [0x77+di+bx], dx",
        "mov [bx+si], cx",
        "mov dh, dl",
        "mov cx, ax",
        "mov ax, ds"
    };

    Instruction *i;
    i = new Mov();
    for(auto line: code) {
        i->setCodeLine(line);
        qDebug() << line << i->process();
    }

    delete i;
    exit(1);
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

