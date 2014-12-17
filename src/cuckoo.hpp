#ifndef CUCKOO_HPP
#define CUCKOO_HPP

#include <string>
#include <vector>
#include <utility>

extern int MERSENNE_PRIME[];
extern int MERSENNE_PRIME_EXP[];

int mod(int, int, int);

typedef std::pair<std::string, std::string> pair;

extern const pair NULL_PAIR;

class cuckoo
{
	public:
		cuckoo();
		~cuckoo() {}
		int hash1(std::string);
		int hash2(std::string);
		void insert(pair);
		pair lookup(std::string);
		int size() const { return entries; }
		int buckets() const { return p; }
		void print();
	private:
		int mersenne_index;
	
		int a1;
		int a2;
		int n;
		int p;
		int max_loop;
		
		int entries;
		const float MAX_LOAD = 0.4f;
		
		std::vector<pair> t1;
		std::vector<pair> t2;
		
		void grow();
		void rehash();
};

#endif