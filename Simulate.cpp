#include "Simulate.h"

Simulate::Simulate(QWidget *parent) : QWidget(parent) {
    setupUi();
}

void Simulate::setupUi() {
    QListView *listView = new QListView();
    QPushButton *btn = new QPushButton("AA");
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(listView);
    layout->addWidget(btn);
    this->setLayout(layout);

}
