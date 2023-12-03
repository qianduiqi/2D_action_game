#ifndef START_H
#define START_H

#include <QWidget>
#include "roleselect.h"
namespace Ui {
class start;
}

class start : public QWidget
{
    Q_OBJECT

public:
    explicit start(QWidget *parent = nullptr);
    ~start();
    static start& getInstance();
    QString getName();
    //获得rs指针
    RoleSelect& get_rs();
private slots:
    void on_startGame_clicked();

private:
    RoleSelect rs;
    Ui::start *ui;
};

#endif // START_H
