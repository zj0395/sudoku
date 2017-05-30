/*************************************************************************
	> File Name: sudokucrack.h
	> Author: zj
	> Mail: zhangjian0395@163.com
	> Created Time: Sat 27 May 2017 11:53:05 AM CST
 ************************************************************************/

#ifndef _SUDOKUCRACK_H
#define _SUDOKUCRACK_H

#include <vector>
#include <set>

typedef std::vector<int> vint;
typedef std::vector<vint> vvint;
typedef std::set<int> sint;
class SudokuCrack
{
public:
    SudokuCrack(vvint &);
    int beginCrack();//开始求解
private:
    bool checkwin();
    int selectBetter();//选择最优点
    int countLackNum(int i, int j);//统计x,y对应坐标的可能数目
    sint& getLackedNumSet( int i, int j );
    int attemptLocate( int i, int j);//对最优点进行猜


    vvint vvnum;//0是没有数字，1-9为正常数字
    static int times;
};

#endif
