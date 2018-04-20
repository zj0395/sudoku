/*************************************************************************
	> File Name: sudoku.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Mar 2017 09:25:41 PM CST
 ************************************************************************/

#include"sudoku.h"
#include "sudokucrack.h"
#include<random>
#include<ctime>
#include<set>
static std::default_random_engine e(time(0));
SudoKu::SudoKu(int a):hardclass(a),vvnum({
                {1,4,5,3,2,7,6,9,8},
                {8,3,9,6,5,4,1,2,7},
                {6,7,2,9,1,8,5,4,3},
                {4,9,6,1,8,5,3,7,2},
                {2,1,8,4,7,3,9,5,6},
                {7,5,3,2,9,6,4,8,1},
                {3,6,7,5,4,2,8,1,9},
                {9,8,4,7,6,1,2,3,5},
                {5,2,1,8,3,9,7,6,4}}){change_Number();}

void SudoKu::change_value(int lhs,int rhs)
{
    for(vint  & vec:vvnum)
        for(int &a : vec)
        {
            if(a==lhs)a=rhs;
            else a= a==rhs?lhs:a;
        }
}
void SudoKu::swap_row(int lhs,int rhs)
{
    using std::swap;
    swap(vvnum[lhs],vvnum[rhs]);
}
void SudoKu::swap_threerow(int lhs,int rhs)
{
    for(int i=0;i<3;++i)
        swap_row(lhs+i,rhs+i);
}
void SudoKu::swap_threecol(int lhs,int rhs)
{
    for(int i=0;i<3;++i)
        swap_col(lhs+i,rhs+i);
}
void SudoKu::swap_col(int lhs,int rhs)
{
    for(int i=0;i<9;++i)
    {
        using std::swap;
        swap(vvnum[i][lhs],vvnum[i][rhs]);
    }
}
void SudoKu::change_Number()
{
    std::uniform_int_distribution<int> u(1,9);
    //swap value
    for(int a=0;a<15;++a)
    {
        int i=u(e),j=u(e);
        if(i!=j)
            change_value(u(e),u(e));
        else --a;
    }
    std::uniform_int_distribution<int>ul(0,2);
    
    //swap row and line 
    for(int a=0;a<10;++a)
    {
        //开始玩魔方
        int bases=3*ul(e),i=bases+ul(e),j=bases+ul(e);
        if(i!=j)
            swap_row(i,j);// 0-8
        i=ul(e),j=ul(e);
        if(i!=j)
            swap_threerow(3*i,3*j);//0 3 6
        bases=3*ul(e),i=bases+ul(e),j=bases+ul(e);
        if(i!=j)
            swap_col(i,j);//0-8
        i=ul(e),j=ul(e);
        if(i!=j)
            swap_threecol(3*i,3*j);// 0 3 6
    }
    result=vvnum;
    while(1)
    {
        GameLevel lev(hardclass);
        for(int a=0;a<9;++a)
        {
            for(int b=0;b<9;++b)
                if(!lev[a][b])
                    vvnum[a][b]=0;
        }
        SudokuCrack sc(vvnum);
        if(sc.beginCrack())
        {
            vvnum=result;
        }
        else
            break;
    }
}

std::pair<int,int> SudoKu::help()
{
    //Help in row
    vint row;//row
    for(int i=0;i<9;++i)
    {
        int a=0;
        for(int j=0;j<9;++j)
        {
            if(vvnum[j][i])
                ++a;
        }
        if(a>5&&a<8)
        {
            row.push_back(i);
            row.push_back(a);
        }
    }
    if(!row.empty())
    {
        std::uniform_int_distribution<int> us(0,row.size()/2-1);
        int temp=us(e);
        const int j=row[temp*2];//row
        std::uniform_int_distribution<int> ud(1,9-row[temp*2+1]);
        int num=ud(e);
        for(int i=0;i<9;++i)
        {
            if(!vvnum[i][j])
             {
                if(num==1)
                {
                    vvnum[i][j]=result[i][j];
                    return std::make_pair(i,j);
                }
                else
                    --num;
            }
        }
    }


    //help in line
    vint line;//line help
    for(int i=0;i<9;++i)
    {
        int a=0;
        for(int j=0;j<9;++j)
        {
            if(vvnum[i][j])
                ++a;
        }
        if(a>4&&a<8)
        {
            line.push_back(i);
            line.push_back(a);
        }
    }
    if(!line.empty())
    {
        std::uniform_int_distribution<int> us(0,line.size()/2-1);
        int temp=us(e);
        const int i=line[temp*2];//line
        std::uniform_int_distribution<int> ud(1,9-line[temp*2+1]);
        int num=ud(e);
        for(int j=0;i<9;++j)
        {
            if(!vvnum[i][j])
             {
                if(num==1)
                {
                    vvnum[i][j]=result[i][j];
                    return std::make_pair(i,j);
                }
                else
                    --num;
            }
        }
    }

    //help Default:
    std::uniform_int_distribution<int> us(0,8);
    int i=us(e),j=us(e);
    for(;;)
    {
        if(!vvnum[i][j])
        {
            break;
        }
        else
        {
            i=us(e);
            j=us(e);
        }
    }
    vvnum[i][j]=result[i][j];
    return std::make_pair(i,j);
}

