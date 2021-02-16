#ifndef LABLES_H
#define LABLES_H

#include <tuple>
#include <QString>
#include <QList>

using Label_T = std::tuple<int, QString>;       //{Label offset, The label itself}

class Labels {
public:
    Labels(const Labels&) = delete;
    static bool labelExists(const QString&);
    static void addLabel(const int&, const QString&);
//private:
    static QList<Label_T> labelsList;
    Labels() {}
};

#endif
