#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QPushButton>
#include <QPainter>
#include <QDebug>
#include <QTableView>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFontDialog>
#include <QFrame>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "sudoku.h"

static const int saveloadNumber=5;
static QString str_load[saveloadNumber]={"存档1","存档2","存档3","存档4","存档5"};
static QString str_save[saveloadNumber]={"位置1","位置2","位置3","位置4","位置5"};
static QString str_hardclass[4]={"简单","中等","困难","地狱"};
static int levelnow=0;
static int helpTimes=3;
static bool begining=0;
static const int m_minsize=550;
static int x_t=0,y_t=0;//current index of 9*9
static QFont ft1("宋体",13);
static QFont ft2("Times New Roman",13);
static QFont ft3("Times New Roman",11);


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),Game(nullptr)
{
    ui->setupUi(this);
    m_widget = new QStackedWidget;

    b_w = new QStackedWidget;
    QWidget *b_widget = new QWidget;
    QWidget *b_widget_select = new QWidget;
    b_w->addWidget( b_widget);
    b_w->addWidget( b_widget_select );
    b_w->setCurrentIndex(0);
    QString b_str[2]={"开始游戏","规则简介"};
    QVBoxLayout *b_layout = new QVBoxLayout(b_widget);
    b_layout->addStretch(1);
    for(int i=0; i<2 ;++i)
    {
        QPushButton *but=new QPushButton(b_str[i]);
        but->setFont(ft1);
        but->setMinimumSize(80,60);
        b_layout->addWidget(but);
        connect(but,SIGNAL(clicked(bool)),this,SLOT(b_buttonClicked()));
        b_layout->addStretch(1);
    }
//    b_layout->addStretch(1);
    b_layout->setMargin(180);
    m_widget->addWidget(b_w);

    QVBoxLayout *b_sele_layout = new QVBoxLayout(b_widget_select);
    b_sele_layout->addStretch(1);
    for( int i=0 ; i<4 ; ++i)
    {
        QPushButton *s= new QPushButton(str_hardclass[i]);
        connect( s ,SIGNAL(clicked(bool)),this,SLOT(b_SelectedHardClicked()));
        s->setMinimumSize(80,60);
        b_sele_layout->addWidget(s);
        b_sele_layout->addStretch(1);
    }
    b_sele_layout->setMargin(180);

    QGridLayout* gl2 = new QGridLayout;
    key=new QPushButton*[10];
    QString a;
    for(int i=0;i<9;++i)
    {
        key[i]=new QPushButton(a.setNum(i+1));
        key[i]->setFont(ft3);
        gl2->addWidget(key[i],i/3,i%3,Qt::AlignCenter|Qt::AlignHCenter);
        key[i]->setMinimumSize(40,40);
        connect(key[i],SIGNAL(clicked(bool)),this,SLOT(keyboardclicked()));
    }
    key[9]=new QPushButton(tr("C"));
    QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor("#ff0000"));
    key[9]->setPalette(pal);
    gl2->addWidget(key[9],3,1,Qt::AlignCenter|Qt::AlignHCenter);
    key[9]->setMinimumSize(40,40);
    key[9]->setFont(ft3);
    for( int i=0 ; i<3; ++i)
    {
        gl2->setRowStretch(i,1);
        gl2->setColumnStretch(i,1);
    }
    gl2->setRowStretch(3,1);
    connect(key[9],SIGNAL(clicked(bool)),this,SLOT(keyboardclicked()));

    widget = new QWidget();
    m_widget->addWidget(widget);
    this->setWindowTitle(tr("数独小游戏"));
    m_widget->setCurrentIndex(0);
    this->setCentralWidget(m_widget);
    widget2 = new QFrame(this);
    QGridLayout* gl = new QGridLayout(widget);
    b= new QPushButton**[9];
    for(int i=0 ;i<9 ;++i)
    {
        b[i]=new QPushButton*[9];
        for( int j=0;j<9;++j)
        {
            b[i][j]=new QPushButton;
            b[i][j]->setMinimumSize(40,40);
            b[i][j]->setFont(ft2);
            gl->addWidget(b[i][j],i,j);
            b[i][j]->setAccessibleName(QString::number(i*10+j));
            connect(b[i][j],SIGNAL(clicked()),this,SLOT(tablebuttonClicked()));
            b[i][j]->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        }
    }
    gl->setMargin(50);
    gl->setVerticalSpacing(1);
    gl->setHorizontalSpacing(1);
    for(int i=0;i<9;++i)
    {
        gl->setColumnStretch(i,1);
        gl->setRowStretch(i,1);
    }
    this->setMinimumSize(m_minsize,m_minsize);

    for(int i=0; i<4 ; ++i)
    {
        QAction *s= new QAction(str_hardclass[i]);
        ui->newGame->addAction(s);
        connect(s,SIGNAL(triggered(bool)),this,SLOT(newGameClicked()));
    }
    for(int i=0; i<saveloadNumber ; ++i)
    {
        QAction *s= new QAction(str_save[i]);
        ui->saveGame->addAction(s);
        connect(s,SIGNAL(triggered(bool)),this,SLOT(saveGameClicked()));
    }
    for(int i=0; i<saveloadNumber ; ++i)
    {
        QAction *s= new QAction(str_load[i]);
        ui->loadGame->addAction(s);
        connect(s,SIGNAL(triggered(bool)),this,SLOT(loadGameClicked()));
    }

    gl2->setSpacing(0);
    widget2->setMinimumSize(120,160);
    widget2->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    widget2->setLayout(gl2);
    widget2->close();

    setButtonInit();

    this->resize( m_minsize+50,m_minsize+50);

    int wid=this->width();
    int hei=this->height();
    //获取设备屏幕大小
    QRect screenRect = QApplication::desktop()->screenGeometry();
    int wid2=screenRect.width();
    int hei2=screenRect.height();
    if( screenRect.height() >1040 )
    {
        double scale=double(hei2)/1080;
        wid*=scale;
        hei*=scale;
    }
    this->setMaximumSize(  double(hei2)/hei*wid  , hei2   );
    this->setGeometry(     (wid2-wid)/3 ,(hei2-hei)/3  ,wid ,hei  );
}
void MainWindow::resizeEvent(QResizeEvent *)
{
    widget2->show();
    widget2->resize( 1.0*widget->width()/m_minsize*120  , 1.0*widget->height()/m_minsize*160  );
    int a=widget2->width()/3;
    int b=widget2->height()/4;
    for( int i=0 ; i<10 ;++i)
    {
        key[i]->resize(a,b);
    }
    key[9]->resize(a-2,b-2);
    widget2->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::tablebuttonClicked()
{
    widget2->close();
    QPushButton*btn = qobject_cast<QPushButton*>(sender());
    int n=btn->accessibleName().toInt();
    int i=n/10,j=n%10;
    x_t=i;y_t=j;
    if(!clickable[x_t][y_t])
        return;
    QRect r1=btn->geometry();
    int xa=b[0][0]->width();
    int m1=  widget2->width() ;
    int m2=  widget2->height() ;
    if(  this->height()-r1.y()-10  > m2 )
        widget2->move( r1.x()-(m1-xa)/2-3 ,  r1.y()+10  );
    else
        widget2->move( r1.x()-(m1-xa)/2-3 ,  this->height()-m2 );
    widget2->show();
}

void MainWindow::keyboardclicked()
{
    widget2->close();

    if(b[x_t][y_t]->text()==("×"))
        return;

    QPushButton*btn = qobject_cast<QPushButton*>(sender());

    int num=btn->text().toInt();
    if(num)
    {
        b[x_t][y_t]->setText(QString::number(num));
        (*Game)[x_t][y_t]=num;
    }
    else
    {
        b[x_t][y_t]->setText("");
        (*Game)[x_t][y_t]=0;
    }
    recoverwrong();
    Game->check1(x_t,y_t,wrong1);
    Game->check2(x_t,y_t,wrong2);
    showAllWrong();
    if(checkwin())
    {
        if(levelnow==0)
            QMessageBox::information(this, tr("胜利了"), tr("简单难度，一般一般吧\n"
                                                         "敢不敢挑战个高点的难度？(#‵′)凸"));
        else if(levelnow==1)
            QMessageBox::information(this, tr("厉害了"), tr("厉害了，通关中等难度"
                                                         "你的智商已经超过百分之好多人了"));
        else if(levelnow==2)
            QMessageBox::information(this, tr("大师！！"), tr("大师！！大师！！\n"
                                                          "智商这么高我们做朋友吧"));
        else if(levelnow==3)
            QMessageBox::information(this, tr("大神！！！"), tr("大神收我一拜\n"
                                                           "在你这样的智商面前我已经没话可说了\n"
                                                           "你会不会是用了求解软件O__O"));
    }
}

bool MainWindow::checkwin()
{
    if(wrong1.empty()&&wrong2.empty()&&Game->checkwin())
        return 1;
    return 0;
}

void MainWindow::setButtonInit()
{
//    static char a1[] =  "background-color: #A2B5CD;" ;
    static char a2[] =  "background-color: #ffffff;" ;
    for(int i=0 ;i<9 ;++i)
    {
        for( int j=0;j<9;++j)
        {
            b[i][j]->setStyleSheet(a2);
        }
    }
}
void MainWindow::loadGameClicked()
{
    if(!begining)
        return;
    widget2->close();
    QAction *btn = qobject_cast<QAction*>(sender());
    int t=0;
    for( ; t< saveloadNumber ; ++t)
    {
        if( str_load[t]==btn->text() )
            break;
    }
    load(t);
}

void MainWindow::newGameClicked()
{
    widget2->close();
    if( begining)
    {
        if (QMessageBox::No == QMessageBox::question(this,tr("新游戏？"),
                                                  tr("确认放弃当前游戏？"),
                                                  QMessageBox::Yes | QMessageBox::No,
                                                  QMessageBox::Yes))
            return;
    }
    static QString str[4]={"简单","中等","困难","地狱"};
    QAction *btn = qobject_cast<QAction*>(sender());
    int i=0;
    for( ; i< 4 ; ++i)
    {
        if( str[i]==btn->text() )
            break;
    }
    levelnow=i;
    if( !begining)
    {
        begining=1;
        m_widget->setCurrentIndex(1);
    }
    beginGameClicked(i);
}
void MainWindow::saveGameClicked()
{
    widget2->close();
    QAction *btn = qobject_cast<QAction*>(sender());
    int i=0;
    for( ; i< saveloadNumber ; ++i)
    {
        if( str_save[i]==btn->text() )
            break;
    }
    save(i);
}

void MainWindow::load(int t)
{
    if(!begining)
        return;
    if(saved[t].empty())
    {
        QMessageBox::critical(this,tr("错误！"),tr("%1为空").arg( str_load[t]));
        return;
    }
    Game->set(saved[t]);
    for(int i=0 ;i<9 ;++i)
    {
        for( int j=0;j<9;++j)
        {
            if( saved[t][i][j])
                b[i][j]->setText(QString::number(saved[t][i][j]));
            else
                b[i][j]->setText("");
        }
    }
    recoverwrong();
    wrong1=save_wrong1[t];
    wrong2=save_wrong2[t];
    showAllWrong();
}

void MainWindow::save(int i)
{
    if(!begining)
        return;
    saved[i]=Game->copy();
    save_wrong1[i]=wrong1;
    save_wrong2[i]=wrong2;
    QMessageBox::information(this,tr("存档成功！"),tr("当前游戏已保存到%1").arg(str_save[i]));
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(begining==1)
    {
        int bm=ui->menuBar->geometry().bottom();
        QPainter p(this);
        QPen ss;
        ss.setWidth(3);
        ss.setColor(Qt::red);
        p.setPen(ss);

        for( int i=3 ; i<8; i+=3)
        {
            int x1= (b[0][i]->geometry().left()+b[0][i-1]->geometry().right())/2;
            int y1= b[0][i]->geometry().top();
            int y2=b[8][i]->geometry().bottom();
            p.drawLine( x1+1 ,y1+2+bm,x1+1,y2+bm );
        }
        for( int i=3 ; i<8; i+=3)
        {
            int y1= (b[i][0]->geometry().top()+b[i-1][0]->geometry().bottom())/2;
            int x1= b[i][0]->geometry().left();
            int x2=b[i][8]->geometry().right();
            p.drawLine( x1+2 ,y1+1+bm,x2-2,y1+1+bm );
        }
    }
}

void MainWindow::on_action_triggered()
{
    bool ok=false;
    qDebug()<<ft2;
    QFont font = QFontDialog::getFont(&ok,ft2,this,tr("主窗口数字字体选择"));
    if(ok)
    {
        ft2=font;
        for(int i=0 ;i<9 ;++i)
        {
            for( int j=0;j<9;++j)
            {
                b[i][j]->setFont(font);
            }
        }
    }
}

void MainWindow::on_action_2_triggered()
{
    bool ok=false;
    QFont font = QFontDialog::getFont(&ok,ft3,this,tr("小键盘数字字体选择"));
    if(ok)
    {
        ft3=font;
        for(int i=0 ;i<10 ;++i)
        {
            key[i]->setFont(font);
        }
    }
}

void MainWindow::beginGameClicked(int hard)
{
    delete Game;
    helpTimes=hard+2;
    Game = new SudoKu(hard);
    SudoKu & game=*Game;
    clickable=vvbool (9,std::vector<bool>(9,1));
    for(int i=0;i<saveloadNumber;++i)//saved number is 3
        saved[i].clear();
    QString a;
    for(int i=0;i<9;++i)
    {
        for(int j=0;j<9;++j)
        {
            b[i][j]->setStyleSheet("background-color: #ffffff;");
            if(game[i][j]!=0)
            {
                b[i][j]->setText(a.setNum(game[i][j]));
                b[i][j]->setStyleSheet("background-color: #A2B5CD;");
                clickable[i][j]=0;
            }
            else
                b[i][j]->setText("");
        }
    }
    recoverwrong();
//    for( int j=0; j<4; ++j)
//    {
//        long s=time(0);
//        for( int i=0 ;i<600; ++i)
//        {
//            SudoKu s(j);
//        }
//        qDebug()<<"600 Times: "<<j<<"take time: "<<time(0)-s;
//    }
}

void MainWindow::b_buttonClicked()
{
    static QString b_str[2]={"开始游戏","规则简介"};
    QPushButton*btn = qobject_cast<QPushButton*>(sender());
    if(btn->text()==b_str[0])
    {
        b_w->setCurrentIndex(1);
    }
    else if(btn->text()==b_str[1])
    {
        on_action_5_triggered();
    }
}
void MainWindow::b_SelectedHardClicked()
{
    QPushButton*btn = qobject_cast<QPushButton*>(sender());
    int i=0;
    for( ; i<4 ; ++i)
    {
        if(btn->text()==str_hardclass[i])
        {
            break;
        }
    }
    begining=1;
    m_widget->setCurrentIndex(1);
    beginGameClicked(i);
}

void MainWindow::showwrong1(int i, int j)
{
    static QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor("#ff0000"));
    b[i][j]->setPalette(pal);
}
void MainWindow::notshowwrong1(int i, int j)
{
    static QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor("#000000"));
    b[i][j]->setPalette(pal);
}

