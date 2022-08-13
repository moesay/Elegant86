#include "labels.h"

QList<Label_T> Labels::labelsList;

bool Labels::labelExists(QString lbl) {
    lbl.remove(':');
    for(auto entry : Labels::labelsList) {
        auto [bytes, lblId] = entry;
        if(lblId == lbl)
            return true;
    }
    return false;
}

void Labels::addLabel(const int& bytes, QString lbl) {
    lbl = lbl.remove(':');
    Labels::labelsList.append(std::make_tuple(bytes, lbl.toUpper()));
}

int Labels::getLabelAddr(const QString& param) {
    for(auto entry : Labels::labelsList) {
        auto [addr, lbl] = entry;
        if(lbl == param)
            return addr;
    }
    return 0;
}

void Labels::clearAll() {
    labelsList.clear();
}
