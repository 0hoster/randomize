#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QListWidgetItem>

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
    QList<int> vips = {};
    bool is_vip_editing=false;
    bool is_vip(int student);

    struct Index {
        int i;
        double v;

        bool operator<(Index const &o) const {
            return this->v < o.v;
        }
    };

private slots:

    void set_min_limit();

    void start();

    void set_all();

    void set_all_range();

    void edit_vip();

    void set_vip(QListWidgetItem *item);

};

#endif // WIDGET_H
