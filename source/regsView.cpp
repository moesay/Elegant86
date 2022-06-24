#include <include/Simulate.h>

void Simulate::addRoot(const QString& regName,
        const QString& hReg, const QString& lReg) {
    QTreeWidgetItem *itm = new QTreeWidgetItem(regsView);
    itm->setText(0, regName);
    itm->setText(1, "0000");
    if(!hReg.isEmpty()) {
        QTreeWidgetItem *hreg = new QTreeWidgetItem(itm);
        QTreeWidgetItem *lreg = new QTreeWidgetItem(itm);
        hreg->setText(0, hReg); hreg->setText(1, "00");
        lreg->setText(0, lReg); lreg->setText(1, "00");
        itm->addChild(hreg); itm->addChild(lreg);
    }
    regsView->addTopLevelItem(itm);
}

void Simulate::updateRegView(const int& reg, const QString & val) {
    switch(reg) {
        case AX:
            regsView->topLevelItem(0)->setText(1, val); break;
        case AH:
            regsView->topLevelItem(0)->child(0)->setText(1, val); break;
        case AL:
            regsView->topLevelItem(0)->child(1)->setText(1, val); break;
        case BX:
            regsView->topLevelItem(1)->setText(1, val); break;
        case BH:
            regsView->topLevelItem(1)->child(0)->setText(1, val); break;
        case BL:
            regsView->topLevelItem(1)->child(1)->setText(1, val); break;
        case CX:
            regsView->topLevelItem(2)->setText(1, val); break;
        case CH:
            regsView->topLevelItem(2)->child(0)->setText(1, val); break;
        case CL:
            regsView->topLevelItem(2)->child(1)->setText(1, val); break;
        case DX:
            regsView->topLevelItem(3)->setText(1, val); break;
        case DH:
            regsView->topLevelItem(3)->child(0)->setText(1, val); break;
        case DL:
            regsView->topLevelItem(3)->child(1)->setText(1, val); break;
        case SP:
            regsView->topLevelItem(4)->setText(1, val); break;
        case BP:
            regsView->topLevelItem(5)->setText(1, val); break;
        case SI:
            regsView->topLevelItem(6)->setText(1, val); break;
        case DI:
            regsView->topLevelItem(7)->setText(1, val); break;
        case IP:
            regsView->topLevelItem(8)->setText(1, val); break;
        case ES:
            regsView->topLevelItem(9)->setText(1, val); break;
        case CS:
            regsView->topLevelItem(10)->setText(1, val); break;
        case SS:
            regsView->topLevelItem(11)->setText(1, val); break;
        case DS:
            regsView->topLevelItem(12)->setText(1, val); break;
    }
}

void Simulate::initRegsView() {
    regsView->setColumnCount(2);
    regsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    regsView->setHeaderLabels({tr("Register"), tr("Value")});
    addRoot("AX", "AH", "AL");
    addRoot("BX", "BH", "BL");
    addRoot("CX", "CH", "CL");
    addRoot("DX", "DH", "DL");
    addRoot("SP");
    addRoot("BP");
    addRoot("SI");
    addRoot("DI");
    addRoot("IP");
    addRoot("ES");
    addRoot("CS");
    addRoot("SS");
    addRoot("DS");
    regsView->expandAll();
}

