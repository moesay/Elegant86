#include <include/labels.h>

QList<Label_T> Labels::labelsList;

bool Labels::labelExists(const QString& lbl) {
    QString buf = lbl;
    buf.remove(':');

    for(auto entry : Labels::labelsList) {
        auto [bytes, lblId] = entry;
        if(lblId == buf)
            return true;
    }
    return false;
}

void Labels::addLabel(const int& bytes, const QString& lbl) {
    QString buf = lbl;
    Labels::labelsList.append(std::make_tuple(bytes, buf.remove(':')));
}
