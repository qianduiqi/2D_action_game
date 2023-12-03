#ifndef ROLESELECT_H
#define ROLESELECT_H

#include <QWidget>
#include "mainwindow.h"
#include<QLabel>
namespace Ui {
class RoleSelect;
}

class RoleSelect : public QWidget
{
    Q_OBJECT

public:
    explicit RoleSelect(QWidget *parent = nullptr);
    ~RoleSelect();
    QLabel* getImageLabel();
    static RoleSelect& getInstance();
private slots:
    void on_pushButton_clicked();
signals:

private:
    static RoleSelect* instance;
    QLabel* imageLabel;
    MainWindow* mw;
    Ui::RoleSelect *ui;
};


#endif // ROLESELECT_H
