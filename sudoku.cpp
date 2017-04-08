/*************************************************************************
	> File Name: sudoku.cpp
	> Author: 
	> Mail: 
	> Created Time: Tue 28 Mar 2017 09:25:41 PM CST
 ************************************************************************/
#include"sudoku.h"
#include<random>
#include<ctime>
#include<set>
static std::default_random_engine e(time(0));
void Numbertable::change_value(std::size_t lhs,std::size_t rhs)
{
    for(std::vector<std::size_t> & vec:vvnum)
        for(std::size_t &a : vec)
        {
            if(a==lhs)a=rhs;
            else a= a==rhs?lhs:a;
        }
}
void Numbertable::swap_line(std::size_t lhs,std::size_t rhs)
{
    using std::swap;
    swap(vvnum[lhs],vvnum[rhs]);
}
void Numbertable::swap_threeline(std::size_t lhs,std::size_t rhs)
{
    for(std::size_t i=0;i<3;++i)
        swap_line(lhs+i,rhs+i);
}
void Numbertable::swap_threerow(std::size_t lhs,std::size_t rhs)
{
    for(std::size_t i=0;i<3;++i)
        swap_row(lhs+i,rhs+i);
}
void Numbertable::swap_row(std::size_t lhs,std::size_t rhs)
{
    for(std::size_t i=0;i<9;++i)
    {
        using std::swap;
        swap(vvnum[i][lhs],vvnum[i][rhs]);
    }
}
void Numbertable::change_Number()
{
    std::uniform_int_distribution<std::size_t> u(1,9);
    //swap value
    for(std::size_t a=0;a<15;++a)
    {
        std::size_t i=u(e),j=u(e);
        if(i!=j)
            change_value(u(e),u(e));
        else --a;
    }
    std::uniform_int_distribution<std::size_t>ul(0,2);
    
    //swap row and line 
    for(std::size_t a=0;a<10;++a)
    {
        //开始玩魔方
        std::size_t bases=3*ul(e),i=bases+ul(e),j=bases+ul(e);
        if(i!=j)
            swap_line(i,j);// 0-8
        i=ul(e),j=ul(e);
        if(i!=j)
            swap_threerow(3*i,3*j);//0 3 6
        bases=3*ul(e),i=bases+ul(e),j=bases+ul(e);
        if(i!=j)
            swap_row(i,j);//0-8
        i=ul(e),j=ul(e);
        if(i!=j)
            swap_threeline(3*i,3*j);// 0 3 6
    }
    result=vvnum;
    GameLevel lev(hardclass);
    for(std::size_t a=0;a<9;++a)
    {
        for(std::size_t b=0;b<9;++b)
            if(!lev[a][b])
                vvnum[a][b]=0;
    }
}

