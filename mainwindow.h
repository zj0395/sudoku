#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <set>
#include <QPainter>
#include <QPen>
#include <table.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    void paintEvent(QPaintEvent*);

public slots:




private slots:

    void slclicked();
    void newgameclicked();
    void savenowclicked();
    void loadgameclicked();
    void arrayclicked();
    void keyboardclicked();
    void on_begingame_clicked();

    void payclicked();
    void on_showrules_clicked();

    void hardclassclicked();

    void on_history_clicked();

private:

    bool checkwin();
    void changebuttontext(const std::vector<std::vector<std::size_t> >& vec);
    void newtable();
    void setkeyloc(std::size_t i, std::size_t j);
    void hidekey();
    void recoverwrong();

    //way for same number
    void showwrong(size_t i,size_t j);
    void notshowwrong(size_t i,size_t j);

    //another way for only number
    void showwrong2(size_t i,size_t j);
    void notshowwrong2(size_t i, size_t j);

    Ui::MainWindow *ui;
    QPushButton* pb[9][9];
    QPushButton* keyboard[10];
    Table tab;
    std::vector<std::vector<bool> > clickable;

    std::vector<std::vector<std::size_t> >saved[3];
    std::set<std::pair<std::size_t,std::size_t> > wrong1;
    std::set<std::pair<std::size_t,std::size_t> > wrong2;
   // QPushButton *pb[3];
};

#endif // MAINWINDOW_H
