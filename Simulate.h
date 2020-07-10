#ifndef SIMULATE_CLASS_H
#define SIMULATE_CLASS_H

#include <QtWidgets>
#include <QAbstractScrollArea>
#include "console.h"

class BaseConverter : public QWidget {};
class Stack : public QWidget {};
class Memory : public QWidget {};
class Variables : public QWidget{};
class Ports : public QWidget {};


class Simulate : public QWidget {
    Q_OBJECT
    public:
        explicit Simulate(QWidget *parent = nullptr);
        void setupUi();

    private:
        struct code {
            QString code;
            QString hexCode;
            int lineNumer;
        };
        enum {AX, BX, CX, DX, AH, AL, BH, BL, CH, CL, DH, DL,
            SP, BP, SI, DI, IP, ES, CS, SS, DS};
        enum {S, Z, AC, P, CY, O, D, I, T};
        const QSize consoleSize = QSize(
                fontMetrics().horizontalAdvance(QLatin1Char('0')) * 90,
                fontMetrics().horizontalAdvance(QLatin1Char('0')) * 59);

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
        QVBoxLayout *consoleLayout;
        QHBoxLayout *topMainLayout;
        QHBoxLayout *bottomMainLayout;
        QVBoxLayout *mainLayout;
        QSplitter *codeViewSP;
        QSplitter *regCodeSP;
        QSplitter *topBottomSP;
        QSplitter *utilsSP;
        QListWidget *log;
        QWidget *topWidget, *bottomWidget;

        void addRoot(const QString&, const QString& = "", const QString& = "");
        void updateRegView(const int&, const QString&);
        QLabel *flagRegs[9];
        QLabel *em = new QLabel;
        void updateFlagRegs(const int&, const bool&, bool = false);


        void initRegsView();
};

#endif
