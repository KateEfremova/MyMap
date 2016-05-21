#include <map>
#include <iostream>
#include <cstdio>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <set>
#include <algorithm>
#include <unordered_map>

#include <Map.h>

using namespace std;

int main() 
{
	Map<int, int> myMap;
	map<int, int> stdMap;
	unordered_map<int, int> unordMap;
	vector<pair<int, int> > testValues;
	set<int> usedKeys;
	srand(time(0));

	const int testNum = 10000000;

	cout << "generating data..." << endl;
	for( int i = 0; i < testNum; ++i ) {
		testValues.push_back( make_pair( i, rand() ) );
	}
	cout << "data generated" << endl;
	cout << "shuffling..." << endl;
	random_shuffle( testValues.begin(), testValues.end() );
	cout << "shuffled" << endl;

	clock_t start = clock();	
	for( int i = 0; i < testNum; ++i ) {
		myMap.add( testValues[i].first, testValues[i].second );
	}
	clock_t end = clock();
	cout << "MyMap time: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
	
	start = clock();
	for( int i = 0; i < testNum; ++i ) {
		stdMap.insert( testValues[i] );
	}
	end = clock();
	cout << "StdMap time: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
	
	start = clock();
	for( int i = 0; i < testNum; ++i ) {
		unordMap.insert( testValues[i] );
	}
	end = clock();
	cout << "UnorderedMap time: " << (double)(end - start) / CLOCKS_PER_SEC << endl;
	
	cout << "Checking remained " << stdMap.size() << " values..." << endl;
	for( map<int, int>::iterator it = stdMap.begin(); it != stdMap.end(); ++it ) {
		int x = myMap.get( it->first );
		if( it->second != x ) {
			cout << "Wrong value with key " << it->first <<  " : got " << x << " instead of " << it->second << endl;
		}
	}
	cout << "Check finished" << endl;
	
	return 0;
}
