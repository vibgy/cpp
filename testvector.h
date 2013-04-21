namespace teststl
{

	template<typename T>
	class tvector
	{
	private:
		// container to hold actual contents
		// we need to keep this separate to get CoW semantics working
		struct storage {
			int ref;
			T* arr;
		};

		storage* stor;

		// to speedup access
		T* direct_access;

		// length of the array
		int length;

		// max size of allocated elements as of now
		int size;

	public:
		/* 
		* Constructors
		*/

		// allocate memory
		void allocvec(int len) {
			stor = new storage;
			stor->ref = 1;
			stor->arr = new T[len];
			direct_access = stor->arr;
		}

		// default constructor
		tvector() : length(0) {
			allocvec(0);
		}

		// explicit int constructor
		tvector(int len) : length(len) {
			allocvec(len);
		}

		// length, value constructor
		tvector(int len, int val) : length(len) {
			allocvec(len);
			for (int i = 0; i < len; i++) {
				direct_access[i] = val;
			}
		}

		// generic COW function
		void copy(const tvector<T>& other) {
			size = other.size;
			len = other.len;
			stor = other.stor;
			stor->ref++;
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

		// pop back - this will modift array
		T& pop_back() {
			// Now we need to make a copy for ourselves first
			return direct_access[--length];
		}

		// push back - this will modify array
		void push_back(const T& val) {

		}

		// assignment operator
		const T& operator=(const tvector<T>& other) {
			std::cout << "Assignment" << std::endl;
			copy(other);
		}
	};
}
