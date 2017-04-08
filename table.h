#ifndef TABLE_H
#define TABLE_H

#include<set>
#include<vector>
class Numbertable;//decleration forward
class Table
{
private:
    Numbertable *tab;
public:
    explicit Table(std::size_t a);
    Table();
    const Table& operator=(Table&rhs);
    ~Table();
    const std::vector<std::size_t>& operator[](std::size_t a)const;
    std::vector<std::size_t>& operator[](std::size_t a);
    void check1(std::size_t a, std::size_t b, std::set<std::pair<std::size_t, std::size_t> > &vecpr);
    void check2(std::size_t a, std::size_t b, std::set<std::pair<std::size_t, std::size_t> > &vecpr);
    bool haswrong(std::size_t i,std::size_t j);
    bool haswrong2(std::size_t i,std::size_t j);
    bool checkwin();
    std::vector<std::vector<std::size_t> > copy();
    void set(const std::vector<std::vector<std::size_t> >& vec);
    const std::vector<std::vector<std::size_t> >& result();
    std::pair<std::size_t,std::size_t> help();
};

#endif // TABLE_H
