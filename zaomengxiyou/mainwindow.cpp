#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "start.h"
#include<QRandomGenerator>
#include<QMessageBox>
#include<QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap originalPixmap(":/image/stage2.png");
    QPixmap scaledPixmap=originalPixmap.scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(scaledPixmap));
    this->setPalette(palette);
    //设置窗口大小不可调整
    this->setFixedSize(this->size());
    moveTimer=new QTimer(this);
    moveLeft=false;
    moveRight=false;
    jump=false;

    moveTimer->start(16);
    connect(moveTimer,SIGNAL(timeout()),this,SLOT(moveCharacter()));
    ground_y=400;
    jump_count=2;
    generate_monster_timer=new QTimer();

    connect(generate_monster_timer,SIGNAL(timeout()),this,SLOT(generate_monster()));
    first_monster_count=0;
    second_monster_count=0;
    //初始化主角和怪物们的血量
    role_health=500;
    for(int i=0;i<5;i++)
    {
        if(i==4)
        {
            monster_health.append(1000);
            break;
        }
        monster_health.append(100);
    }
    //主角和boss血条显示
    bar_role=new QProgressBar(this);
    bar_boss_1=new QProgressBar(this);
    bar_role->setRange(0,500);
    //初始生命值
    bar_role->setValue(role_health);
    bar_boss_1->setRange(0,1000);
    bar_role->setGeometry(70,30,40,20);
    bar_boss_1->setGeometry(640,30,40,20);
    bar_boss_1->setValue(monster_health[4]);
    bar_boss_1->hide();
    bar_role->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid grey;"  // 设置边框
        "   border-radius: 5px;"     // 设置边框圆角（如果需要的话）
        "   text-align: center;"     // 文本居中显示
        "}"
        "QProgressBar::chunk {"      // "chunk"是进度条的填充部分
        "   background-color: red;"  // 设置为红色
        "}"
        );
    bar_boss_1->setStyleSheet(
        "QProgressBar {"
        "   border: 2px solid grey;"  // 设置边框
        "   border-radius: 5px;"     // 设置边框圆角（如果需要的话）
        "   text-align: center;"     // 文本居中显示
        "}"
        "QProgressBar::chunk {"      // "chunk"是进度条的填充部分
        "   background-color: red;"  // 设置为红色
        "}"
        );

    mainCharacter=new QLabel(this);
    boss=new QLabel(this);
    mainCharacter->setText("唐僧：");
    boss->setText("Boss：");
    //设置字体颜色为红色
    QPalette role_palette=mainCharacter->palette();
    role_palette.setColor(QPalette::WindowText,Qt::red);
    mainCharacter->setPalette(role_palette);
    boss->setPalette(role_palette);

    mainCharacter->setGeometry(30,30,40,20);
    boss->setGeometry(600,30,40,20);
    boss->hide();

    monster_move=new QTimer(this);
    boss_attack=new QTimer();


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent* event)
{
    if(event->key()==Qt::Key_A)
        moveLeft=true;
    else if(event->key()==Qt::Key_D)
        moveRight=true;
    else if(event->key()==Qt::Key_K&&jump_count>0)
        jump=true;
    else if(event->key()==Qt::Key_J&&role_health>0)
        attack();
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if(event->key()==Qt::Key_A)
        moveLeft=false;
    else if(event->key()==Qt::Key_D)
        moveRight=false;
}

void MainWindow::attack()
{

    QLabel* bullet=new QLabel(this);
    bullet->setPixmap(QPixmap(":/image/tang_attack.png"));
    bullet->setGeometry(label->x(),label->y(),100,100);
    bullet->show();
    QTimer* timer=new QTimer(this);

    connect(timer,&QTimer::timeout,[bullet,this,timer](){
        bullet->move(bullet->x()+5,bullet->y());
        //碰撞检测
        for(int i=0;i<monsterList.size();++i)
        {
            if(monsterList[i])
            {
                if(bullet->geometry().intersects(monsterList[i]->geometry()))
                {

                    hitMonster(i);
                    bullet->hide();
                    bullet->deleteLater();
                    if(timer->isActive())
                        timer->stop();
                    timer->deleteLater();
                    break;
                }else if(bullet->x()>this->width())
                {

                    if(timer->isActive())
                        timer->stop();
                    timer->deleteLater();
                    bullet->deleteLater();
                    break;
                }
            }

        }
    });
    timer->start(10);
}

void MainWindow::hitMonster(int i)
{
    monster_health[i]-=50;
    //boss血条的减少
    if(i==4)
    {
        bar_boss_1->setValue(monster_health[i]);
    }
    //血量值降为0触发死亡事件
    if(monster_health[i]<=0&&monsterList[i])
    {
        monsterList[i]->hide();
        monsterList[i]->deleteLater();
        monsterList[i]=nullptr;
        if(i==4)
        {
            boss_attack->stop();
            QMessageBox::information(this,"对局结果","获胜！");
        }
    }

}

