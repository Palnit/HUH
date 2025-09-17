#pragma once
#include <memory>
#include <mutex>

namespace HUH {

template<class BaseClass>
class Singleton {
public:
    static BaseClass& Get(){
        static std::unique_ptr<BaseClass> instance;
        static std::once_flag initFlag;
        std::call_once(initFlag, [](){
            instance.reset(new BaseClass(Initializer()));
        });
        return *instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton() = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

protected:
    struct Initializer {};
    explicit Singleton(Initializer) {};
    ~Singleton() = default;
};
}