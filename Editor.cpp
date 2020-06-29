#include "Editor.h"
#include "mainwindow.h"
#include "codeEditor/codeeditor.h"
#include "codeEditor/highliter.h"

Editor::Editor(QWidget *parent) : QWidget(parent) {
    codeEditor = new CodeEditor(this);
    hi = new Highliter(codeEditor->document());

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(codeEditor);
    this->setLayout(layout);
}
