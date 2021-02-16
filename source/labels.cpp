#include <include/labels.h>

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
    Labels::labelsList.append(std::make_tuple(bytes, lbl));
}

/*
 * Should've been implemented using regex but, who cares. i hate regex.
 * Maybe later
 */
bool Labels::isLableDef(QString lbl) {
    bool init = (lbl.startsWith('.') && lbl.endsWith(':'));
    lbl = lbl.left(lbl.length() -1);
    lbl = lbl.right(lbl.length() -1);
    return (!lbl.contains('.') && !lbl.contains(':') && lbl.length() > 0 && init);
}

void Labels::clearAll() {
    labelsList.clear();
}
