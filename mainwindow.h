#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "codeEditor/codeeditor.h"

#include "codeEditor/highliter.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    CodeEditor *codeEditor = new CodeEditor(this);

    ~MainWindow();

private slots:
    void on_action_Open_triggered();
    void on_action_Simulate_triggered();

private:
    Ui::MainWindow *ui;
    Highliter *h;
};

#endif // MAINWINDOW_H