bool SudoKu::checkwin()
{
    const std::set<int>ss={1,2,3,4,5,6,7,8,9};
    for(int i=0;i<9;++i)
    {
        std::set<int>m;
        std::set<int>n;
        for(int j=0;j<9;++j)
        {
            m.insert(vvnum[i][j]);
            n.insert(vvnum[j][i]);
        }
        if(ss!=m||ss!=n)
            return 0;
    }
    for(int x=0;x<7;x+=3)
    {
        std::set<int>m;
        for(int i=0;i<3;++i)
        {
            for(int j=0;j<3;++j)
            {
                m.insert(vvnum[i+x][x+j]);
            }
        }
        if(ss!=m)
            return 0;
    }
    return 1;
}

bool SudoKu::haswrong(int a, int b)
{
    checktemp=vvnum[a][b];
    for(int i=0;i<9;i++)
    {
        if(i==b)
            continue;
        if(checktemp==vvnum[a][i])//checkline
        {
            return 1;
        }
    }
    for(int i=0;i<9;i++)
    {
        if(i==a)
            continue;
        if(checktemp==vvnum[i][b])//checkrow
        {
            return 1;
        }
    }
    for(int i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(int y=0,btemp=b/3*3;y<3;++y,++btemp)
        {
            if(atemp==a&&btemp==b)
                continue;
            if(checktemp==vvnum[atemp][btemp])
            {
                return 1;
            }
        }
    }
    return 0;
}

bool SudoKu::haswrong2(int a, int b)
{
    int m=onlyone(a,b);
    for(int i=0;i<9;++i)
    {
        if(i!=b&&(vvnum[a][i]==m))
            return 1;
    }
    for(int i=0;i<9;++i)
    {
        if(i!=b&&(vvnum[i][b]==m))
            return 1;
    }
    for(int i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(int y=0,btemp=b/3*3;y<3;++y,++btemp)
        {
            if(!(atemp==a&&btemp==b)  &&  vvnum[atemp][btemp]==m)
                return 1;
        }
    }
    return 0;
}

void SudoKu::check2(int a, int b, spairii &vecpr)
{
    checktemp=vvnum[a][b];
    for(int i=0;i<9;++i)
    {
        if(i==b||(vvnum[a][i]!=0))
            continue;
        if(checktemp==onlyone(a,i))//checkline
        {
            vecpr.insert(std::make_pair(a,i));
        }
    }
    for(int i=0;i<9;++i)
    {
        if(i==a||(vvnum[i][b]!=0))
            continue;
        if(checktemp==onlyone(i,b))//checkrow
        {
            vecpr.insert(std::make_pair(i,b));
        }
    }
    for(int i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(int y=0,btemp=b/3*3;y<3;++y,++btemp)
        {
            if(vvnum[atemp][btemp]!=0)
            {continue;}
            if(atemp==a&&btemp==b)
                continue;
            if(checktemp==onlyone(atemp,btemp))
            {
                vecpr.insert(std::make_pair(atemp,btemp));
            }
        }
    }
 }
int SudoKu::onlyone(int a, int b)
{
    std::set<size_t>s{1,2,3,4,5,6,7,8,9};
    for(size_t i=0;i<9;++i)
    {
        s.erase(vvnum[i][b]);
    }
    if(s.size()==1)
        return *s.begin();
    s={1,2,3,4,5,6,7,8,9};
    for(size_t i=0;i<9;++i)
    {
        s.erase(vvnum[a][i]);
    }
    if(s.size()==1)
        return *s.begin();
    s.insert({1,2,3,4,5,6,7,8,9});
    for(int i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(int y=0,btemp=b/3*3;y<3;++y,++btemp)
        {
            s.erase(vvnum[atemp][btemp]);
        }
    }
    if(s.size()==1)
        return *s.begin();
    return 10;
}

void SudoKu::check1(int a, int b, spairii &vecpr)
{
    checktemp=vvnum[a][b];
    for(int i=0;i<9;i++)
    {
        if(i==b)
            continue;
        if(checktemp==vvnum[a][i])//checkline
        {
            vecpr.insert(std::make_pair(a,i));
            vecpr.insert(std::make_pair(a,b));
        }
    }
    for(int i=0;i<9;i++)
    {
        if(i==a)
            continue;
        if(checktemp==vvnum[i][b])//checkrow
        {
            vecpr.insert(std::make_pair(i,b));
            vecpr.insert(std::make_pair(a,b));
        }
    }
    for(int i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(int y=0,btemp=b/3*3;y<3;++y,++btemp)
        {
            if(atemp==a&&btemp==b)
                continue;
            if(checktemp==vvnum[atemp][btemp])
            {
                vecpr.insert(std::make_pair(atemp,btemp));
                vecpr.insert(std::make_pair(a,b));
            }
        }
    }
}
vvint SudoKu::copy()
{
    return vvnum;
}
void SudoKu::set(const vvint &vec)
{
    vvnum=vec;
}


