#include "table.h"
#include "sudoku.h"

Table::Table(std::size_t a):tab(new Numbertable(a))
{
}
Table::Table():tab(nullptr)
{

}

std::pair<std::size_t,std::size_t> Table::help()
{
    return tab->help();
}

std::vector<std::vector<std::size_t> > Table::copy()
{
    return tab->copy();
}
void Table::set(const std::vector<std::vector<std::size_t> >& vec)
{
    tab->set(vec);
}

const Table& Table::operator=(Table&rhs)
{
    Numbertable *temp=rhs.tab;
    rhs.tab=tab;
    tab=temp;
    return *this;
}

const std::vector<std::vector<std::size_t> >& Table::result()
{
    return tab->getresult();
}

const std::vector<std::size_t>& Table::operator[](std::size_t a)const
{
    return (*tab)[a];
}
std::vector<std::size_t>& Table::operator[](std::size_t a)
{
    return (*tab)[a];
}
void Table::check1(std::size_t a,std::size_t b,std::set<std::pair<std::size_t,std::size_t> >&vecpr)
{
    tab->check1(a,b,vecpr);
}
void Table::check2(std::size_t a,std::size_t b,std::set<std::pair<std::size_t,std::size_t> >&vecpr)
{
    tab->check2(a,b,vecpr);
}

Table::~Table(){delete tab;}

bool Table::haswrong(std::size_t i, std::size_t j)
{
    return tab->haswrong(i,j);
}
bool Table::haswrong2(std::size_t i, std::size_t j)
{
    return tab->haswrong2(i,j);
}
bool Table::checkwin()
{
    return tab->checkwin();
}
