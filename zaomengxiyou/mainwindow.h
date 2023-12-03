#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QTimer>
#include <QMainWindow>
#include<QLabel>
#include<QKeyEvent>
#include<QTimer>
#include<QList>
#include<QProgressBar>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    QTimer* generate_monster_timer;
    QLabel* mainCharacter;
    QLabel* boss;
    //角色攻击方法
    void attack();
    void hitMonster(int i);
    //怪物移动
    void monsterMove();

public slots:

    //Boss攻击
    void bossAttack();
    void moveCharacter();
    void generate_monster();
private:
    //怪物移动的定时器
    QTimer* monster_move;
    //Boss攻击的定时器
    QTimer* boss_attack;
    QTimer* moveTimer;
    int moveSpeed;
    bool moveLeft;
    bool moveRight;
    QLabel* label;
    Ui::MainWindow *ui;
    //地面的纵坐标
    int ground_y;
    bool jump;
    //两段跳
    int jump_count;
    QList<QLabel*> monsterList;
    int first_monster_count;
    int second_monster_count;

    //血条
    QProgressBar* bar_role;
    QProgressBar* bar_boss_1;


    //血量
    int role_health;
    QList<int> monster_health;
};
#endif // MAINWINDOW_H
