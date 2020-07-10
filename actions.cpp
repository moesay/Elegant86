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
