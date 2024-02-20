#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget {
Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;
private:
    Ui::Widget *ui;
    bool turn_all = false;

private slots:
    void set_min_limit();
    void start();
    void set_all();
    void set_all_range();
};

#endif // WIDGET_H
