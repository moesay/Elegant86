#include <Editor.h>
#include "mainwindow.h"

Editor::Editor(QWidget *parent) : QWidget(parent) {
    codeEditor = new CodeEditor(this);
    hi = new Highliter(codeEditor->document());

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(codeEditor);
    this->setLayout(mainLayout);
}
