#include "mainwindow.h"
//#include<QGridLayout>
#include<QMessageBox>
#include "ui_mainwindow.h"
#include <QString>
#include<QThread>
#include "QDebug"
const static std::size_t savednumber=3;
const static std::size_t xx=80,yy=30,mwidth=50,mhigh=50;//location of mainwindow number table
const static std::size_t kwidth=mwidth*3/5,khigh=mhigh*3/5;//location of keyboard

const static std::size_t xbutton=xx+mwidth/2,ybutton=yy+mhigh*19/2;//three button location
const static std::size_t xsaveloc=xbutton+3*mwidth,ysaveloc=ybutton;//help location for save and load

static std::size_t xi=0,ji=0;//temp of i and j
static bool saveload=0;//0==save,1==load

static bool initialization=0;
static std::size_t payclicknum=200;
static std::size_t helpnumber=0;

static QPushButton *showorhide=nullptr;//for same box clicked
static QPushButton *newgame,*loadgame,*savenow;
static QPushButton *slbutton[3];
static QPushButton *hardclass[5];//hardclass[4] is cancel
static QPushButton *pay;

static std::size_t level=0;

/*static void pause()
{
    QThread::msleep(5000);
}*/

static void showbutton()
{
    newgame->show();
    loadgame->show();
    savenow->show();
    pay->show();
}
static void saveshow()
{
    for(std::size_t i=0;i<3;++i)
    {
        slbutton[i]->setGeometry(xsaveloc,ysaveloc-(i+1)*mhigh,2*mwidth,mhigh);
        slbutton[i]->show();
    }
}
static void loadshow()
{
    for(std::size_t i=0;i<3;++i)
    {
        slbutton[i]->setGeometry(xsaveloc+3*mwidth,ysaveloc-(i+1)*mhigh,2*mwidth,mhigh);
        slbutton[i]->show();
    }
}
static void hideslkeyboard()
{
    for(std::size_t i=0;i<3;++i)
    {
        slbutton[i]->hide();
    }
}
static void showhardclass(bool a)
{
    if(a)
        for(std::size_t i=0;i<5;++i)
            hardclass[i]->show();
    else
        for(std::size_t i=0;i<5;++i)
            hardclass[i]->hide();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),pb(),tab()
{
        ui->setupUi(this);
        setWindowTitle("数独4.1");
        setWindowIcon(QIcon("icon.ico"));
        ui->textEdit->hide();
        ui->textEdit->setReadOnly(true);
        resize(600,650);
        QString a,b;
        QFont font;
        font.setPointSize(15);
        font.setFamily("宋体");
        for(std::size_t i=0;i<9;++i)
        {
            for(std::size_t j=0;j<9;++j)
            {
                pb[i][j]=new QPushButton("",this);
                connect(pb[i][j],SIGNAL(clicked()),this,SLOT(arrayclicked()));
                std::size_t xtemp=0,ytemp=0;
                if(i>5)
                    ytemp=2;
                else if(i>2)
                    ytemp=1;
                else ytemp=0;
                if(j>5)
                    xtemp=2;
                else if(j>2)
                    xtemp=1;
                else xtemp=0;
                pb[i][j]->setGeometry(xx+mwidth*j+xtemp,yy+mhigh*i+ytemp,mwidth,mhigh);
                pb[i][j]->setAccessibleName(a.setNum(i)+b.setNum(j));
                pb[i][j]->hide();
                pb[i][j]->setFont(font);
            }
        }
        font.setPointSize(10);

        newgame=new QPushButton("新游戏",this);
        connect(newgame,SIGNAL(clicked()),this,SLOT(newgameclicked()));
        newgame->setGeometry(xbutton,ybutton,mwidth*2,mhigh);
        newgame->hide();
        newgame->setFont(font);

        savenow=new QPushButton("存档",this);
        connect(savenow,SIGNAL(clicked()),this,SLOT(savenowclicked()));
        savenow->setGeometry(xsaveloc,ysaveloc,mwidth*2,mhigh);
        savenow->hide();
        savenow->setFont(font);

        loadgame=new QPushButton(tr("读档"),this);
        connect(loadgame,SIGNAL(clicked()),this,SLOT(loadgameclicked()));
        loadgame->setGeometry(xsaveloc+3*mwidth,ysaveloc,mwidth*2,mhigh);
        loadgame->hide();
        loadgame->setFont(font);

        pay=new QPushButton("充值解锁提示",this);
        connect(pay,SIGNAL(clicked()),this,SLOT(payclicked()));
        pay->setGeometry(xsaveloc-mwidth/2,ysaveloc+mwidth*2/2,mwidth*3,mhigh);
        pay->hide();
        pay->setFont(font);


        for(std::size_t i=0;i<9;++i)
        {
            keyboard[i]=new QPushButton(a.setNum(i+1),this);
            connect(keyboard[i],SIGNAL(clicked()),this,SLOT(keyboardclicked()));
        }
        keyboard[9]=new QPushButton((tr("clear")),this);
        connect(keyboard[9],SIGNAL(clicked()),this,SLOT(keyboardclicked()));
        hidekey();


        for(std::size_t i=0;i<savednumber;++i)
        {
            slbutton[i]=new QPushButton(("存档"+a.setNum(i+1)),this);
            slbutton[i]->hide();
            connect(slbutton[i],SIGNAL(clicked()),this,SLOT(slclicked()));
            slbutton[i]->setAccessibleName(a);
        }

        QString strs[5]={"简单","中等","困难","炼狱","取消"};
        for(std::size_t i=0;i<5;++i)
        {
            hardclass[i]=new QPushButton(strs[i],this);
            connect(hardclass[i],SIGNAL(clicked()),this,SLOT(hardclassclicked()));
            hardclass[i]->setAccessibleName(a.setNum(i));
            hardclass[i]->hide();
            hardclass[i]->setGeometry(mwidth*9/2,yy+(i+2)*mwidth,mwidth*3,mhigh);
        }
}

