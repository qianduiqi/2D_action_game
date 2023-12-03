#include "start.h"

#include <QApplication>
#include<QScreen>
#include<QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    start::getInstance();

    //在第二个屏幕上显示
    QList<QScreen*> screens = QGuiApplication::screens();
    if (screens.count() > 1) {
        QRect screenGeometry = screens[1]->geometry();
        // 计算中心位置
        int x = screenGeometry.x() + (screenGeometry.width() - start::getInstance().width()) / 2;
        int y = screenGeometry.y() + (screenGeometry.height() - start::getInstance().height()) / 2;
        start::getInstance().move(x, y);
    }

    start::getInstance().show();
    return a.exec();
}

