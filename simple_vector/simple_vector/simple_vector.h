#pragma once

#include <cstdlib>

// � еализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
    SimpleVector() : data(nullptr), end_(nullptr), capacity_(0), size_(0) {};
    explicit SimpleVector(size_t size) {
        capacity_ = size;
        size_ = size;
        data = new T[size];
        end_ = data + size;
    }
    ~SimpleVector() {
        delete[] data;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    T* begin() {
        return data;
    }
    T* end() {
        return end_;
    }

    const T *begin() const {
        return data;
    }
    const T *end() const {
        return end_;
    }

    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return capacity_;
    }
    void PushBack(const T& value) {
        if (size_ == 0) {
            capacity_ = 1;
            data = new T[capacity_];
        }
        else if (capacity_ == size_) {
            capacity_ *= 2;
            T* temp = data;
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = temp[i];
            }
            delete[] temp;
        }
        data[size_] = value;
        size_++;
        end_ = data + size_;
    }

private:
    T* data;
    T* end_;
    size_t capacity_;
    size_t size_;
    // Добавьте поля для хранения данных вектора
}; 
