#ifndef SIMULATE_CLASS_H
#define SIMULATE_CLASS_H

#include <QtWidgets>
#include <QAbstractScrollArea>
#include "console.h"
#include "stack.h"
#include "utils.h"
#include "ports.h"
#include "variables.h"

class Simulate : public QWidget {
    Q_OBJECT
    public:
        explicit Simulate(QWidget *parent = nullptr);
        void insertLog(const QString&);
        void clearLog();
        void addToCodeViews(const QString&, const QString&);
        void resetUi();

    private:
        ~Simulate();
        struct code {
            QString code;
            QString hexCode;
            int lineNumer;
        };
        enum {AX, BX, CX, DX, AH, AL, BH, BL, CH, CL, DH, DL,
            SP, BP, SI, DI, IP, ES, CS, SS, DS};
        enum {S, Z, AC, P, CY, O, D, I, T};

        Console *console;
        QTabWidget *utilsTabWidget;
        QTreeWidget *regsView;
        QListWidget *hexView, *codeView;
        QGroupBox *registersGB;
        QGroupBox *codeViewGB;
        QHBoxLayout *codeViewLayout;
        QHBoxLayout *topFlagRegsLayout;
        QHBoxLayout *bottomFlagRegsLayout;
        QVBoxLayout *flagRegsLayout;
        QVBoxLayout *regsLayout;
        QHBoxLayout *topMainLayout;
        QHBoxLayout *bottomMainLayout;
        QVBoxLayout *mainLayout;
        QSplitter *codeViewSP;
        QSplitter *consoleSP;
        QSplitter *regCodeSP;
        QSplitter *topBottomSP;
        QSplitter *utilsSP;
        QListWidget *log;
        QWidget *topWidget, *bottomWidget;

        void setupUi();
        Ports *prt;
        Stack *stk;
        Variables *vrb;
        Utils *utl;
        void addRoot(const QString&, const QString& = "", const QString& = "");
        void updateRegView(const int&, const QString&);
        QLabel *flagRegs[9];
        QLabel *em = new QLabel;
        void updateFlagRegs(const int&, const bool&, bool = false);


        void initRegsView();
        private slots:
        void setCodeView(int);
        void setHexView(int);
};

#endif
