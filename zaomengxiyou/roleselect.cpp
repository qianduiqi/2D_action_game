#include "roleselect.h"
#include "ui_roleselect.h"
#include<QMessageBox>
#include "start.h"
#include <QLabel>
#include<QPixmap>
RoleSelect::RoleSelect(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RoleSelect)
{
    ui->setupUi(this);
    mw=new MainWindow;
    imageLabel=nullptr;
    QPixmap originalPixmap(":/image/roleselect.png");
    QPixmap scaledPixmap=originalPixmap.scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(scaledPixmap));
    this->setPalette(palette);

    //设置窗口大小不可调整
    this->setFixedSize(this->size());

     imageLabel=new QLabel(mw);
}

RoleSelect::~RoleSelect()
{
    delete ui;
}

QLabel *RoleSelect::getImageLabel()
{
    return this->imageLabel;
}

RoleSelect & RoleSelect::getInstance()
{
    static RoleSelect rs;
    return rs;
}


void RoleSelect::on_pushButton_clicked()
{
    if(!(ui->tangCheckBox->isChecked()^ui->shaCheckBox->isChecked()))
    {
        QMessageBox::information(this,"角色选择","请选择一个角色！");
        return;
    }

    //在指定位置显示图片

    QPixmap pixmap;
    if(ui->tangCheckBox->isChecked())
    {
        pixmap.load(":/image/monktang.png");
    }else{
        pixmap.load(":/image/monksha.png");

    }

    QPixmap scaledPixmap = pixmap.scaled(100, 100,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    imageLabel->resize(scaledPixmap.size());
    imageLabel->setPixmap(scaledPixmap);
    imageLabel->move(100,400);


    QString name= start::getInstance().getName();
    mw->setWindowTitle(name+"的冒险之旅！");
    this->hide();
    mw->show();

    //判断是选择了哪个角色
    if(ui->shaCheckBox->isChecked())
    {
        mw->mainCharacter->setText("沙僧：");
    }else{
        mw->mainCharacter->setText("唐僧：");
    }

    mw->generate_monster_timer->start(5000);
}

