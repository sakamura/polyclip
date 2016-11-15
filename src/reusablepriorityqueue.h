/*
 This version is licensed as : http://unlicense.org . Please see LICENSE and README.md for more information.
 Enjoy the code :)
 */
#ifndef reusablepriorityqueue_h
#define reusablepriorityqueue_h

#include <vector>

// Very simple adapter giving a priority queue that can be reset and reused.
template <class _Tp, class _Container = std::vector<_Tp>,
    class _Compare = std::less<typename _Container::value_type> >
class reusable_priority_queue
{
public:
    typedef _Container                               container_type;
    typedef _Compare                                 value_compare;
    typedef typename container_type::value_type      value_type;
    typedef typename container_type::reference       reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::size_type       size_type;
protected:
    container_type c;
    size_type where;

public:
    explicit reusable_priority_queue() : where(0)
    {
        
    }
    
    void set(container_type&& __c)
    {
        c = std::move(__c);
        where = 0;
        std::sort(c, value_compare());
    }
    inline bool hasData() const
    {
        return !c.empty();
    }
    inline void reset()
    {
        where = 0;
    }
    inline bool empty() const
    {
        return size() == 0;
    }
    inline size_type size() const
    {
        return c.size() - where;
    }
    inline const_reference top() const
    {
        return c.at(size()-1);
    }
    inline void pop()
    {
        ++where;
    }
};
#endif /* reusablepriorityqueue_h */
