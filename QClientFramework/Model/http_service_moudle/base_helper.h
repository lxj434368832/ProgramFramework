#pragma once
#include <memory>
#include <string>
#include <functional>
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <list>
#include <set>
#include <map>
#include <assert.h>
#if _MSC_VER >= 1500
#include <unordered_set>
#include <unordered_map>

#define hash_set			unordered_set
#define hash_map			unordered_map
#define hash_multimap		unordered_multimap
#else
#include <hash_set>
#include <hash_map>

#define unordered_set		hash_set
#define unordered_map		hash_map
#define unordered_multimap	hash_multimap
#endif


using namespace std;
using namespace stdext;
using namespace std::tr1;

#include "http_service_interface.h"