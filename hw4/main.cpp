#include <functional>
#include <iostream>
using std::function;
template <class... T>
class ArgsPack;

template <>
class ArgsPack<> {
public:
    ArgsPack() = default;

    template <class Function, class... Args>
    auto call(Function function, Args... args) -> decltype(function(args...)) {
        return function(args...);
    }
};

template <class T>
class Convert {
public:
    static T* ToPointer(T& param) { return &param; }

    static T* ToPointer(T* param) { return param; }

    static T& ToReference(T& param) { return param; }

    static T& ToReference(T* param) { return *param; }
};

template <class Head, class... Tail>
class ArgsPack<Head, Tail...> {
    typedef ArgsPack<Tail...> Base;

private:
    Head param_;
    Base base_;

public:
    explicit ArgsPack(Head param, Tail... params)
            : base_(params...), param_(param) {}

    // typedef Convert<Head> Convert;

    template <class Function, class... Args>
    auto call(const Function& function, Args... args)
    -> decltype(base_.call(function, args...,
                           Convert<Head>::ToPointer(this->param_))) {
        return base_.call(function, args..., Convert<Head>::ToPointer(param_));
    }

    template <class Function, class... Args>
    auto call(const Function& function, Args... args)
    -> decltype(base_.call(function, args...,
                           Convert<Head>::ToReference(this->param_))) {
        return base_.call(function, args...,
                          Convert<Head>::ToReference(param_));
    }
};
template <class Function>
class FunctionWrapper {
private:
    Function function_;

public:
    explicit FunctionWrapper(const Function function) : function_(function) {}

    template <class... Args>
    auto operator()(Args... args) -> decltype(function_(args...)) {
        return function_(args...);
    }
};

template <class Class, class Callable>
class ObjectWrapper {
private:
    Class& object_;
    Callable callable_;

public:
    ObjectWrapper(Class& object, const Callable& callable)
            : object_(object), callable_(callable) {}

    template <class... Args>
    auto operator()(Args... args) -> decltype((object_.*callable_)(args...)) {
        return (object_.*callable_)(args...);
    }
};

template <class Function, class... Args>
class Functor {
private:
    typedef ArgsPack<Args...> base;
    Function function_;
    base base_;

public:
    explicit Functor(Function function, Args... args)
            : base_(args...), function_(function) {}

    template <class Object>
    auto call(Object& object)
    -> decltype(base_.call(ObjectWrapper<Object, Function>(object,
                                                           function_))) {
        return base_.call(ObjectWrapper<Object, Function>(object, function_));
    }

    template <class Callable = Function>
    auto call() -> decltype(base_.call(FunctionWrapper<Callable>(function_))) {
        return base_.call(FunctionWrapper<Callable>(function_));
    }
};

template <class Function, class... Args>
Functor<Function, Args...> make_functor(Function function, Args... args) {
    return Functor<Function, Args...>(function, args...);
}
class C {
public:
    int f(int x) { return x * x; }
};
int Twice(int a) { return 2 * a; }
int main() {
    auto functor1 = make_functor(
            [](int a, int b, int c) { return a * a + b + c; }, 5, 7, 9);
    std::cout << functor1.call() << std::endl;
    auto functor2 = make_functor(Twice, 4);
    std::cout << functor2.call() << std::endl;
    C c;
    auto functor3 = make_functor(&C::f, 5);
    std::cout << functor3.call(c) << std::endl;


}