void MainWindow::hidekey()
{
    for(std::size_t i=0;i<10;++i)
    {
        keyboard[i]->hide();
    }
}
void MainWindow::newtable()
{
    clickable=std::vector<std::vector<bool> >(9,std::vector<bool>(9,1));
    for(std::size_t i=0;i<savednumber;++i)
        saved[i].clear();
    //QButtonGroup *numgroup=new QButtonGroup;
    QString a;
    QString b;
    for(std::size_t i=0;i<9;++i)
    {
        for(std::size_t j=0;j<9;++j)
        {
            pb[i][j]->setStyleSheet("background-color: #ffffff;");
            if(tab[i][j]!=0)
            {
                pb[i][j]->setText(a.setNum(tab[i][j]));
                pb[i][j]->setStyleSheet("background-color: #A2B5CD;");
                clickable[i][j]=0;
            }
            else    pb[i][j]->setText("");
            //numgroup->addButton(pb[i][j]);
            pb[i][j]->show();
        }
    }
}

void MainWindow::setkeyloc(std::size_t i, std::size_t j)
{
    std::size_t a=xx+j*mwidth,b=yy+i*mhigh;
    a-=(kwidth*3-mwidth)/2;b+=mhigh;
    for(std::size_t i=0;i<9;++i)
    {
        std::size_t y=i/3,x=i%3;
        keyboard[i]->setGeometry(x*kwidth+a,y*khigh+b,kwidth,khigh);
        keyboard[i]->show();
    }
    keyboard[9]->setGeometry(a,b+khigh*3,kwidth*3,khigh);
    keyboard[9]->show();
    xi=i;ji=j;
}


void MainWindow::paintEvent(QPaintEvent *)
{
    if(initialization)
    {
        QPen pen(Qt::red,5);
        QPainter painter(this);
        painter.setPen(pen);
        QLine lines[4];
        lines[0].setLine(xx+1,yy+3*mhigh+1,xx+9*mwidth-2,yy+3*mhigh+1);//up
        lines[1].setLine(xx+1,yy+6*mhigh+2,xx+9*mwidth-2,yy+6*mhigh+2);//down
        lines[2].setLine(xx+mwidth*3+1,yy+2,xx+mwidth*3+2,yy+9*mhigh-2);//left
        lines[3].setLine(xx+6*mwidth+2,yy+2,xx+6*mwidth+2,yy+9*mhigh-2);//right
        painter.drawLines(lines,4);
    }
}


