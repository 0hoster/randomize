#include "widget.h"
#include "./ui_widget.h"
#include <queue>
#include <QRandomGenerator>

Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    connect(ui->btn_start, &QPushButton::clicked, this, &Widget::start);
    connect(ui->spinBox_from, &QSpinBox::valueChanged, this, &Widget::set_min_limit);
    connect(ui->checkBox_all,&QCheckBox::toggled,this,&Widget::set_all);
}

Widget::~Widget() {
    delete ui;
}

void Widget::start() {
    int mn = ui->spinBox_from->value();
    int mx = ui->spinBox_to->value() + 1;
    if (mn + 1 >= mx) {
        ui->text_edit->setText(tr("可选项太少"));
        ui->spinBox_to->setValue(mn + 1);
        return;
    }
    int total = ui->spinBox_total->value();
    struct Index {
        int i;
        unsigned int v;

        bool operator<(Index const &o) const {
            return this->v < o.v;
        }
    };

    std::priority_queue<Index> index;
    ui->text_edit->clear();
    for (int i = 0; i < total; ++i) {
        if (index.empty()) {
            for (int j = mn; j < mx; ++j) {
                index.push({j, QRandomGenerator::global()->generate()});
            }
        }
        QString cur = QString::asprintf("%2d:%2d\n", i + 1, index.top().i);
        ui->text_edit->append(cur);
        index.pop();
    }
}

void Widget::set_min_limit() {
    int mn = ui->spinBox_from->value();
    ui->spinBox_to->setMinimum(mn + 1);
}

void Widget::set_all() {
    if (turn_all == !ui->checkBox_all->isChecked()) {
        turn_all = ui->checkBox_all->isChecked();
        if (turn_all) {
            ui->spinBox_total->setReadOnly(true);
            set_all_range();
            connect(ui->spinBox_from, &QSpinBox::valueChanged, this, &Widget::set_all_range);
            connect(ui->spinBox_to, &QSpinBox::valueChanged, this, &Widget::set_all_range);
        } else {
            ui->spinBox_total->setReadOnly(false);
            disconnect(ui->spinBox_from, &QSpinBox::valueChanged, this, &Widget::set_all_range);
            disconnect(ui->spinBox_to, &QSpinBox::valueChanged, this, &Widget::set_all_range);
        }
    }

}

void Widget::set_all_range() {
    int range = ui->spinBox_to->value() - ui->spinBox_from->value() + 1;
    ui->spinBox_total->setValue(range >= 1 ? range : 1);
}


