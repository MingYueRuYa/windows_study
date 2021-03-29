#include <iostream>

template <typename T>
class B1
{
public:
    void SyaHi()
    {
        T *pT = static_cast<T *>(this);
        pT->PrintClassName();
    }

protected:
    void PrintClassName() { std::cout <<"this is B1" <<std::endl; }

};

class D1 : public B1<D1>
{
    // TODO nothing
};

class D2 : public B1<D2>
{
protected:
    void PrintClassName() { std::cout << "this is D2" << std::endl; }
};

int main(int argc, char *argv[])
{
    D1 d1;
    D2 d2;
    d1.SyaHi();
    d2.SyaHi();

    getchar();
    return 0;
}