#pragma once
#include <QtWidgets>

class CodeEditor;

class LineNumber : public QWidget
{
public:
    LineNumber(CodeEditor*);
    QSize sizeHint() const override;

private:
    CodeEditor *codeEditor;

protected:
    void paintEvent(QPaintEvent *event) override;
};