void MainWindow::showwrong2(int i, int j)
{
    QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor("#ff0000"));
    b[i][j]->setPalette(pal);
    b[i][j]->setText("×");
}
void MainWindow::notshowwrong2(int i, int j)
{
    QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor("#000000"));
    b[i][j]->setPalette(pal);
    b[i][j]->setText("");
}
void MainWindow::showAllWrong()
{
    if(!wrong1.empty())
    {
        for(const std::pair<int,int>&ss:wrong1)
        {
            showwrong1(ss.first,ss.second);
        }
    }
    if(!wrong2.empty())
    {
        for(const std::pair<int,int>&ss:wrong2)
        {
            showwrong2(ss.first,ss.second);
        }
    }
}

void MainWindow::recoverwrong()
{
    typedef std::set<std::pair<int,int> >::iterator it;
    for(it ss=wrong1.begin();ss!=wrong1.end(); )
    {
        if(!Game->haswrong((*ss).first,(*ss).second))
        {
            notshowwrong1((*ss).first,(*ss).second);
            wrong1.erase(ss++);
        }
        else
            ++ss;
    }
    for(it ss=wrong2.begin();ss!=wrong2.end(); )
    {
        if(!Game->haswrong2((*ss).first,(*ss).second))
        {
            notshowwrong2((*ss).first,(*ss).second);
            wrong2.erase(ss++);
        }
        else
            ++ss;
    }
}

