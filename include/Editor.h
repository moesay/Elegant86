#ifndef EDITOR_CLASS_H
#define EDITOR_CLASS_H

#include <QtWidgets>
#include <codeEditor/include/codeeditor.h>
#include <codeEditor/include/highliter.h>

class Editor : public QWidget {
    Q_OBJECT
    public:
        CodeEditor *codeEditor;
        Highliter *hi;
        explicit Editor(QWidget *parent = nullptr);
};

#endif
