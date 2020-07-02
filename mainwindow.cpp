#include "mainwindow.h"
#include "parser/parser.h"
#include <QDebug>
#include <QCoreApplication>

MainWindow::MainWindow()
{
    setWindowTitle(tr("Elegant86[*]"));
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
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

    connect(editorWidget->codeEditor, SIGNAL(cursorPositionChanged()), this, SLOT(updateStatusBar()));

    connect(editorWidget->codeEditor->document(), &QTextDocument::modificationChanged,
            this, &QWidget::setWindowModified);
    connect(editorWidget->codeEditor->document(), &QTextDocument::modificationChanged,
            saveAction, &QAction::setEnabled);
    connect(editorWidget->codeEditor->document(), &QTextDocument::undoAvailable,
            undoAction, &QAction::setEnabled);
    connect(editorWidget->codeEditor->document(), &QTextDocument::redoAvailable,
            redoAction, &QAction::setEnabled);

    saveAction->setEnabled(editorWidget->codeEditor->document()->isModified());
    undoAction->setEnabled(editorWidget->codeEditor->document()->isUndoAvailable());
    redoAction->setEnabled(editorWidget->codeEditor->document()->isRedoAvailable());
    setWindowModified(editorWidget->codeEditor->document()->isModified());

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
    if(!okToContinue()) return;

    QString filename = QFileDialog::getOpenFileName(this,
            tr("Open File"), ".",
            tr("ASM Files (*.asm)\n"
                "Text Files (*.txt)"));
    if (!filename.isEmpty()) loadFile(filename);
}

void MainWindow::createStatusBar() {
    fileStatusLabel = new QLabel("File Status : " + getFileStatus());

    cursorLabel = new QLabel();
    cursorLabel->setAlignment(Qt::AlignRight);
    cursorLabel->setMinimumSize(cursorLabel->sizeHint());

    statusBar()->addWidget(fileStatusLabel);
    statusBar()->addWidget(cursorLabel, 1);
}

void MainWindow::updateStatusBar() {
    fileStatusLabel->setText("File Status : " + getFileStatus());
    cursorLabel->setText(QString::number(editorWidget->codeEditor->textCursor().positionInBlock())
            + " / " + QString::number(editorWidget->codeEditor->getLineNumber()));
}

void MainWindow::newFile() {
    if(okToContinue()) {
        editorWidget->codeEditor->clear();
        tabWidget->setCurrentIndex(0);
        setCurrentFileName("");
    }
}
bool MainWindow::save() {
    if(currentFileName.isEmpty())
        return saveAs();
    else
        return saveFile(currentFileName);
}
bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save ASM File"), ".",
            tr("ASM File (*.asm)"));
    if (fileName.isEmpty()) return false;
    return saveFile(fileName+".asm");
}
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

void MainWindow::about() {}
QString MainWindow::getFileStatus() {return "";}

bool MainWindow::okToContinue() {
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Elegant86"),
                tr("The source file has been modified.\n"
                    "Do you want to save your changes?"),
                QMessageBox::Yes | QMessageBox::Default,
                QMessageBox::No,
                QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::loadFile(const QString &fileName) {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "Error", "Cannot open the file.\n"+file.errorString());
        return false;
    }
    QTextStream input(&file);
    editorWidget->codeEditor->clear();
    editorWidget->codeEditor->insertPlainText(input.readAll());
    setCurrentFileName(fileName);
    return true;
}

bool MainWindow::saveFile (const QString& fileName) {
    QSaveFile fileToSave(fileName);
    if(fileToSave.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&fileToSave);
        out << editorWidget->codeEditor->toPlainText();
        if(!fileToSave.commit()) {
            statusBar()->showMessage("Failed to save your file", 2000);
            return false;
        }
        else {
            editorWidget->codeEditor->document()->setModified(false);
            setCurrentFileName(fileName);
            fileStatus = FileStatus::Saved;
            return true;
        }
    }
    statusBar()->showMessage("Failed to save your file", 2000);
    return false;
}

void MainWindow::setCurrentFileName(const QString& fileName) {
    currentFileName = fileName;
    editorWidget->codeEditor->document()->setModified(false);

    if(fileName.isEmpty())
        shownFileName = "untitled.asm";
    else
        shownFileName = QFileInfo(fileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownFileName, QCoreApplication::applicationName()));
}
