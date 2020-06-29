#include "mainwindow.h"
#include "Editor.h"
#include "Simulate.h"
#include "parser/parser.h"
#include <QDebug>

MainWindow::MainWindow()
{
    tabWidget = new QTabWidget;

    tabWidget->addTab(new Editor(this), tr("Code Editor"));
    tabWidget->addTab(new Simulate(this), tr("Simulator"));

    setCentralWidget(tabWidget);

    /* Instruction *t = new Mov("mov ds, ax"); //the immediate values processing */
    /* qDebug() << t->process(); */
    /* exit(1); */

    std::vector<QString> code {
        "mov ax, bx",
            "mov cx, [bx+66]",
            "mov cx, dx",
            "mov dh, [d]",
            "mov [di+bp], ax",
            "mov [77+di+bx], dx",
            "mov [bx+si], cx",
            "mov dh, dl",
            "mov ax, [bx]",
            "mov cx, ax",
            "mov ax, ds",
            "mov si, ax",
            "mov ds, ax",
            "mov ax, 10"
    };

    Instruction *i;
    i = new Mov();
    for(auto line: code) {
        i->setCodeLine(line);
        qDebug() << line << i->process();
    }

    delete i;
}

MainWindow::~MainWindow()
{

}

void MainWindow::open() {
    /* QString filename = QFileDialog::getOpenFileName(this, "Open File"); */
    /* QFile file(filename); */
    /* if(!file.open(QIODevice::ReadOnly | QFile::Text)) { */
    /*     QMessageBox::warning(this, "Error", "Cannot open the file.\n"+file.errorString()); */
    /*     return; */
    /* } */
    /* QTextStream input(&file); */
    /* codeEditor->clear(); */
    /* codeEditor->insertPlainText(input.readAll()); */
}

