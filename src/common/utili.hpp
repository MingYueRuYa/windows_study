/****************************************************************************
**
** Copyright (C) 2018 635672377@qq.com
** All rights reserved.
**
****************************************************************************/

#include <memory>

using std::shared_ptr;

namespace std
{

// �Զ�������shared_ptr
template <typename T>
shared_ptr<T> make_shared_array(size_t size)
{
    //default_delete��STL�е�Ĭ��ɾ����
    return shared_ptr<T>(new T[size], default_delete<T[]>());
}

}; // std