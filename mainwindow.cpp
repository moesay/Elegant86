#include "mainwindow.h"
#include "parser/parser.h"
#include <QDebug>
#include <QDesktopWidget>

MainWindow::MainWindow()
{
    createActions();
    createMenus();
    createToolBars();
    this->showMaximized();

    tabWidget = new QTabWidget;
    editorWidget = new Editor(this);
    simulateWidget = new Simulate(this);
    tabWidget->addTab(editorWidget, tr("Code Editor"));
    tabWidget->addTab(simulateWidget, tr("Simulator"));

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

void MainWindow::createActions() {
    newAction = new QAction(tr("&New File"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setIcon(QIcon(":/images/new.png"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open File"), this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setIcon(QIcon(":/images/open.png"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setIcon(QIcon(":/images/save.png"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save As"), this);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    exitAction = new QAction(tr("E&xit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(Exit()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setIcon(QIcon(":/images/copy.png"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));

    cutAction = new QAction(tr("Cut"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setIcon(QIcon(":/images/cut.png"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+U"));
    undoAction->setIcon(QIcon(":/images/undo.png"));
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));

    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcut(tr("Ctrl+R"));
    redoAction->setIcon(QIcon(":/images/redo.png"));
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));

    prefAction = new QAction(tr("Preferences"), this);
    connect(prefAction, SIGNAL(triggered()), this, SLOT(pref()));

    findAction = new QAction(tr("&Find"), this);
    findAction->setShortcut(tr("Ctrl+F"));
    connect(findAction, SIGNAL(triggered()), this, SLOT(find()));

    findAndReplaceAction = new QAction(tr("Find And Replace"), this);
    connect(findAndReplaceAction, SIGNAL(triggered()), this, SLOT(findAndReplace()));

    runAction = new QAction(tr("Run Simulation"));
    runAction->setShortcut(tr("F5"));
    runAction->setIcon(QIcon(":/images/run.png"));
    connect(runAction, SIGNAL(triggered()), this, SLOT(run()));

    pauseAction = new QAction(tr("Pause Simulation"), this);
    pauseAction->setIcon(QIcon(":/images/pause.png"));
    connect(pauseAction, SIGNAL(triggered()), this, SLOT(pause()));

    stepIntoAction = new QAction(tr("Step In"), this);
    stepIntoAction->setShortcut(tr("F6"));
    stepIntoAction->setIcon(QIcon(":/images/stepInto.png"));
    connect(stepIntoAction, SIGNAL(triggered()), this, SLOT(stepInto()));

    stepOutAction = new QAction(tr("Step Out"), this);
    stepOutAction->setShortcut(tr("F7"));
    stepOutAction->setIcon(QIcon(":/images/stepOut.png"));
    connect(stepOutAction, SIGNAL(triggered()), this, SLOT(stepOut()));

    continueAction = new QAction(tr("Continue Simulation"), this);
    continueAction->setIcon(QIcon(":/images/continue.png"));
    connect(continueAction, SIGNAL(triggered()), this, SLOT(Continue()));

    killAction = new QAction(tr("Kill Simulation"), this);
    killAction->setIcon(QIcon(":/images/kill.png"));
    connect(killAction, SIGNAL(triggered()), this, SLOT(kill()));

    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    separatorAction = fileMenu->addSeparator();
    for(auto act : recentFileActions)
        fileMenu->addAction(act);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(exitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAction);
    editMenu->addAction(cutAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addAction(prefAction);
    editMenu->addAction(findAction);
    editMenu->addAction(findAndReplaceAction);

    SimulateMenu = menuBar()->addMenu(tr("&Simulate"));
    SimulateMenu->addAction(runAction);
    SimulateMenu->addAction(pauseAction);
    SimulateMenu->addAction(continueAction);
    SimulateMenu->addAction(stepIntoAction);
    SimulateMenu->addAction(stepOutAction);
    SimulateMenu->addAction(killAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolBars() {
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    simulateToolBar = addToolBar(tr("&Simulate"));
    simulateToolBar->addAction(runAction);
    simulateToolBar->addAction(killAction);
    simulateToolBar->addAction(pauseAction);
    simulateToolBar->addAction(continueAction);
    simulateToolBar->addAction(stepIntoAction);
    simulateToolBar->addAction(stepOutAction);
}

void MainWindow::openFile() {
    QString filename = QFileDialog::getOpenFileName(this, "Open File");
    QFile file(filename);
    if (filename=="") return;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open the file.\n"+file.errorString());
        return;
    }
    QTextStream input(&file);
    editorWidget->codeEditor->clear();
    editorWidget->codeEditor->insertPlainText(input.readAll());
}

void MainWindow::newFile() {}
void MainWindow::save() {}
void MainWindow::saveAs() {}
void MainWindow::openRecentFile() {}
void MainWindow::Exit() {}

void MainWindow::copy() {}
void MainWindow::cut() {}
void MainWindow::paste() {}
void MainWindow::undo() {}
void MainWindow::redo() {}
void MainWindow::pref() {}
void MainWindow::find() {}
void MainWindow::findAndReplace() {}

void MainWindow::stepInto() {}
void MainWindow::stepOut() {}
void MainWindow::run() {tabWidget->setCurrentIndex(1);}
void MainWindow::kill() {}
void MainWindow::pause() {}
void MainWindow::Continue() {}
void MainWindow::updateStatusBar() {}

void MainWindow::about() {}

