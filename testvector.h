namespace teststl
{

	template<typename T>
	class tvector
	{
	private:
		// container to hold actual contents
		// we need to keep this separate to get COW semantics working
		struct storage {
			int ref;
			T* arr;
		};

		storage* stor;

		// to speedup access
		T* direct_access;

		// length of the array
		int length;

		// max size of allocated elements as of now (not used as of now)
		int size;

		// last index - for push_back
		int last;

	public:
		/* 
		* Constructors
		*/

		// allocate memory
		void allocvec(int length) {
			// length is already done
			stor = new storage;
			stor->ref = 1;
			if (length) {
				stor->arr = new T[length];
			}
			direct_access = stor->arr;
			last = 0;
		}

		// default constructor
		tvector() : length(0) {
			allocvec(0);
		}

		// explicit int constructor
		tvector(int length) : length(length) {
			allocvec(length);
		}

		// length, value constructor
		tvector(int length, int val) : length(length) {
			allocvec(length);
			for (int i = 0; i < length; i++) {
				direct_access[i] = val;
			}
		}

		// generic COW function
		void copy(const tvector<T>& other) {
			size = other.size;
			length = other.length;
			stor = other.stor;
			stor->ref++;
			last = other.last;
		}

		void grow() {
			
		}

		// copy constructor: this has to be smart to do COW
		// for CoW all we need to do is increment the ref
		tvector(const tvector<T>& other) {
			std::cout << "Copy" << std::endl;
			copy(other);
		}

		// iterator based constructor

		/*
		* Others
		*/
		const T& val(int i) {
			if (i < 0 || i >= length) {
				return NULL;
			}
			return direct_access[i];
		}

		// deref operator
		const T& operator[](int i) {
			return val(i);
		}
		
		// deref operator
		T& operator[](int i) {
			// make copy
			return val(i);
		}

		// value at
		T& at(int i) {
			return val(i);
		}

		// capacity
		int capacity() {
			return length;
		}

		// pop back - this will modify array
		T& pop_back() {
			// Now we need to make a copy for ourselves first
			if (last == 0) {
				return NULL;
			}
			return direct_access[--last];
		}

		// push back - this will modify array
		void push_back(const T& val) {
			if (length == last) {
				// allocate more (double of last size)
				grow();
			}
			// length should now be more than last
			direct_access[last++] = val;
		}

		// assignment operator
		const T& operator=(const tvector<T>& other) {
			std::cout << "Assignment" << std::endl;
			copy(other);
		}
	};
}
