#ifndef SINGLETON_H
#define SINGLETON_H
#include <pthread.h>
namespace vincy {
//不考虑delete，因为singleton通常和程序生命一样长
template<class T>
class Singleton
{
public:
    static T& instance(){
        pthread_once(&once_, &Singleton::init);
        return *value_;
    }

    static void init() { value_ = new T(); }
private:
    Singleton();
    ~Singleton();

    static T*  value_;
    static pthread_once_t once_;
};

template<class T>
T* Singleton<T>::value_ = nullptr;

template<class T>
pthread_once_t Singleton<T>::once_ = PTHREAD_ONCE_INIT;




}
#endif // SINGLETON_H
