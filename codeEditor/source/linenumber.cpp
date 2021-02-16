#include <codeEditor/include/linenumber.h>
#include <codeEditor/include/codeeditor.h>

LineNumber::LineNumber(CodeEditor *editor) : QWidget (editor), codeEditor(editor)
{}

QSize LineNumber::sizeHint() const {
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
}

void LineNumber::paintEvent(QPaintEvent *event){
    codeEditor->lineNumberAreaPaintEvent(event);
}

int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 5 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}