void MainWindow::arrayclicked()
{
    hideslkeyboard();
    showhardclass(0);
    QPushButton * digitalbutton = (QPushButton *) sender();
    if(showorhide==digitalbutton)
    {
        hidekey();
        showorhide=nullptr;
    }
    else
    {
        showorhide=digitalbutton;
        QString str=digitalbutton->accessibleName();
        std::size_t num=str.toInt();
        std::size_t i=num/10,j=num%10;
        if(clickable[i][j])
            setkeyloc(i,j);
        else    hidekey();
    }
}
void MainWindow::keyboardclicked()
{
    showorhide=nullptr;
    QPushButton *butt=(QPushButton*)sender();
    QString str=butt->text();
    std::size_t num=str.toInt();
    if(num)
    {
        pb[xi][ji]->setText(str);
        tab[xi][ji]=num;
    }
    else
    {
        pb[xi][ji]->setText("");
        tab[xi][ji]=0;
    }
    recoverwrong();
    tab.check1(xi,ji,wrong1);
    tab.check2(xi,ji,wrong2);
    if(!wrong1.empty())
    {
        for(const std::pair<size_t,size_t>&ss:wrong1)
        {
            showwrong(ss.first,ss.second);
        }
    }
    if(!wrong2.empty())
    {
        for(const std::pair<size_t,size_t>&ss:wrong2)
        {
            showwrong2(ss.first,ss.second);
        }
    }
    if(checkwin())
    {
        if(level==0)
            QMessageBox::information(this, tr("胜利了"), tr("简单难度，一般一般吧\n"
                                                         "敢不敢挑战个高点的难度？(#‵′)凸"));
        else if(level==1)
            QMessageBox::information(this, tr("厉害了"), tr("厉害了，通关中等难度"
                                                         "你的智商已经超过百分之好多人了"));
        else if(level==2)
            QMessageBox::information(this, tr("大师！！"), tr("大师！！大师！！\n"
                                                          "智商这么高我们做朋友吧\n"
                                                          "QQ:1043326391"));
        else if(level==3)
            QMessageBox::information(this, tr("大神！！！"), tr("大神收我一拜\n"
                                                           "在你这样的智商面前我已经没话可说了\n"
                                                           "大神快带我上天！\n"
                                                           "QQ:1043326391"));
    }
}
bool MainWindow::checkwin()
{
    if(wrong1.empty()&&wrong2.empty()&&tab.checkwin())
        return 1;
    return 0;
}

void MainWindow::recoverwrong()
{
    hidekey();
    for(std::set<std::pair<std::size_t,std::size_t> >::iterator ss=wrong1.begin();ss!=wrong1.end(); )
    {
        if(!tab.haswrong((*ss).first,(*ss).second))
        {
            notshowwrong((*ss).first,(*ss).second);
            wrong1.erase(ss++);
        }
        else
            ++ss;
    }
    for(std::set<std::pair<std::size_t,std::size_t> >::iterator ss=wrong2.begin();ss!=wrong2.end(); )
    {
        if(!tab.haswrong2((*ss).first,(*ss).second))
        {
            notshowwrong2((*ss).first,(*ss).second);
            wrong2.erase(ss++);
        }
        else
            ++ss;
    }
}

void MainWindow::showwrong(size_t i, size_t j)
{
    QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor("#ff0000"));
    pb[i][j]->setPalette(pal);
    //pb[i][j]->setStyleSheet("QPushButton{color:red}");
    //pb[i][j]->setStyleSheet("background-color: rgb(170, 0, 255);");//background-color
}
void MainWindow::notshowwrong(size_t i, size_t j)
{
    QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor("#000000"));
    pb[i][j]->setPalette(pal);
    //pb[i][j]->setStyleSheet("QPushButton{color:black}");
}

void MainWindow::showwrong2(size_t i, size_t j)
{
    QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor("#ff0000"));
    pb[i][j]->setPalette(pal);
    pb[i][j]->setText("X");
}
void MainWindow::notshowwrong2(size_t i, size_t j)
{
    QPalette pal;
    pal.setColor(QPalette::ButtonText,QColor("#000000"));
    pb[i][j]->setPalette(pal);
    pb[i][j]->setText("");
}


