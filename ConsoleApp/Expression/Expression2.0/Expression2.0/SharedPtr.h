#ifndef SharedPtr_H
#define SharedPtr_H

#include <crtdbg.h>


template<class T>
class SharedPtr
{
public :
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
private :
    pointer _Ptr;
    int *_pRefCount;
public :
    // constructor
    explicit SharedPtr(pointer ptr);
    SharedPtr(const SharedPtr<T> &sharedPtrRef);
    ~SharedPtr();
    // Overried operator
    reference operator *();
    const_reference operator *() const;
    pointer operator ->();
    const_pointer operator ->() const;
    SharedPtr<T>& operator =(const SharedPtr<T> &sharedPtrRef);
};

template<class T>
SharedPtr<T>::SharedPtr(pointer ptr)
{
    _ASSERT(NULL != ptr);

    _Ptr = ptr;
    _pRefCount = new int;
    *_pRefCount = 1;
}

template<class T>
SharedPtr<T>::SharedPtr(const SharedPtr<T> &sharedPtrRef)
{
    _ASSERT(*(sharedPtrRef._pRefCount) > 0);

    _Ptr = sharedPtrRef._Ptr;
    _pRefCount = sharedPtrRef._pRefCount;
    ++(*_pRefCount);
}

template<class T>
SharedPtr<T>::~SharedPtr()
{
    _ASSERT(*_pRefCount > 0);

    --(*_pRefCount);
    if (0 == *_pRefCount)
    {
        delete _Ptr;
        _Ptr = NULL;
        delete _pRefCount;
        _pRefCount = NULL;
    }
}

template<class T>
typename SharedPtr<T>::reference SharedPtr<T>::operator *()
{
    return (*_Ptr);
}

template<class T>
typename SharedPtr<T>::const_reference SharedPtr<T>::operator *() const
{
    return (*_Ptr);
}

template<class T>
typename SharedPtr<T>::pointer SharedPtr<T>::operator ->()
{
    return _Ptr;
}

template<class T>
typename SharedPtr<T>::const_pointer SharedPtr<T>::operator ->() const
{
    return _Ptr;
}

template<class T>
SharedPtr<T>& SharedPtr<T>::operator =(const SharedPtr<T> &sharedPtrRef)
{
    _ASSERT(*(sharedPtrRef._pRefCount) > 0);
    _ASSERT(*_pRefCount > 0);

    if (this != &sharedPtrRef)
    {
        --(*_pRefCount);
        if (0 == *_pRefCount)
        {
            delete _Ptr;
            _Ptr = NULL;
            delete _pRefCount;
            _pRefCount = NULL;
        }

        _Ptr = sharedPtrRef._Ptr;
        _pRefCount = sharedPtrRef._pRefCount;
        ++(*_pRefCount);
    }

    return (*this);
}

#endif