std::pair<std::size_t,std::size_t> Numbertable::help()
{
    //Help in row
    std::vector<std::size_t>row;//row
    for(std::size_t i=0;i<9;++i)
    {
        std::size_t a=0;
        for(std::size_t j=0;j<9;++j)
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
        std::uniform_int_distribution<std::size_t> us(0,row.size()/2-1);
        std::size_t temp=us(e);
        const std::size_t j=row[temp*2];//row
        std::uniform_int_distribution<std::size_t> ud(1,9-row[temp*2+1]);
        std::size_t num=ud(e);
        for(std::size_t i=0;i<9;++i)
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
    std::vector<std::size_t>line;//line help
    for(std::size_t i=0;i<9;++i)
    {
        std::size_t a=0;
        for(std::size_t j=0;j<9;++j)
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
        std::uniform_int_distribution<std::size_t> us(0,line.size()/2-1);
        std::size_t temp=us(e);
        const std::size_t i=line[temp*2];//line
        std::uniform_int_distribution<std::size_t> ud(1,9-line[temp*2+1]);
        std::size_t num=ud(e);
        for(std::size_t j=0;i<9;++j)
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
    std::uniform_int_distribution<std::size_t> us(0,8);
    std::size_t i=us(e),j=us(e);
    for(;;)
    {
        if(!vvnum[i][j])
        {
            vvnum[i][j]=result[i][j];
            break;
        }
        else
        {
            i=us(e);
            j=us(e);
        }
    }
    return std::make_pair(i,j);
}

/*void Numbertable::show()
{
    for(std::vector<std::size_t> &vec:vvnum)
    {    
        for(std::size_t&a:vec)
        {
            std::cout<<a<<" ";
        }
        std::cout<<"\n";
    }
}*/

bool Numbertable::checkwin()
{
    const std::set<std::size_t>ss={1,2,3,4,5,6,7,8,9};
    for(std::size_t i=0;i<9;++i)
    {
        std::set<std::size_t>m;
        std::set<std::size_t>n;
        for(std::size_t j=0;j<9;++j)
        {
            m.insert(vvnum[i][j]);
            n.insert(vvnum[j][i]);
        }
        if(ss!=m||ss!=n)
            return 0;
    }
    for(std::size_t x=0;x<7;x+=3)
    {
        std::set<std::size_t>m;
        for(std::size_t i=0;i<3;++i)
        {
            for(std::size_t j=0;j<3;++j)
            {
                m.insert(vvnum[i+x][x+j]);
            }
        }
        if(ss!=m)
            return 0;
    }
    return 1;
}

bool Numbertable::haswrong(std::size_t a, std::size_t b)
{
    checktemp=vvnum[a][b];
    for(std::size_t i=0;i<9;i++)
    {
        if(i==b)
            continue;
        if(checktemp==vvnum[a][i])//checkline
        {
            return 1;
        }
    }
    for(std::size_t i=0;i<9;i++)
    {
        if(i==a)
            continue;
        if(checktemp==vvnum[i][b])//checkrow
        {
            return 1;
        }
    }
    for(std::size_t i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(std::size_t y=0,btemp=b/3*3;y<3;++y,++btemp)
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

bool Numbertable::haswrong2(std::size_t a, std::size_t b)
{
    checktemp=vvnum[a][b];
    for(std::size_t i=0;i<9;++i)
    {
        if(i==b||(vvnum[a][i]!=0))
            continue;
        if(checktemp==onlyone(a,i))//checkline
        {
            return 1;
        }
    }
    for(std::size_t i=0;i<9;++i)
    {
        if(i==b||(vvnum[i][b]!=0))
            continue;
        if(checktemp==onlyone(i,b))//checkrow
        {
            return 1;
        }
    }
    for(std::size_t i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(std::size_t y=0,btemp=b/3*3;y<3;++y,++btemp)
        {
            if((atemp==a&&btemp==b)||vvnum[atemp][btemp]!=0)
                continue;
            if(checktemp==onlyone(atemp,btemp))
            {
                return 1;
            }
        }
    }
    return 0;
}

void Numbertable::check2(std::size_t a, std::size_t b, std::set<std::pair<std::size_t, std::size_t> > &vecpr)
{
    checktemp=vvnum[a][b];
    for(std::size_t i=0;i<9;++i)
    {
        if(i==b||(vvnum[a][i]!=0))
            continue;
        if(checktemp==onlyone(a,i))//checkline
        {
            vecpr.insert(std::make_pair(a,i));
        }
    }
    for(std::size_t i=0;i<9;++i)
    {
        if(i==a||(vvnum[i][b]!=0))
            continue;
        if(checktemp==onlyone(i,b))//checkrow
        {
            vecpr.insert(std::make_pair(i,b));
        }
    }
    for(std::size_t i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(std::size_t y=0,btemp=b/3*3;y<3;++y,++btemp)
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
std::size_t Numbertable::onlyone(std::size_t a, std::size_t b)
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
    for(std::size_t i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(std::size_t y=0,btemp=b/3*3;y<3;++y,++btemp)
        {
            s.erase(vvnum[atemp][btemp]);
        }
    }
    if(s.size()==1)
        return *s.begin();
    return 10;
}

void Numbertable::check1(std::size_t a,std::size_t b,std::set<std::pair<std::size_t,std::size_t> >&vecpr)
{
    checktemp=vvnum[a][b];
    for(std::size_t i=0;i<9;i++)
    {
        if(i==b)
            continue;
        if(checktemp==vvnum[a][i])//checkline
        {
            vecpr.insert(std::make_pair(a,i));
            vecpr.insert(std::make_pair(a,b));
        }
    }
    for(std::size_t i=0;i<9;i++)
    {
        if(i==a)
            continue;
        if(checktemp==vvnum[i][b])//checkrow
        {
            vecpr.insert(std::make_pair(i,b));
            vecpr.insert(std::make_pair(a,b));
        }
    }
    for(std::size_t i=0,atemp=a/3*3;i<3;++i,++atemp)//checkbox
    {
        for(std::size_t y=0,btemp=b/3*3;y<3;++y,++btemp)
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
std::vector<std::vector<std::size_t> > Numbertable::copy()
{
    auto temp=vvnum;
    return temp;
}
void Numbertable::set(const std::vector<std::vector<std::size_t> > &vec)
{
    for(std::size_t i=0;i<9;++i)
        for(std::size_t j=0;j<9;++j)
            vvnum[i][j]=vec[i][j];
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
void GameLevel::sayeasyways()
{
    //36-39
    std::uniform_int_distribution<std::size_t> us(4,6);
    ways[0]=ways[3]=ways[6]=us(e);
    std::uniform_int_distribution<std::size_t> um(3,4);
    ways[1]=ways[7]=um(e);
    ways[2]=ways[5]=ways[8]=std::size_t((33-(3*ways[0]+2*ways[1]))/3);
    std::uniform_int_distribution<std::size_t> un(4,6);
    ways[4]=un(e);
}
void GameLevel::saymediumways()
{
    //30-36
    std::uniform_int_distribution<std::size_t> us(3,6);
    ways[0]=ways[3]=ways[6]=us(e);
    std::uniform_int_distribution<std::size_t> um(2,3);
    ways[1]=ways[7]=um(e);
    ways[2]=ways[5]=ways[8]=std::size_t((28-(3*ways[0]+2*ways[1]))/3);
    std::uniform_int_distribution<std::size_t> un(3,5);
    ways[4]=un(e);
}
void GameLevel::sayhardways()
{
    //29-31
    std::uniform_int_distribution<std::size_t> us(3,5);
    ways[0]=ways[8]=us(e);
    ways[1]=ways[7]=7-ways[0];
    std::uniform_int_distribution<std::size_t> ud(2,3);
    ways[2]=ways[6]=ud(e);
    ways[3]=ways[5]=7-ways[2];
    std::uniform_int_distribution<std::size_t> ul(1,3);
    ways[4]=ul(e);
}
void GameLevel::sayhellways()
{
    //27-29
    std::uniform_int_distribution<std::size_t> us(3,5);
    ways[0]=ways[8]=us(e);
    ways[1]=ways[7]=6-ways[0];
    std::uniform_int_distribution<std::size_t> ud(2,3);
    ways[2]=ways[6]=ud(e);
    ways[3]=ways[5]=7-ways[2];
    std::uniform_int_distribution<std::size_t> ul(1,3);
    ways[4]=ul(e);
}

void GameLevel::level()
{
    if(0==intlevel)
        sayeasyways();
    else if(1==intlevel)
        saymediumways();
    else if(2==intlevel)
        sayhardways();
    else if(3==intlevel)
        sayhellways();
    else
        sayeasyways();
    generate_map();
}
void GameLevel::generate_map()
{
    if(intlevel<2)
        for(std::size_t a=0;a<9;++a)
            changealine(defaultshow[a],ways[a]);
    else
    {
        for(std::size_t a=0;a<5;++a)
            changealine(defaultshow[a],ways[a]);
        for(std::size_t a=5;a<9;++a)
            defaultshow[a]=defaultshow[8-a];
    }
}
void GameLevel::changealine(std::vector<bool>&a,std::size_t num)
{
    std::size_t s=0;
        std::uniform_int_distribution<std::size_t> un(1,9-s);
        std::size_t n=un(e);
        std::size_t temp=0;
        std::vector<bool>::iterator i=a.begin();
        do{
            if(0==*i)
            temp++;
            if(temp==n)
            {
                *i=1;
                if(++s==num)break;
                i=a.begin();
                std::uniform_int_distribution<std::size_t>ud(1,9-s);
                n=ud(e);temp=0;
            }
            else ++i;
        }while(i!=a.end());
}
