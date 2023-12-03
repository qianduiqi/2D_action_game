#include "start.h"
#include "ui_start.h"
#include<QPixmap>
#include<QPalette>
#include<QBrush>
#include<QMessageBox>
start::start(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::start)
{
    ui->setupUi(this);


    QPixmap originalPixmap(":/image/start.PNG");
    QPixmap scaledPixmap=originalPixmap.scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(scaledPixmap));
    this->setPalette(palette);

    //设置窗口大小不可调整
    this->setFixedSize(this->size());
}

start::~start()
{
    delete ui;

}

start& start::getInstance()
{
    static start s;
    return s;
}

QString start::getName()
{
    return ui->nameLineEdit->text();
}

RoleSelect& start::get_rs()
{
    return this->rs.getInstance();
}

void start::on_startGame_clicked()
{
    if(ui->nameLineEdit->text().isEmpty())
    {
        QMessageBox::information(this,"开始界面","请输入昵称！");
        return;
    }

    //隐藏当前界面
    this->hide();
    rs.getInstance().show();

}


