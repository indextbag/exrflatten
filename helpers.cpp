#include "helpers.h"
#include <stdarg.h>

#include <string>
using namespace std;

void make_swaps(vector<int> order, vector<pair<int,int>> &swaps)
{
    // order[0] is the index in the old list of the new list's first value.
    // Invert the mapping: inverse[0] is the index in the new list of the
    // old list's first value.
    vector<int> inverse(order.size());
    for(int i = 0; i < order.size(); ++i)
	inverse[order[i]] = i;

    swaps.resize(0);

    for(int idx1 = 0; idx1 < order.size(); ++idx1)
    {
	// Swap list[idx] with list[order[idx]], and record this swap.
	int idx2 = order[idx1];
	if(idx1 == idx2)
	    continue;

	swaps.push_back(make_pair(idx1, idx2));

	// list[idx1] is now in the correct place, but whoever wanted the value we moved out
	// of idx2 now needs to look in its new position.
	int idx1_dep = inverse[idx1];
	order[idx1_dep] = idx2;
	inverse[idx2] = idx1_dep;
    }
}

string vssprintf(const char *fmt, va_list va)
{
    // Work around a gcc bug: passing a va_list to vsnprintf alters it. va_list is supposed
    // to be by value.
    va_list vc;
    va_copy(vc, va);

    int iBytes = vsnprintf(NULL, 0, fmt, vc);
    char *pBuf = (char*) alloca(iBytes + 1);
    vsnprintf(pBuf, iBytes + 1, fmt, va);
    return string(pBuf, iBytes);
}

string ssprintf(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    return vssprintf(fmt, va);
}

string subst(string s, string from, string to)
{
    int start = 0;
    while(1)
    {
        auto pos = s.find(from, start);
        if(pos == string::npos)
            break;

        string before = s.substr(0, pos);
        string after = s.substr(pos + from.size());
        s = before + to + after;
        start = pos + to.size();
    }

    return s;
}

/*
 * Return the last named component of dir:
 * a/b/c -> c
 * a/b/c/ -> c
 */
string basename(const string &dir)
{
    size_t end = dir.find_last_not_of("/\\");
    if( end == dir.npos )
        return "";

    size_t start = dir.find_last_of("/\\", end);
    if(start == dir.npos)
        start = 0;
    else
        ++start;

    return dir.substr(start, end-start+1);
}

string setExtension(string path, const string &ext)
{
    auto pos = path.rfind('.');
    if(pos != string::npos)
        path = path.substr(0, pos);

    return path + ext;
}
