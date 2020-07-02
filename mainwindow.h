#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Editor.h"
#include "Simulate.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private slots:
    void newFile();
    void openFile();
    bool save();
    bool saveAs();
    void openRecentFile();
    void Exit();

    void copy();
    void cut();
    void paste();
    void undo();
    void redo();
    void pref();
    void find();
    void findAndReplace();

    void stepInto();
    void stepOut();
    void run();
    void kill();
    void pause();
    void Continue();
    void updateStatusBar();
    void about();

private:
    QTabWidget *tabWidget;
    Editor *editorWidget;
    Simulate *simulateWidget;
    int fileStatus = -1;
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);
    void updateCurrentFileActions();
    /* QString strippedName (const QString& fullFileName); */
    QString getFileStatus();
    bool okToContinue();
    void setCurrentFileName(const QString&);

    QStringList recentFiles;
    QString shownFileName;
    QString currentFileName;

    enum {MaxRecentFiles = 5};
    enum FileStatus {Modified, Saved, NewFile};
    enum Mode {Editing, Simulation, Debuging};

    //file
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *separatorAction;
    QAction *recentFileActions[MaxRecentFiles];
    QAction *saveAsAction;
    QAction *exitAction;

    //edit
    QAction *copyAction;
    QAction *cutAction;
    QAction *pasteAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *prefAction;
    QAction *findAction;
    QAction *findAndReplaceAction;

    //simulate
    QAction *runAction;
    QAction *pauseAction;
    QAction *continueAction;
    QAction *stepIntoAction;
    QAction *stepOutAction;
    QAction *killAction;

    //help
    QAction *aboutAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *SimulateMenu;
    QMenu *helpMenu;
    QMenu *contextMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *simulateToolBar;

    QLabel *fileStatusLabel;
    QLabel *cursorLabel;
    QLabel *totalLinesLabel;
    QLabel *ModeLabel;

};

#endif // MAINWINDOW_H