//******************************
/*void GameLevel::show()
{
    for(std::vector<bool> &vec:defaultshow)
    {    
        for(bool a:vec)
        {
            std::cout<<a<<" ";
        }
        std::cout<<"\n";
    }
}*/
void GameLevel::sayEasyWays()
{
    //36-39
    std::uniform_int_distribution<int> us(4,6);
    ways[0]=ways[3]=ways[6]=us(e);
    std::uniform_int_distribution<int> um(3,4);
    ways[1]=ways[7]=um(e);
    ways[2]=ways[5]=ways[8]=int((33-(3*ways[0]+2*ways[1]))/3);
    std::uniform_int_distribution<int> un(4,6);
    ways[4]=un(e);
}
void GameLevel::sayMediumWays()
{
    //30-36
    std::uniform_int_distribution<int> us(3,6);
    ways[0]=ways[3]=ways[6]=us(e);
    std::uniform_int_distribution<int> um(2,3);
    ways[1]=ways[7]=um(e);
    ways[2]=ways[5]=ways[8]=int((28-(3*ways[0]+2*ways[1]))/3);
    std::uniform_int_distribution<int> un(3,5);
    ways[4]=un(e);
}
void GameLevel::sayHardWays()
{
    //32-34
    std::uniform_int_distribution<int> us(3,5);
    ways[0]=ways[8]=us(e);
    ways[3]=ways[5]=8-ways[0];
    std::uniform_int_distribution<int> ud(2,3);
    ways[2]=ways[6]=ud(e);
    ways[1]=ways[7]=7-ways[2];
    std::uniform_int_distribution<int> ul(2,4);
    ways[4]=ul(e);
}
void GameLevel::sayHellWays()
{
    //29-32
    std::uniform_int_distribution<int> us(3,5);
    ways[0]=ways[8]=us(e);
    ways[1]=ways[7]=7-ways[0];
    std::uniform_int_distribution<int> ud(2,3);
    ways[2]=ways[6]=ud(e);
    ways[3]=ways[5]=7-ways[2];
    std::uniform_int_distribution<int> ul(1,4);
    ways[4]=ul(e);
    //33-37
//    std::uniform_int_distribution<int> us(0,3);//3-5 -> 0-1
//    ways[0]=us(e);
//    ways[1]=3-ways[0];//-3
//    std::uniform_int_distribution<int> ud(1,2);//2-3 -> 0-1
//    ways[3]=ud(e);
//    ways[2]=3-ways[3]+ud(e);//-5
//    std::uniform_int_distribution<int> ul(1,3);//1-3 ->2-4
//    ways[4]=ul(e);
//    std::set<int> rows={0,1,2,3,4,5,6,7,8};
//    std::set<int> colmuns={0,1,2,3,4,5,6,7,8};
//    std::uniform_int_distribution<int> um(0,2);
//    for( int m=0 ; m < 7 ; m+=3)
//    {
//        for( int n=0 ; n<7 ; n+=3)
//        {
//            while (1) {
//                int i=m+um(e),j=n+um(e);
//                if( rows.find( i )!=rows.end() && colmuns.find(j)!=colmuns.end())
//                {
//                    qDebug()<<i<<j;
//                    defaultshow[i][j]=1;
//                    rows.erase(i);
//                    colmuns.erase(j);
//                    if(i > 4)
//                        defaultshow[8-i][j]=1;//9ge
//                    break;
//                }
//            }
//        }
//    }
}

void GameLevel::setlevel()
{
    if(0==level)
        sayEasyWays();
    else if(1==level)
        sayMediumWays();
    else if(2==level)
        sayHardWays();
    else if(3==level)
        sayHellWays();
    else
        sayEasyWays();
}
void GameLevel::generate_map()
{
    if(level<2)
        for(int a=0;a<9;++a)
            changeRow(defaultshow[a],ways[a]);
    else
    {
        for(int a=0;a<5;++a)
        {
            changeRow(defaultshow[a],ways[a]);
        }
        for(int a=5;a<9;++a)
            defaultshow[a]=defaultshow[8-a];
    }
}
void GameLevel::changeRow(std::vector<bool>&a,int num)
{
    int s=0;
    if(num==0)
        return;
    std::uniform_int_distribution<int> un(1,9-s);
    int n=un(e);
    int temp=0;
    std::vector<bool>::iterator i=a.begin();
    do{
        if(0==*i)
        temp++;
        if(temp==n)
        {
            *i=1;
            if(++s==num)break;
            i=a.begin();
            std::uniform_int_distribution<int>ud(1,9-s);
            n=ud(e);temp=0;
        }
        else
        {
            if(i==a.end())
                i=a.begin();
            else
                ++i;
        }
    }while(1);
}
