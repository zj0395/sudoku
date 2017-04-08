# sudoku
A game in QT
table.h 和 table.cpp 是中间类  
数独中有两种错误，一种为已有重复数字，此为wrong1，另一种为与该行、列或3×3宫格内的必须为某个数字的有冲突，此为wrong2  
sudoku.h 和 sudoku.cpp 是核心类，构造函数只有一个接受一个难度（size_t）参数的，接口有检查错误（haswrong和haswrong2），返回错误（check1和check2），存储当前数组（copy），读取已存数组（set），检查胜利（checkwin），获得提示（help），
