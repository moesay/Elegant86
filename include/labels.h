#ifndef LABLES_H
#define LABLES_H

#include <tuple>
#include <QString>
#include <QList>

using Label_T = std::tuple<int, QString>;       //{Label offset, The label itself}

class Labels {
public:
    Labels(const Labels&) = delete;
    static bool labelExists(QString);
    static void addLabel(const int&, QString);
    static void clearAll();
    static bool isLableDef(QString);
private:
    static QList<Label_T> labelsList;
    Labels() {}
};

#endif
