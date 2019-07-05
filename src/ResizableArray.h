// Copyright 2019  Cosmin-Razvan Vancea

#ifndef RESIZABLEARRAY_H_
#define RESIZABLEARRAY_H_

template <typename T>
class ResizableArray {
 private:
    int defaultCapacity = 5;
    int expandFactor = 2;

    int numElements;
    int maxCapacity;
    T *data;

 public:
    ResizableArray() {
        numElements = 0;
        maxCapacity = defaultCapacity;

        data = new T[maxCapacity];
    }

    explicit ResizableArray(int initialCapacity) {
        numElements = 0;
        maxCapacity = initialCapacity;

        data = new T[maxCapacity];
    }

    ResizableArray(int initialCapacity, int defaultFactor) {
        numElements = 0;
        maxCapacity = initialCapacity;
        expandFactor = defaultFactor;

        data = new T[maxCapacity];
    }

    ResizableArray(const ResizableArray& other) {
        defaultCapacity = other.defaultCapacity;
        expandFactor = other.expandFactor;
        numElements = other.numElements;
        maxCapacity = other.maxCapacity;
        data = new T[maxCapacity];

        for (int i = 0; i < numElements; ++i) {
            data[i] = other.data[i];
        }
    }

    ResizableArray& operator=(const ResizableArray& other) {
        if (this != &other) {
            defaultCapacity = other.defaultCapacity;
            expandFactor = other.expandFactor;
            numElements = other.numElements;
            maxCapacity = other.maxCapacity;
            data = new T[maxCapacity];

            for (int i = 0; i < numElements; ++i) {
                data[i] = other.data[i];
            }
        }

        return *this;
    }

    ~ResizableArray() {
        delete[] data;
    }

    void addLast(T element) {
        if (numElements == maxCapacity) {
            maxCapacity *= expandFactor;
            T *new_data = new T[maxCapacity];

            if (new_data == nullptr) {
                maxCapacity /= expandFactor;
                return;
            }

            for (int i = 0; i < numElements; ++i) {
                new_data[i] = data[i];
            }

            delete[] data;
            data = new_data;
        }

        data[numElements++] = element;
    }

    void addFirst(T element) {
        if (numElements == maxCapacity) {
            maxCapacity *= expandFactor;
            T *new_data = new T[maxCapacity];

            if (new_data == nullptr) {
                maxCapacity /= expandFactor;
                return;
            }

            for (int i = 0; i < numElements; ++i) {
                new_data[i+1] = data[i];
            }

            delete[] data;
            data = new_data;
        } else {
            for (int i = numElements; i > 0; --i) {
                data[i] = data[i-1];
            }
        }

        data[0] = element;
        numElements++;
    }

    T removeLast() {
        if (numElements == 0) {
            return T();
        }

        return data[--numElements];
    }

    T removeFirst() {
        if (numElements == 0) {
            return T();
        }

        T ret = data[0];
        numElements--;
        memmove(data, data + 1, sizeof(T) * numElements);

        return ret;
    }

    void clear() {
        numElements = 0;
    }

    void setElement(int index, T element) {
        if (index < 0) {
            return;
        }

        if (index >= numElements) {
            if (index >= maxCapacity) {
                T *new_data = new T[index + 1];

                if (new_data == nullptr) {
                    return;
                }

                for (int i = 0; i < numElements; ++i) {
                    new_data[i] = data[i];
                }

                delete[] data;
                data = new_data;
                maxCapacity = index + 1;
            }
            numElements = index + 1;
        }

        data[index] = element;
    }

    T getElement(int index) const {
        if (index >= numElements || index < 0) {
            return T();
        }

        return data[index];
    }

    T& operator[](int index) {
        return data[index];
    }

    bool isEmpty() const {
        return (numElements == 0);
    }

    int size() const {
        return numElements;
    }

    T *getData() {
        return data;
    }

    int getMaxCapacity() const {
        return maxCapacity;
    }
};

#endif  // RESIZABLEARRAY_H_
