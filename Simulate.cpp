#include "Simulate.h"

Simulate::Simulate(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void Simulate::updateFlagRegs(const int& reg, const bool& set, bool init) {
    if (init) {
        flagRegs[S]->setText("S : 0"); flagRegs[Z]->setText("Z : 0");
        flagRegs[AC]->setText("AC : 0"); flagRegs[P]->setText("P : 0");
        flagRegs[CY]->setText("CY : 0"); flagRegs[O]->setText("O : 0");
        flagRegs[D]->setText("D: 0"); flagRegs[I]->setText("I : 0");
        flagRegs[T]->setText("T : 0");
        return;
    }
    else {
        QString buf = flagRegs[reg]->text();
        buf.remove('0'); buf.remove('1'); buf.append(QString::number(set));
        flagRegs[reg]->setText(buf);
    }
}

void Simulate::setupUi() {
    topFlagRegsLayout = new QHBoxLayout;
    regsLayout = new QVBoxLayout;
    bottomFlagRegsLayout = new QHBoxLayout;
    flagRegsLayout = new QVBoxLayout;
    console = new Console;
    registersGB = new QGroupBox(tr("Registers"));
    consoleLayout = new QVBoxLayout;
    codeViewGB = new QGroupBox(tr("Code View"));
    topMainLayout = new QHBoxLayout;
    regsView = new QTreeWidget;
    codeView = new QListWidget;
    hexView = new QListWidget;
    codeViewLayout = new QHBoxLayout;
    mainLayout = new QVBoxLayout;
    bottomMainLayout = new QHBoxLayout;
    log = new QListWidget;
    utilsTabWidget = new QTabWidget;
    codeViewSP = new QSplitter;
    regCodeSP = new QSplitter;
    topBottomSP = new QSplitter;
    utilsSP = new QSplitter;
    Memory *mem = new Memory();
    Ports *prt = new Ports();
    Stack *stk = new Stack();
    Variables *vrb = new Variables();
    BaseConverter *bc = new BaseConverter();
    utilsTabWidget->addTab(mem, tr("Memory"));
    utilsTabWidget->addTab(vrb, tr("Variales"));
    utilsTabWidget->addTab(prt, tr("Ports"));
    utilsTabWidget->addTab(stk, tr("Stack"));
    utilsTabWidget->addTab(bc, tr("Base Converter"));

    codeViewSP->addWidget(codeView); codeViewSP->addWidget(hexView);
    initRegsView();
    console->setFixedSize(consoleSize);
    topWidget = new QWidget;
    bottomWidget = new QWidget;
    for(auto &lbl : flagRegs) lbl = new QLabel;
    for(int i = 0; i < 5; i++) topFlagRegsLayout->addWidget(flagRegs[i]);
    for(int i = 5; i < 9; i++) bottomFlagRegsLayout->addWidget(flagRegs[i]);
    updateFlagRegs(0, 0, true);
    bottomFlagRegsLayout->addWidget(em);
    flagRegsLayout->addLayout(topFlagRegsLayout); flagRegsLayout->addLayout(bottomFlagRegsLayout);
    codeViewLayout->addWidget(codeViewSP);
    codeViewGB->setLayout(codeViewLayout);
    regsLayout->addWidget(regsView);
    regsLayout->addLayout(flagRegsLayout);
    registersGB->setLayout(regsLayout);
    regCodeSP->addWidget(codeViewGB); regCodeSP->addWidget(registersGB);
    consoleLayout->addWidget(console); //consoleLayout->addStretch();
    topMainLayout->addLayout(consoleLayout); topMainLayout->addWidget(regCodeSP);
    topWidget->setLayout(topMainLayout);
    utilsSP->addWidget(log); utilsSP->addWidget(utilsTabWidget);
    bottomMainLayout->addWidget(utilsSP);
    bottomWidget->setLayout(bottomMainLayout);
    topBottomSP->setOrientation(Qt::Vertical);
    topBottomSP->addWidget(topWidget); topBottomSP->addWidget(bottomWidget);
    mainLayout->addWidget(topBottomSP);
    this->setLayout(mainLayout);
}
