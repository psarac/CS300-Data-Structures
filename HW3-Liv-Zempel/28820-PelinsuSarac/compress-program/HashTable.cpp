#ifndef _HASHTABLE_CPP
#define _HASHTABLE_CPP

#include <vector>

using namespace std;

/* Begin: code taken from 9-Hashtables2.ppt and updated by Pelinsu Sarac */
template<class Key, class Value>
class HashTable
{
public:
	//parametric constructor
	HashTable(const Value & notFound, int size = 4096*2);
	//copy constructor
	HashTable(const HashTable & rhs)
		: ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), array(rhs.array), currentSize(rhs.currentSize) {}

	const Value & find(const Key &  x) const; //to query key-value tuples using key

	void makeEmpty(); //empties the table
	void insert(const Key & k, const Value & v); //inserts key-value tuple to the table
	void remove(const Key & x); //removes a key-value tuple given key

	enum EntryType{ACTIVE, EMPTY, DELETED}; //to track state of cells of table

private:
	//struct to hold key-value tuples and cell's state
	struct HashEntry
	{
		Key key;
		Value value;
		EntryType info;

		HashEntry(const Key & k = Key(), const Value & v = Value(), EntryType i=EMPTY)
			:key(k), value(v), info(i) {}
	};

	vector<HashEntry> array; //array that the entries will be stored
	int currentSize; //to track to see if there is a need for rehash
	const Value ITEM_NOT_FOUND; //value to return after unsuccessful query

	bool isActive(int currentPos) const; //returns true if cell is occupied
	int findPos(const Key & x) const; //given key, finds appropriate index on table
	void rehash(); //if load factor is greater than half, it resized the table

	//Helper functions
	bool isPrime(int num); //to see if a number is prime
	int nextPrime(int num); //to find next prime number for size of table
};

/*****************************************************************************
This function is the hashing function for the Hash Table, using the key and 
table size it returns the hash code. This function depends on the assumption:
"For most applications, the key (of the data element to be stored) is a 
character string of some sort." (taken from 8-Hashtables.ppt) and
it uses Horner's method to hash the character string.
Input: key of value (sort of character string), hash table size
Returns: hash code 
*****************************************************************************/

/* Begin: code taken from 8-Hashtables.ppt and updated by Pelinsu Sarac */
template <class Key>
int hashFunc(const Key & key, int tableSize)
{
	int hashVal = 0;
	int len = key.length();

	for (int i = 0; i < len; i++)
		hashVal = 37*hashVal + key[i];

	hashVal = hashVal % tableSize;

	if (hashVal < 0)
		hashVal = hashVal + tableSize;

	return hashVal;
}
/* End: code taken from 8-Hashtables.ppt and updated by Pelinsu Sarac */

/*****************************************************************************
This function is the constructor of Hash Table class. It takes a value to return
in case of unsuccessful queries and size of the table. However, it creates table
with next prime number and empty cells.
Input: value to return when key is not found, size of table
Returns: empty Hash Table
*****************************************************************************/

template <class Key, class Value>
HashTable<Key, Value>::HashTable(const Value & notFound, int size = 4096*2)
	:ITEM_NOT_FOUND(notFound), array(nextPrime(size)) 
{
	makeEmpty();
}

/*****************************************************************************
This function empties all cells of hash table.
Input: none
Returns: none (empty hash table)
*****************************************************************************/

template <class Key, class Value>
void HashTable<Key, Value>::makeEmpty()
{
	int size = array.size();
	for( int j = 0; j < size; j++ )
		array[ j ].info = EMPTY;
}

/*****************************************************************************
This function is for finding position on hash table given a key, wether to 
query the table or to find appropriate index to insert a new tuple. It's
implemented according to open address and linear probing. It returns an 
index on table.
Input: key of value 
Returns: appropriate index according to given key
*****************************************************************************/

