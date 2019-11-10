//
// Created by mila on 10.11.2019.
//


class NullType {};

template <class A=NullType, class ... U>
struct TypeList
{
    using head = A;
    using tail = TypeList<U ...>;
};

template <class A>
struct TypeList<A>
{
    using head = A;
    using tail = NullType;
};
