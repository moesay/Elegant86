#include "mainwindow.h"
#include "assembler/precom.h"
#include <QDebug>
#include <QScriptEngine>
#include <QCoreApplication>

MainWindow::MainWindow()
{
    initUi();

    tabWidget->setCurrentIndex(1);

    QStringList code {
        /* "mov ax, dd11", */
        /* "idiv bh", "idiv bx", "add ax, ch","inc ax", */
        /* "aa:", */
        "jmp aa",
            /* "mov ax, [bx+di]", */
            /* "mov ax, [bx-di]", */
            /* "mov ax, [bx-10]", */
            /* "mov ax, [bx+10]", */
            /* "mov cx, [bx+66]", */
            /* ";comment", */
            /* "mov cx, dx", */
            /* "mov dh, [d]", */
            /* "mov [di+bp], ax", */
            /* "mov [77+di+bx], dx", */
            /* "mov ax, 1d", */
            /* "mov ah, -1d", */
            /* "mov [bx+si], cx", */
            /* "mov dh, dl", */
            /* "mov ax, [bx]", */
            /* "lbl:", */
            /* "mov cx, ax", */
            /* "lbl2:;comment", */
            /* "stc", "add ax, bx", */
            /* "mov ax, ds", */
            /* "mov si, ax", */
            /* "mov ds, ax", */
            /* "mov ax, 10" */
    };

    FirstPass p;
    QList<error> err = p.validate(code);
    for(auto &err_ : err) {
        auto [errmsg, inst, line] = err_;
        qDebug() << errmsg << inst << line;
    }

    /* Base *i; */
    /* i = new Mov(); */
    /* for(auto line: code) { */
    /*     i->setCodeLine(line); */
    /*     qDebug() << line << i->process(); */
    /* } */

    /* delete i; */
    exit(1);
}

void MainWindow::initUi() {
    setWindowTitle(tr("Elegant86[*]"));
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    tabWidget = new QTabWidget;
    editorWidget = new Editor(this);
    simulateWidget = new Simulate(this);
    tabWidget->addTab(editorWidget, tr("Code Editor"));
    tabWidget->addTab(simulateWidget, tr("Simulator"));
    setCentralWidget(tabWidget);
    /* setFileStatus(FileStatus::NewFile); */

    connect(editorWidget->codeEditor, SIGNAL(cursorPositionChanged()), this, SLOT(updateStatusBar()));

    connect(editorWidget->codeEditor->document(), &QTextDocument::modificationChanged,
            this, &QWidget::setWindowModified);
    connect(editorWidget->codeEditor->document(), &QTextDocument::modificationChanged,
            saveAction, &QAction::setEnabled);
    connect(editorWidget->codeEditor->document(), &QTextDocument::undoAvailable,
            undoAction, &QAction::setEnabled);
    connect(editorWidget->codeEditor->document(), &QTextDocument::redoAvailable,
            redoAction, &QAction::setEnabled);
    connect(editorWidget->codeEditor->document(), &QTextDocument::modificationChanged,
            this, [this] { setFileStatus(FileStatus::Modified); });


    saveAction->setEnabled(editorWidget->codeEditor->document()->isModified());
    undoAction->setEnabled(editorWidget->codeEditor->document()->isUndoAvailable());
    redoAction->setEnabled(editorWidget->codeEditor->document()->isRedoAvailable());
    setWindowModified(editorWidget->codeEditor->document()->isModified());
    cursorLabel->setText(tr("Line %1 / %2     Column %3").arg(
                QString::number(editorWidget->codeEditor->getLineNumber()),
                QString::number(editorWidget->codeEditor->document()->blockCount()),
                QString::number(editorWidget->codeEditor->textCursor().positionInBlock())));
}

MainWindow::~MainWindow()
{
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
    fileStatusLabel = new QLabel();
    setFileStatus(FileStatus::NewFile);

    cursorLabel = new QLabel();
    cursorLabel->setAlignment(Qt::AlignRight);
    cursorLabel->setMinimumSize(cursorLabel->sizeHint());

    statusBar()->addWidget(fileStatusLabel);
    statusBar()->addWidget(cursorLabel, 1);
}

void MainWindow::updateStatusBar() {
    setFileStatus(fileStatus);
    cursorLabel->setText(tr("Line %1 / %2     Column %3").arg(
                QString::number(editorWidget->codeEditor->getLineNumber()),
                QString::number(editorWidget->codeEditor->document()->blockCount()),
                QString::number(editorWidget->codeEditor->textCursor().positionInBlock())));
}

void MainWindow::newFile() {
    if(okToContinue()) {
        editorWidget->codeEditor->clear();
        tabWidget->setCurrentIndex(0);
        setWindowModified(false);
        setCurrentFileName("");
        setFileStatus(FileStatus::NewFile);
    }
}
bool MainWindow::save() {
    if(currentFileName.isEmpty())
        return saveAs();
    else
        return saveFile(currentFileName);
}
bool MainWindow::saveAs() {
    QFileDialog dialog(this, tr("Save As ..."));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Assembly Files (*.asm)\n"
                "Text Files (*.txt)"));
    dialog.setDefaultSuffix("asm");
    if(dialog.exec() != QDialog::Accepted) return false;
    QString fileName = dialog.selectedFiles().first();
    if (fileName.isEmpty()) return false;
    return saveFile(fileName);
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
    setFileStatus(FileStatus::NewFile);
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
            saveAction->setEnabled(false);
            setCurrentFileName(fileName);
            setFileStatus(FileStatus::Saved);
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

std::tuple <QString, QString> MainWindow::getFileStatus(const FileStatus& status){
    switch(status) {
        case NewFile:
            return {"New File", "black"}; break;
        case Modified:
            return {"Modified", "red"}; break;
        case Saved:
            return {"Saved", "green"}; break;
        default : return {"Unknown", "red"};
    }
}

void MainWindow::setFileStatus(const FileStatus& status) {
    this->fileStatus = status;
    auto [str, color] = getFileStatus(status);
    fileStatusLabel->setText("File Status : "+str);
    fileStatusLabel->setStyleSheet("QLabel { color : "+color+"; }");
}
