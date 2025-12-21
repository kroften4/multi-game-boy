#ifndef __COMPONENT_POOL_H__
#define __COMPONENT_POOL_H__

template <typename T>
struct ComponentPool
{
    T *components;
    int capacity;
    int size = 0;

    ComponentPool<T>(int size) : capacity{ size }
    {
        components = new T[size];
    };

    ~ComponentPool<T>()
    {
        delete[] components;
    };

    void add(T item)
    {
        components[size++] = item;
    };
};


#endif // __COMPONENT_POOL_H__
