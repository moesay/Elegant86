#ifndef SIMULATE_CLASS_H
#define SIMULATE_CLASS_H

#include <QtWidgets>

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
        QListView *listView;
};

#endif
