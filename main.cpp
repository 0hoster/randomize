#include "widget.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QFontDatabase>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    int fontId = QFontDatabase::addApplicationFont(":/font/Poppins-Light.ttf");
    if (fontId >= 0) {
        QString my_font = QFontDatabase::applicationFontFamilies(fontId).at(0);
        qDebug() << "Font loaded successfully";
        QFont font(my_font);
        QApplication::setFont(font);
    } else qDebug() << "fontId:" << fontId << " failed";

    Widget w;
    w.setWindowTitle(QObject::tr("随机"));
    w.show();
    return QApplication::exec();
}
