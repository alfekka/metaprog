//
// Created by mila on 10.11.2019.
//
#pragma once
#include "listt.h"

template <class TList, template <class> class Unit>
class GenScatterHierarchy;

template <class Head, class ... Tail, template <class> class Unit>
class GenScatterHierarchy<TypeList<Head, Tail ...>, Unit>:
public GenScatterHierarchy<class TypeList<Head, Tail ...>::head , Unit>,
public GenScatterHierarchy<class TypeList<Head, Tail ...>::tail, Unit> {
public:
using TList = TypeList<Head, Tail ...>;
using LeftBase = GenScatterHierarchy<class TypeList<Head, Tail ...>::head, Unit>;
using RightBase = GenScatterHierarchy<class TypeList<Head, Tail ...>::tail, Unit>;

template <typename A> struct Rebind
{
    using Result = Unit<A>;
};
};

template <class Type, template <class> class Unit>
class GenScatterHierarchy:
        public Unit<Type> {
public:
    using LeftBase = Unit<Type>;
    template <typename T> struct Rebind
    {
        using Result = Unit<T>;
    };
};

template <template <class> class Unit>
class GenScatterHierarchy<NullType, Unit> {
    template <typename A> struct Rebind
    {
        using Result = Unit<A>;
    };
};

template <class A, class H>
typename H::template Rebind<A>::Result& Field(H& obj)
{
    return obj;
}