void MainWindow::newgameclicked()
{
    hideslkeyboard();
    hidekey();
    if(showorhide==newgame)
    {
        showhardclass(0);
        showorhide=nullptr;
    }
    else
    {
        showhardclass(1);
        showorhide=newgame;
    }
}
void MainWindow::savenowclicked()
{
    hidekey();
    showhardclass(0);
    saveload=0;
    if(showorhide==savenow)
    {
        hideslkeyboard();
        showorhide=nullptr;
    }
    else
    {
        saveshow();
        showorhide=savenow;
    }
}
void MainWindow::loadgameclicked()
{
    hidekey();
    showhardclass(0);
    saveload=1;
    if(showorhide==loadgame)
    {
        hideslkeyboard();
        showorhide=nullptr;
    }
    else
    {
        loadshow();
        showorhide=loadgame;
    }
}
void MainWindow::payclicked()
{
    if(helpnumber)
    {
        QString a;
        if(payclicknum==200)
        {
            a.setNum(helpnumber);
            a="怎么能充值呢\n这又不是腾讯游戏╮(╯_╰)╭\n"
              "想要提示的话，就使劲点击这个按钮吧\n"
              "能提示的次数与难度有关哦\n"
              "当前难度能提示"+a+"次";
            payclicknum=20;
            QMessageBox::information(this, tr("鄙视你"), a);
        }
        if(!payclicknum)
        {
            a.setNum(--helpnumber);
            a="提示部分为红色显示\n本次提示之后还剩余"+a+"次提示";
            QMessageBox::information(this, tr("请注意"), a);

            std::pair<std::size_t,std::size_t>helptemp=tab.help();
            std::size_t xi=helptemp.first,yi=helptemp.second;
            pb[xi][yi]->setText(a.setNum(tab[xi][yi]));
            showwrong(xi,yi);
            wrong1.insert(helptemp);

            repaint();

            //pause();

            payclicknum=20+(4-helpnumber)*5;
        }

            a.setNum(payclicknum);
            a="点击"+a+"次查看提示";
            pay->setText(a);
            --payclicknum;
            if(helpnumber==0)
                pay->setText("不给提示喽");
    }
    else
        pay->hide();
}

void MainWindow::slclicked()
{
    hideslkeyboard();
    showorhide=nullptr;
    QPushButton *but=(QPushButton *)sender();
    QString str=but->accessibleName();
    int n=str.toInt()-1;
    if(saveload)//load
    {
        if(!saved[n].empty())
        {
            changebuttontext(saved[n]);
            tab.set(saved[n]);
        }
        else
        {
            QMessageBox::information(this, tr("空存档"), tr("大哥/大姐，该存档为空(￣_,￣ )"));
        }
    }
    else//save
        saved[n]=tab.copy();
}

void MainWindow::changebuttontext(const std::vector<std::vector<std::size_t> >& vec)
{
    QString a;
    for(std::size_t i=0;i<9;++i)
        for(std::size_t j=0;j<9;++j)
            if(vec[i][j]!=0)
                pb[i][j]->setText(a.setNum(vec[i][j]));
            else
                pb[i][j]->setText("");
}

MainWindow::~MainWindow()
{
    for(std::size_t i=0;i<9;++i)
        for(std::size_t j=0;j<9;++j)
            delete pb[i][j];
    delete ui;
}



void MainWindow::on_begingame_clicked()
{
    showhardclass(1);
}
void MainWindow::hardclassclicked()
{
    showorhide=nullptr;
    QPushButton *but=(QPushButton*)sender();
    level=but->accessibleName().toInt();
    helpnumber=level+1;
    if(level!=4)
    {
        ui->begingame->hide();
        ui->showrules->hide();
        ui->history->hide();
        hidekey();
        hideslkeyboard();
        Table temp(level);
        tab=temp;
        newtable();
        showbutton();
        showhardclass(0);
        initialization=1;
        payclicknum=200;
        pay->setText("充值解锁提示");
        update();
    }
    else
        showhardclass(0);

}

void MainWindow::on_showrules_clicked()
{
    QMessageBox::information(this, tr("规则简介"), tr("玩家需要根据9×9盘面上的已知数字，推理出所有剩余空格的数字，"
                                                  "并满足每一行、每一列、每一个3×3宫内的数字均含1-9，不重复。"
                                                  "每一道数独谜题都有且仅有唯一答案。\n"
                                                   "9×9宫格分为如下9个3×3宫格:\n"
                                                   "3×3|3×3|3×3\n"
                                                   "3×3|3×3|3×3\n"
                                                   "3×3|3×3|3×3"));
}

void MainWindow::on_history_clicked()
{
    static bool canclicked=true;
    if(canclicked)
    {
        ui->textEdit->show();
        canclicked=0;
    }
    else
    {
        ui->textEdit->hide();
        canclicked=1;
    }
}
