#include <include/Simulate.h>

Simulate::Simulate(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void Simulate::addToCodeViews(const QString& asmCode, const QString& hexCode) {
    hexView->addItem(hexCode); codeView->addItem(asmCode);
}

void Simulate::clearLog() {
    log->clear();
}

void Simulate::insertLog(const QString& param) {
    QString timeStr = QTime::currentTime().toString("hh:mm:ss");
    log->addItem("["+timeStr+"] "+param);
}

void Simulate::updateFlagRegs(const int& reg, const bool& set, bool init) {
    if (init) {
        flagRegs[S]->setText("S : 0"); flagRegs[Z]->setText("Z : 0");
        flagRegs[AC]->setText("AC : 0"); flagRegs[P]->setText("P : 0");
        flagRegs[CY]->setText("I : 0"); flagRegs[O]->setText("O : 0");
        flagRegs[D]->setText("D: 0"); flagRegs[I]->setText("CY : 0");
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
    consoleSP = new QSplitter;
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
    prt = new Ports();
    stk = new Stack();
    vrb = new Variables();
    utl = new Utils();
    utilsTabWidget->addTab(vrb, tr("Variales"));
    utilsTabWidget->addTab(prt, tr("Ports"));
    utilsTabWidget->addTab(stk, tr("Stack"));
    utilsTabWidget->addTab(utl, tr("Utilities"));

    codeViewSP->addWidget(codeView); codeViewSP->addWidget(hexView);
    initRegsView();
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
    consoleSP->setOrientation(Qt::Horizontal);
    consoleSP->addWidget(console); consoleSP->addWidget(regCodeSP);
    consoleSP->setSizes(QList<int>({450, 10}));
    topMainLayout->addWidget(consoleSP); //topMainLayout->addWidget(regCodeSP);
    topWidget->setLayout(topMainLayout);
    utilsSP->addWidget(log); utilsSP->addWidget(utilsTabWidget);
    bottomMainLayout->addWidget(utilsSP);
    bottomWidget->setLayout(bottomMainLayout);
    topBottomSP->setOrientation(Qt::Vertical);
    topBottomSP->addWidget(topWidget); topBottomSP->addWidget(bottomWidget);
    mainLayout->addWidget(topBottomSP);
    this->setLayout(mainLayout);

    connect(hexView, SIGNAL(currentRowChanged(int)), this, SLOT(setCodeView(int)));
    connect(codeView, SIGNAL(currentRowChanged(int)), this, SLOT(setHexView(int)));
}

Simulate::~Simulate() {
}

void Simulate::setHexView(int param) {
    hexView->setCurrentRow(param);
}

void Simulate::setCodeView(int param) {
    codeView->setCurrentRow(param);
}

void Simulate::resetUi() {
    clearLog();
    codeView->clear();
    hexView->clear();
    updateFlagRegs(0, 0, 1);
    //clear console
}
