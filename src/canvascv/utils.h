#ifndef UTILS_H
#define UTILS_H

#include <functional>
#include <list>

namespace canvascv
{

template <typename... Args>
class Dispathcer
{
public:
    typedef std::function<void(Args...)> CBType;
    
    class CBID
    {
    public:
        CBID() : valid(false) {}
    private:
        friend class Dispathcer<Args...>;
        CBID(typename std::list<CBType>::iterator i)
            : iter(i), valid(true)
        {}
        
        typename std::list<CBType>::iterator iter;
        bool valid;
    };
    
    // register to be notified
    CBID addCB(CBType cb)
    {
        cbs.push_back(cb);
        return CBID(--cbs.end());
    }

    // unregister to be notified
    void delCB(const CBID &id)
    {
        if (id.valid)
        {
            cbs.erase(id.iter);
            const_cast<CBID &>(id).valid = false;
        }
    }
    
    void broadcast(Args... args)
    {
        for (auto &cb : cbs)
        {
            if (cb)
            {
                cb(args...);
            }
        }
    }

private:
    std::list<CBType> cbs;
};

}

#endif // UTILS_H
