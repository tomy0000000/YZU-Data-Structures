#ifndef DEQUE_H
#define DEQUE_H

// deque class template definition
template< typename T >
class deque
{
public:
   deque(); // Constructs an empty deque container, with no elements.

   ~deque(); // Destroys the deque container.

   // Assigns new contents to the container, replacing its current contents,
   // and modifying its size accordingly.
   deque& operator=( const deque &x );

   // Inserts a new element at the beginning of the deque container,
   // right before its current first element.
   // The content of val is copied to the inserted element.
   // This effectively increases the container size by one.
   void push_front( const T val );

private:
   T **map = nullptr;         // pointer to array of pointers to blocks
   unsigned int mapSize = 0;  // size of map array, zero or 2^N
   unsigned int myOff = 0;    // offset of initial element
   unsigned int mySize = 0;   // current length of sequence, in [ 0 .. 4 * mapSize )
}; // end class template deque


// Constructs an empty deque container, with no elements.
template< typename T >
deque< T >::deque()
   :     map( nullptr ),
     mapSize( 0 ),
       myOff( 0 ),
      mySize( 0 )
{
} // end deque default constructor

// destroys the deque container
template< typename T >
deque< T >::~deque()
{
   if( mapSize > 0 )
   {
      for( unsigned int i = 0; i < mapSize; i++ )
         if( map[ i ] != nullptr )
            delete [] map[ i ];
      delete[] map;
   }
} // end destructor

// Assigns new contents to the container, replacing its current contents,
// and modifying its size accordingly.
template< typename T >
deque< T >& deque< T >::operator=( const deque< T > &x )
{
	//return *this;
   if( &x != this ) // avoid self-assignment
   {
	   if (!x.map)
	   {
		   for (unsigned int i = 0; i < mapSize; i++) {
			   if (map[i] != nullptr) {
				   delete[] map[i];
			   }
		   }
		   delete[] map;
		   map = nullptr;
		   mapSize = 0;
		   myOff = 0;
		   mySize = 0;
	   }
	   if (mapSize < x.mapSize)
	   {
		   if (!map)
		   {
			   mapSize = x.mapSize;
			   map = new T*[mapSize]();
			   myOff = x.myOff;
		   }
		   else {
			   int times = x.mapSize / mapSize;
			   T **newMap = new T*[x.mapSize]();
			   for (int i = 0; i < mySize; i++)
			   {
				   int old_block_ind = (myOff + i >= mapSize * 4) ? myOff + i - mapSize * 4 : myOff + i;
				   int new_block_ind = (myOff + i >= mapSize * times * 4) ? myOff + i - mapSize * times * 4 : myOff + i;
				   newMap[new_block_ind / 4] = map[old_block_ind / 4];
			   }
			   delete[] map;
			   map = newMap;
			   mapSize = x.mapSize;
		   }
	   }
	   for (int i = 0; i < x.mySize; i++)
	   {
		   int self_ind = (myOff + i >= mapSize * 4) ? myOff + i - mapSize * 4 : myOff + i;
		   int copy_ind = (x.myOff + i >= x.mapSize * 4) ? x.myOff + i - x.mapSize * 4 : x.myOff + i;
		   if (!map[self_ind / 4])
		   {
			   map[self_ind / 4] = new T[4]();
		   }
		   map[self_ind / 4][self_ind % 4] = x.map[copy_ind / 4][copy_ind % 4];
	   }
	   mySize = x.mySize;
   } // end outer if
   return *this; // enables x = y = z, for example
} // end function operator=

template< typename T >
void deque< T >::push_front( const T val )
{
	//return;
	// Check Map & Calibrate myOff
	if (!map)
	{
		mapSize = 8;
		map = new T*[mapSize]();
		myOff = 31;
	}
	else {
		// Check Map Expandtion
		if (mySize / 4 >= mapSize - 1)
		{
			T **newMap = new T*[mapSize * 2]();
			for (int i = 0; i < mySize; i++)
			{
				int old_block_ind = (myOff + i >= mapSize * 4) ? myOff + i - mapSize * 4 : myOff + i;
				int new_block_ind = (myOff + i >= mapSize * 2 * 4) ? myOff + i - mapSize * 2 * 4 : myOff + i;
				newMap[new_block_ind / 4] = map[old_block_ind / 4];
			}
			delete[] map;
			map = newMap;
			mapSize *= 2;
		}
		myOff = (myOff == 0) ? mapSize * 4 - 1 : myOff - 1;
	}

	// Chekc Block Allocation
	if (!map[myOff / 4])
	{
		map[myOff / 4] = new T[4]();
	}
	map[myOff / 4][myOff % 4] = val;
	mySize++;
}

#endif

