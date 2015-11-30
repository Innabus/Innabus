#ifndef IB_STD_H
#define IB_STD_H
#pragma once

#include "ibStdAllocator.h"

#include <list>
#include <map>
#include <vector>
#include <queue>

template <class T>
class ibList : public std::list<T, ibAllocator<T>> {};

template <class T>
class ibVector : public std::vector<T, ibAllocator<T>> {};

template <class K, class V, class Comp = std::less<K>>
class ibMap : public std::map<K, V, Comp, ibAllocator<std::pair<const K, V>>> {};

template <class T>
class ibQueue : public std::queue<T, std::deque<T, ibAllocator<T>>> {};

#endif // IB_STD_H