void MainWindow::on_action_5_triggered()
{
    QMessageBox::information(this, tr("规则简介"), tr("玩家需要根据9×9盘面上的已知数字，推理出所有剩余空格的数字，"
                                                  "并满足每一行、每一列、每一个3×3宫内的数字均含1-9，不重复。"
                                                  "每一道数独谜题都有且仅有唯一答案。\n"
                                                   "9×9宫格分为如下9个3×3宫格:\n"
                                                   "3×3|3×3|3×3\n"
                                                   "3×3|3×3|3×3\n"
                                                   "3×3|3×3|3×3"));
}

void MainWindow::on_action_6_triggered()
{

    QMessageBox::information(this, tr("关于本软件"), tr("本软件完全免费！\n"
                                                  "可保证生成的每一个数独游戏都仅有唯一解!\n"
                                                   "如有其它想法或建议可联系本人，QQ:1043326391"));
}

void MainWindow::on_action_help_triggered()
{
    if(!begining)
        return;
    if(!helpTimes)
    {
        QMessageBox::information(this, tr("对不起"), tr("提示次数已用完O(∩_∩)O"));
        return;
    }
    QString a;
    a=tr("提示部分为红色显示\n本次提示之后还剩余%1次提示").arg(--helpTimes);
    QMessageBox::information(this, tr("请注意"), a);

    std::pair<int,int> helptemp=Game->help();
    int xi=helptemp.first,yi=helptemp.second;
    b[xi][yi]->setText(QString::number((*Game)[xi][yi]));
    wrong1.insert(helptemp);
    showwrong1(xi,yi);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if( e->key()==Qt::Key_H)
    {
        on_action_help_triggered();
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_1 )
    {
        save(0);
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_2 )
    {
        save(1);
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_3 )
    {
        save(2);
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_4 )
    {
        save(3);
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_5 )
    {
        save(4);
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_Q )
    {
        load(0);
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_W )
    {
        load(1);
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_E )
    {
        load(2);
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_R )
    {
        load(3);
    }
    else if( ( e->modifiers() & Qt::ControlModifier ) && e->key()==Qt::Key_T )
    {
        load(4);
    }
}

void MainWindow::on_show_shortcuts_triggered()
{
    QMessageBox::information(this, tr("快捷键"), tr("本软件支持以下快捷键：\n"
                                                  "键盘H：获得提示\n"
                                                  "Crtl+1至Crtl+5：分别对应存入位置1-5\n"
                                                  "Crtl+Q至Crtl+T：位于相应存档键的下方，分别读取存档1-5\n"));
}