template<class Key, class Value>
int HashTable<Key, Value>::findPos(const Key & x) const
{
	//find the result of hash 
	int p = hashFunc(x, array.size());

	//until we find an empty cell or the key given on table
	while(array[p].info != EMPTY && array[p].key != x)
	{
		p++; //probe linearly
		//(differences between two position would be 1 due to linearity)

		//take mode if neccessary
		int size = array.size();
		if (p >= size)
			p -= size;
	}

	return p;
}

/*****************************************************************************
This function is to see if a cell is actively occupied or not given its index
Input: position/index of a cell
Returns: true if cell is occupied, false otherwise
*****************************************************************************/

template<class Key, class Value>
bool HashTable<Key, Value>::isActive(int currentPos) const
{
	return array[currentPos].info == ACTIVE;
}

/*****************************************************************************
This function is to remove a key-value tuple from the table given the key value
Input: key of the tuple to be removed
Returns: none (updated hash table)
*****************************************************************************/

template<class Key, class Value>
void HashTable<Key, Value>::remove(const Key & x)
{
	int currentPos = findPos(x);

	if (isActive(currentPos))
		array[currentPos].info = DELETED;
}

/*****************************************************************************
This function is to find the tuple given its key. If tuple exists, it returns
the value of tuple, if not it returns the ITEM_NOT_FOUND value initialized
at construction.
Input: key to be queried
Returns: value of tuple if such tuple exists, ITEM_NOT_FOUND otherwise
*****************************************************************************/

template <class Key, class Value>
const Value & HashTable<Key, Value>::find(const Key & x) const
{
	int currentPos = findPos(x);

	//if cell is actively occupied, it means that tuple exists
	if (isActive(currentPos))
		return array[currentPos].value;

	return ITEM_NOT_FOUND;
}

/*****************************************************************************
This function is to insert key-value tuples into the hash table. It rehashes
the table if load factor is over half.
Input: key and value of the tuple to be inserted
Returns: none (updated hash table)
*****************************************************************************/

template <class Key, class Value>
void HashTable<Key, Value>::insert(const Key & k, const Value & v)
{
	int currentPos = findPos(k);

	if (isActive(currentPos))
		return;

	array[currentPos] = HashEntry(k, v, ACTIVE);

	int size = array.size();
	if (++currentSize >= size/2)
		rehash();
}

/*****************************************************************************
This function is to resize the whole table so that the load factor will be 
lower than half again. It roughly doubles the table size.
Input: none
Returns: none (resized hash table)
*****************************************************************************/

template<class Key, class Value>
void HashTable<Key, Value>::rehash()
{
	vector<HashEntry> oldArray = array;

	//double the table size
	array.resize(nextPrime(2*oldArray.size())); 

	makeEmpty();

	//fill the new table with old tuples
	currentSize = 0;
	int oldSize = oldArray.size();
	for (int i = 0; i < oldSize; i++)
		if (oldArray[i].info == ACTIVE)
			insert(oldArray[i].key, oldArray[i].value);
}

/*****************************************************************************
This function is to see if given number is a prime number. It's a helper
function to find the next prime number for size of table
Input: number
Returns: true if the number is prime, false otherwise
*****************************************************************************/

/* End: code taken from 9-Hashtables2.ppt and updated by Pelinsu Sarac */

template<class Key, class Value>
bool HashTable<Key, Value>::isPrime(int num)
{
	if (num <= 1) return false;

	for (int i = 2; i*i <= num; i++)
	{
		if (num % i == 0)
			return false;
	}
	return true;
}

/*****************************************************************************
This function is to find next prime number given a number. It returns the 
number itself if it's already a prime, otherwise it searches for the next.
Input: number
Returns: next prime number to given number
*****************************************************************************/

template<class Key, class Value>
int HashTable<Key, Value>::nextPrime(int x)
{
	if (isPrime(x)) return x;

	bool isFound = false;
	while (!isFound)
	{
		x++;
		if (isPrime(x))
			isFound = true;
	}
	return x;
}

#endif
