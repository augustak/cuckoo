#include "cuckoo.hpp"

#include <ctime>
#include <cstdlib>

#include <iostream>

int MERSENNE_PRIME[4] = { 127, 8191, 131071, 524287 };
int MERSENNE_PRIME_EXP[4] = { 7, 13, 17, 19 }; 

const pair NULL_PAIR("", "");

int mod(int k, int n, int p)
{
	int i = (k & p) + (k >> n);
	return (i >= p) ? i - p : i;
}

cuckoo::cuckoo()
{
	entries = 0;
	mersenne_index = 0;
	p = MERSENNE_PRIME[mersenne_index];
	n = MERSENNE_PRIME_EXP[mersenne_index];
	a1 = rand() % (p-1) + 1;
	a2 = rand() % (p-1) + 1;
	std::cout << "a1: " << a1 << " a2: " << a2 << " n: " << n << " p: " << p << std::endl;
	max_loop = p;
	for(int i = 0; i < p; ++i)
	{
		t1.push_back(NULL_PAIR);
		t2.push_back(NULL_PAIR);
	}
}

pair cuckoo::lookup(std::string s)
{
	int i = hash1(s);
	if(t1[i].first == s) return t1[i];
	int j = hash2(s);
	if(t2[j].first == s) return t2[j];
	return NULL_PAIR;
}

void cuckoo::insert(pair p)
{
	if(lookup(p.first)!=NULL_PAIR) return;
	for(int i = 0; i < max_loop; ++i)
	{
		std::swap(p, t1[hash1(p.first)]);
		if(p==NULL_PAIR) { ++entries; return; }
		std::swap(p, t2[hash2(p.first)]);
		if(p==NULL_PAIR) { ++entries; return; }
	}
	rehash();
	insert(p);
}

void cuckoo::print()
{
	std::cout << "\t\tTABLE 1" << std::endl;
	for(int i = 0; i < buckets(); ++i)
	{
		if(t1[i] != NULL_PAIR) std::cout << "INDEX: " << i << " ( " << t1[i].first << " , " << t1[i].second << " )" << std::endl;
	}
	
	std::cout << "\t\tTABLE 2" << std::endl;
	for(int i = 0; i < buckets(); ++i)
	{
		if(t2[i] != NULL_PAIR) std::cout << "INDEX: " << i << " ( " << t2[i].first << " , " << t2[i].second << " )" << std::endl;
	}
}

void cuckoo::rehash()
{
	std::vector<pair> pairs;
	
	for(int i = 0; i < buckets(); ++i)
	{
		if(t1[i] != NULL_PAIR) { pairs.push_back(t1[i]); t1[i] = NULL_PAIR; }
		if(t2[i] != NULL_PAIR) { pairs.push_back(t2[i]); t2[i] = NULL_PAIR; }
	}
	
	if(float(size())/buckets() > MAX_LOAD) grow();
	
	a1 = rand() % (p-1) + 1;
	a2 = rand() % (p-1) + 1;
	std::cout << "a1: " << a1 << " a2: " << a2 << " n: " << n << " p: " << p << std::endl;
	
	for(int i = 0; i < pairs.size(); ++i)
	{
		insert(pairs[i]);
	}
}

void cuckoo::grow()
{
	++mersenne_index;
	int old_buckets = buckets();
	p = MERSENNE_PRIME[mersenne_index];
	n = MERSENNE_PRIME_EXP[mersenne_index];
	
	t1.reserve(buckets());
	t2.reserve(buckets());
	
	for (int i = old_buckets; i < buckets(); ++i)
	{
		t1.push_back(NULL_PAIR);
		t2.push_back(NULL_PAIR);
	}
}

int cuckoo::hash1(std::string s)
{
	int h = mod(s[0]*a1, n, p);
	for(int i = 1; i < s.size(); ++i)
	{
		h = mod(((h*a1) + s[i]), n, p);
	}
	return h;
}

int cuckoo::hash2(std::string s)
{
	int h = mod(s[0]*a2, n, p);
	for(int i = 1; i < s.size(); ++i)
	{
		h = mod(((h*a2) + s[i]), n, p);
	}
	return h;
}