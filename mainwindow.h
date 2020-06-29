#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow();
    ~MainWindow();

private slots:
    /* void newFile(); */
    void open();
    /* void save(); */
    /* void saveAs(); */
    /* void findAndReplace(); */
    /* void openRecentFile(); */
    /* void updateStatusBar(); */
    /* void about(); */

private:
    QTabWidget *tabWidget;

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
    QString strippedName (const QString& fullFileName);

    QStringList recentFiles;
    QString curFile;

    enum {MaxRecentFiles = 5};
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *contextMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAction;
    QAction *openAction;
    QAction *aboutAction;
};

#endif // MAINWINDOW_H

