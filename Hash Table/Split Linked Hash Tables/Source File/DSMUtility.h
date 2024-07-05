#ifndef __DSMUTILITY__H__
#define __DSMUTILITY__H__


namespace DSM {

// �ж��Ƿ�������
inline bool IsPrime(size_t& n) 
{
    if (n == 2) {
        return true;
    }
    if (n <= 1) {
        return false;
    }
    if (n % 2 == 0) {
        ++n;
        return false;
    }
    for (size_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

// �����������һ����������������
inline size_t NextPrime(const size_t& n)
{
    if (n <= 1) {
        return 2;
    }
    size_t i = n;
    for (; !IsPrime(i); i += 2);
    return i;
}


}


#endif // !__DSMUTILITY__H__
