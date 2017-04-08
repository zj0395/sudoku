/*************************************************************************
	> File Name: sudoku.h
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Mar 2017 09:25:19 PM CST
 ************************************************************************/

#ifndef _SUDOKU_H
#define _SUDOKU_H

#include<vector>
#include<set>
#include<cstdlib>
class Numbertable
{
private:
    const std::size_t hardclass;
    std::size_t checktemp;//临时变量，不再一直调用vvnum[][]
    std::vector<std::vector<std::size_t> > vvnum;
    std::vector<std::vector<std::size_t> > result;

    void change_Number();//初始化9×9格子
    //5个初始化算法
    void change_value(std::size_t lhs,std::size_t rhs);
    void swap_line(std::size_t lhs,std::size_t rhs);
    void swap_row(std::size_t lhs,std::size_t rhs);
    void swap_threeline(std::size_t lhs,std::size_t rhs);
    void swap_threerow(std::size_t lhs,std::size_t rhs);

    std::size_t onlyone(std::size_t a,std::size_t b);
public:
    ~Numbertable(){}
    explicit Numbertable(std::size_t a):hardclass(a),vvnum({
                    {1,2,3,4,5,6,7,8,9},
                    {4,5,6,7,8,9,1,2,3},
                    {7,8,9,1,2,3,4,5,6},
                    {2,3,4,5,6,7,8,9,1},
                    {5,6,7,8,9,1,2,3,4},
                    {8,9,1,2,3,4,5,6,7},
                    {3,4,5,6,7,8,9,1,2},
                    {6,7,8,9,1,2,3,4,5},
                    {9,1,2,3,4,5,6,7,8}}){change_Number();}

    const std::vector<std::size_t>& operator[](std::size_t a)const{return vvnum[a];}

    std::vector<std::size_t>& operator[](std::size_t a){return vvnum[a];}
    std::vector<std::vector<std::size_t> > copy();
    void set(const std::vector<std::vector<std::size_t> >& vec);


    const std::vector<std::vector<std::size_t> >& getresult(){return result;}
    std::pair<std::size_t,std::size_t> help();

    //same num check
    void check1(std::size_t a, std::size_t b, std::set<std::pair<std::size_t, std::size_t> > &vecpr);

    //only number check
    void check2(std::size_t a, std::size_t b, std::set<std::pair<std::size_t, std::size_t> > &vecpr);

    bool haswrong(std::size_t a, std::size_t b);
    bool haswrong2(std::size_t a, std::size_t b);
    bool checkwin();
    //void show();
};

//Assist class
class GameLevel
{
private:
    std::size_t intlevel;
    std::vector<std::vector<bool> >defaultshow;//9×9的显示方式
    std::vector<std::size_t>ways;//显示方式,每行显示出来几个

    void level();//所有设置，调用下两个函数

    void sayeasyways();//设置成员ways
    void saymediumways();
    void sayhardways();
    void sayhellways();

    void generate_map();//设置defaultshow，调用下一函数

    void changealine(std::vector<bool>&,std::size_t);

public:
    const std::vector<bool>& operator[](std::size_t a)const{return defaultshow[a];}

    explicit GameLevel(std::size_t a):intlevel(a),
        defaultshow(9,std::vector<bool>(9,0)),ways(9,0)
        {level();}
    
    //void show();
};

#endif