void MainWindow::monsterMove()
{

    //判断方向
    foreach(QLabel* monsterLabel,monsterList)
    {
        if(!monsterLabel)
            continue;

        QPoint monsterPos=monsterLabel->pos();
        //怪物每次移动的步长
        int stepSize=1;
        if(!label)
            return;
        int x= label->x();

        if(monsterPos.x()<=x)
        {

            //怪物在主角的左边
            int monster_x=monsterPos.x()+stepSize;
            monsterPos.setX(monster_x);
            monsterLabel->move(monsterPos);
            //在一定的距离内主角持续掉血
            if(x-monster_x<20&&role_health)
            {

                role_health-=1;
                bar_role->setValue(role_health);
                if(role_health==0)
                {
                    if(monster_move->isActive())
                        monster_move->stop();
                    monster_move->deleteLater();
                    monster_move=nullptr;
                    if(moveTimer->isActive())
                        moveTimer->stop();
                    moveTimer->deleteLater();
                    label->hide();
                    label->deleteLater();
                    QMessageBox::critical(this,"战斗状态","您已阵亡！");
                }
            }
        }else if(monsterPos.x()>=x)
        {
            //怪物在主角的右边
            int monster_x=monsterPos.x()-stepSize;
            monsterPos.setX(monster_x);
            monsterLabel->move(monsterPos);
            if(monster_x-x<20&&role_health)
            {
                role_health-=1;
                bar_role->setValue(role_health);
                if(role_health==0)
                {


                    if(monster_move->isActive())
                        monster_move->stop();
                    monster_move->deleteLater();
                    monster_move=nullptr;
                    label->hide();


                    if(moveTimer->isActive())
                        moveTimer->stop();
                    moveTimer->deleteLater();

                    QMessageBox::critical(this,"战斗状态","您已阵亡");
                }
            }
        }
    }
}

void MainWindow::bossAttack()
{
    //boss攻击逻辑
    QLabel* bullet=new QLabel(this);
    bullet->setPixmap(QPixmap(":/image/tang_attack.png"));
    bullet->setGeometry(monsterList[4]->x(),monsterList[4]->y(),100,100);
    bullet->show();
    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,[bullet,this,timer](){
        bullet->move(bullet->x()-5,bullet->y());
        //碰撞检测
        if(bullet->geometry().intersects(label->geometry()))
        {
            qDebug()<<"111";
            role_health-=100;
            bar_role->setValue(role_health);
            if(role_health<=0&&label)
            {
                if(moveTimer->isActive())
                    moveTimer->stop();
                moveTimer->deleteLater();
                bar_role->setValue(0);
                label->deleteLater();
                QMessageBox::critical(this,"战斗状态","您已阵亡！");
                //boss停止攻击
                if(boss_attack->isActive())
                    boss_attack->stop();
            }
            if(timer->isActive())
                timer->stop();
            timer->deleteLater();
            bullet->deleteLater();
        }else if(bullet->x()<0)
        {
            if(timer->isActive())
                timer->stop();
            timer->deleteLater();
            bullet->deleteLater();
        }
    });
    timer->start(10);

    //每隔一段随机的时间发起攻击
    if(boss_attack->isActive())
        boss_attack->stop();
    srand(QTime::currentTime().msec());
    int random= rand()%10;
    if(role_health>0&&monster_health[4]>0)
        boss_attack->start(random*1000);
}

//移动人物
void MainWindow::moveCharacter()
{
    label= start::getInstance().get_rs().getImageLabel();
    //AD键实现左右移动
    if(!label)
        return;
    int x=label->x();
    int y=label->y();

    if(moveLeft)
        x-=5;
    if(moveRight)
        x+=5;
    //按k实现两段跳跃
    if(jump&&jump_count>0)
    {
        y-=160;
        jump_count--;
        jump=false;
    }

    //下落
    if(y<ground_y)
    {
        y+=2;
    }
    if(y==ground_y)
        jump_count=2;

    label->move(x,y);

}
//生成怪物
void MainWindow::generate_monster()
{
    if(monster_move&& !monster_move->isActive())
    {

        connect(monster_move,&QTimer::timeout,this,&MainWindow::monsterMove);
        monster_move->start(16);
    }


    monsterList.append(new QLabel(this));
    QLabel* label=monsterList.last();
    QPixmap pixmap;
    label->resize(100,100);
    label->show();

    //生成怪物1
    if(++first_monster_count<=2)
    {
        pixmap.load(":/image/first_monster.png");
        QPixmap scaledPixmap=pixmap.scaled(100,100,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        label->setPixmap(scaledPixmap);
        //随机位置生成

        int randomX=QRandomGenerator::global()->bounded(0,this->width()-label->width());
        label->move(randomX,400);
    }else if(++second_monster_count<=2)//生成怪物2
    {
        pixmap.load(":/image/second_monster.png");
        QPixmap scaledPixmap=pixmap.scaled(100,100,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        label->setPixmap(scaledPixmap);
        //随机位置生成
        int randomX=QRandomGenerator::global()->bounded(0,this->width()-label->width());
        label->move(randomX,400);
        if(second_monster_count==2)
        {
            generate_monster_timer->stop();
            generate_monster_timer->start(30000);
        }
    }else{
        //生成boss
        pixmap.load(":/image/first_boss.png");
        QPixmap scaledPixmap=pixmap.scaled(100,100,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
        label->setPixmap(scaledPixmap);
        label->move(600,400);
        generate_monster_timer->stop();
        boss->show();
        bar_boss_1->show();
        if(monster_move->isActive())
            monster_move->stop();
        //对主角发动攻击
        if(!label->isHidden())
        {
            bossAttack();
        }
        boss_attack->start(100);
        connect(boss_attack,SIGNAL(timeout()),this,SLOT(bossAttack()));
    }
}


