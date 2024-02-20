#include "widget.h"
#include "./ui_widget.h"
#include <queue>
#include <QRandomGenerator>
#include <algorithm>

Widget::Widget(QWidget *parent)
        : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    connect(ui->btn_start, &QPushButton::clicked, this, &Widget::start);
    connect(ui->spinBox_from, &QSpinBox::valueChanged, this, &Widget::set_min_limit);
    connect(ui->checkBox_all, &QCheckBox::toggled, this, &Widget::set_all);
    connect(ui->btn_vip, &QPushButton::clicked, this, &Widget::edit_vip);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &Widget::set_vip);
    QString vip_text("当前%0位Vip");
    ui->label_vip->setText(vip_text.arg(vips.size()));
}

Widget::~Widget() {
    delete ui;
}

void Widget::start() {
    is_vip_editing=false;
    QString vip_text("当前%0位Vip");
    ui->label_vip->setText(vip_text.arg(vips.size()));
    int mn = ui->spinBox_from->value();
    int mx = ui->spinBox_to->value() + 1;
    if (mn + 1 >= mx) {
//        ui->text_edit->setText(tr("可选项太少"));
        ui->spinBox_to->setValue(mn + 1);
        return;
    }
    int total = ui->spinBox_total->value();

    std::priority_queue<Index> index;
    ui->listWidget->clear();
    QListWidgetItem *item;
    QIcon normal(":/images/student.bmp");
    QIcon vip(":/images/studentVIP.bmp");
    for (int i = 0; i < total; ++i) {
        if (index.empty()) {
            for (int j = mn; j < mx; ++j) {
                if (is_vip(j)) {
                    index.push({j, .75 + QRandomGenerator::global()->bounded(.25)});
                } else {
                    index.push({j, QRandomGenerator::global()->generateDouble()});
                }
            }
        }
        QString cur;
        cur.setNum(index.top().i);
        item = new QListWidgetItem;
        item->setText(cur);
        if (is_vip(index.top().i)) {
            item->setIcon(vip);
        } else {
            item->setIcon(normal);
        }
        item->setFlags(Qt::ItemIsEnabled);
        ui->listWidget->addItem(item);
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

bool Widget::is_vip(int student) {
    auto pos = std::lower_bound(vips.begin(), vips.end(), student);
    if (pos == vips.end() || *pos != student) {
        return false;
    } else return true;
}

void Widget::set_vip(QListWidgetItem *item) {
    if (!is_vip_editing)return;
    int cur = item->text().toInt();
    if (is_vip(cur)) {
        vips.removeAt(std::lower_bound(vips.begin(), vips.end(), cur)-vips.begin());
    } else {
        vips.push_back(cur);
        std::sort(vips.begin(), vips.end());
    }
    edit_vip();
}

void Widget::edit_vip() {
    ui->listWidget->clear();
    QListWidgetItem *item;
    QIcon normal(":/images/student.bmp");
    QIcon vip(":/images/studentVIP.bmp");

    for (int i = ui->spinBox_from->value(); i <= ui->spinBox_to->value(); ++i) {
        QString cur;
        cur.setNum(i);
        item = new QListWidgetItem;
        item->setText(cur);
        if (is_vip(i)) {
            item->setIcon(vip);
        } else {
            item->setIcon(normal);
        }
        item->setFlags(Qt::ItemIsEnabled);
        ui->listWidget->addItem(item);
    }
    is_vip_editing=true;
    ui->label_vip->setText(tr("双击以切换Vip"));
}

