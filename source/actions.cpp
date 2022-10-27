#include "mainwindow.h"


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

    SimulateMenu = menuBar()->addMenu(tr("E&mulate"));
    SimulateMenu->addAction(runAction);
    SimulateMenu->addAction(pauseAction);
    SimulateMenu->addAction(continueAction);
    SimulateMenu->addAction(stepIntoAction);
    SimulateMenu->addAction(stepOutAction);
    SimulateMenu->addAction(killAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createActions() {
    newAction = new QAction(tr("&New File"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setIcon(QIcon(":/resources/new.png"));
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

    openAction = new QAction(tr("&Open File"), this);
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setIcon(QIcon(":/resources/open.png"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setIcon(QIcon(":/resources/save.png"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);

    saveAsAction = new QAction(tr("Save As"), this);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], &QAction::triggered, this, &MainWindow::openRecentFile);
    }

    exitAction = new QAction(tr("E&xit"), this);
    connect(exitAction, &QAction::triggered, this, &MainWindow::Exit);

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setShortcut(tr("Ctrl+C"));
    copyAction->setIcon(QIcon(":/copy.png"));
    connect(copyAction, &QAction::triggered, this, &MainWindow::copy);

    cutAction = new QAction(tr("Cut"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setIcon(QIcon(":/resources/cut.png"));
    connect(cutAction, &QAction::triggered, this, &MainWindow::cut);

    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setIcon(QIcon(":/resources/paste.png"));
    connect(pasteAction, &QAction::triggered, this, &MainWindow::paste);

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+U"));
    undoAction->setIcon(QIcon(":/resources/undo.png"));
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcut(tr("Ctrl+R"));
    redoAction->setIcon(QIcon(":/resources/redo.png"));
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);

    prefAction = new QAction(tr("Preferences"), this);
    connect(prefAction, &QAction::triggered, this, &MainWindow::pref);

    findAction = new QAction(tr("&Find"), this);
    findAction->setShortcut(tr("Ctrl+F"));
    connect(findAction, &QAction::triggered, this, &MainWindow::find);

    findAndReplaceAction = new QAction(tr("Find And Replace"), this);
    connect(findAndReplaceAction, &QAction::triggered, this, &MainWindow::findAndReplace);

    runAction = new QAction(tr("Run Emulation"));
    runAction->setShortcut(tr("F5"));
    runAction->setIcon(QIcon(":/resources/run.png"));
    connect(runAction, &QAction::triggered, this, &MainWindow::run);

    pauseAction = new QAction(tr("Pause Emulation"), this);
    pauseAction->setIcon(QIcon(":/resources/pause.png"));
    connect(pauseAction, &QAction::triggered, this, &MainWindow::pause);

    stepIntoAction = new QAction(tr("Step In"), this);
    stepIntoAction->setShortcut(tr("F6"));
    stepIntoAction->setIcon(QIcon(":/resources/stepInto.png"));
    connect(stepIntoAction, &QAction::triggered, this, &MainWindow::stepInto);

    stepOutAction = new QAction(tr("Step Out"), this);
    stepOutAction->setShortcut(tr("F7"));
    stepOutAction->setIcon(QIcon(":/resources/stepOut.png"));
    connect(stepOutAction, &QAction::triggered, this, &MainWindow::stepOut);

    continueAction = new QAction(tr("Continue Emulation"), this);
    continueAction->setIcon(QIcon(":/resources/continue.png"));
    connect(continueAction, &QAction::triggered, this, &MainWindow::Continue);

    killAction = new QAction(tr("Kill Emulation"), this);
    killAction->setIcon(QIcon(":/resources/kill.png"));
    connect(killAction, &QAction::triggered, this, &MainWindow::kill);

    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);

}
