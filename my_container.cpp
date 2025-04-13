#include <iostream>
#include <iterator>
#include <stdexcept>

using namespace std;

int version() {
	return PROJECT_VERSION_PATCH;
}

// Пользовательский шаблон класса контейнера
template <class T, class Allocator = allocator<T>> 
class MyContainer {
public:
    // Определения типов для упрощения доступа к типам, используемым в контейнере    
    using value_type = T; // Тип элементов, хранящихся в контейнере    
    using allocator_type = Allocator; // Тип распределителя, используемый для управления памятью    
    using reference = value_type&; // Ссылочный тип на элементы контейнера    
    using const_reference = const value_type&; // Постоянный ссылочный тип на элементы контейнера    
    using size_type = size_t; // Тип в зависимости от размера и вместимости контейнера
    using difference_type = ptrdiff_t; // Тип для определения разницы между позициями итератора

    // Класс-итератор для пользовательского контейнера
    class iterator {
    public:
        // Определения типов для итератора        
        using iterator_category = random_access_iterator_tag; // Тег категории для итераторов произвольного доступа
        using value_type = T; // Тип элементов, на которые указывает итератор
        using difference_type = ptrdiff_t; // Тип для определения разницы между итераторами
        using pointer = T*; // Тип указателя на элемент
        using reference = T&; // Ссылочный тип на элемент

        // Конструктор для инициализации итератора указателем на элемент
        iterator(pointer ptr) : ptr_(ptr) {}

        // Оператор разыменования для доступа к элементу, на который указывает итератор
        reference operator*() const {
            return *ptr_;
        }

        // Оператор со стрелкой для доступа к элементам элемента
        pointer operator->() const {
            return ptr_;
        }

        // Оператор увеличения префикса для перемещения итератора к следующему элементу
        iterator& operator++() {
            ++ptr_;
            return *this;
        }

        // Оператор постфиксного приращения для перемещения итератора к следующему элементу и возврата в предыдущее положение
        iterator operator++(int) {
            iterator temp = *this;
            ++ptr_;
            return temp;
        }

        // Оператор равенства для сравнения, равны ли два итератора (указывают на один и тот же элемент)
        bool operator==(const iterator& other) const {
            return ptr_ == other.ptr_;
        }

        // Оператор неравенства для сравнения, если два итератора не равны (указывают на разные элементы)
        bool operator!=(const iterator& other) const {
            return ptr_ != other.ptr_;
        }

    private:
        pointer ptr_; // Указатель на элемент, на который в данный момент указывает итератор
    };

    // Конструктор по умолчанию для создания пустого контейнера
    MyContainer() : data_(nullptr), size_(0), capacity_(0) {}

    MyContainer(const size_t capacity) {       
        T* new_data = allocator_type().allocate(capacity);
        data_ = new_data;
        capacity_ = capacity;
    }
/*
    // Конструктор копирования для создания контейнера как копии другого контейнера
    MyContainer(data_) : size_(other.size_), capacity_(other.capacity_) {
        data_ = allocator_type().allocate(capacity_); // Выделите память для элементов
        copy(other.data_, other.data_ + size_, data_); // Скопируйте элементы из другого контейнера
    }
*/
// Деструктор для очистки выделенной памяти при уничтожении контейнера
    ~MyContainer() {
        allocator_type().deallocate(data_, capacity_); // Освободите выделенную память
    }

    // Функция-член для добавления нового элемента в конец контейнера
    void push_back(const T& value) {
        if (size_ == capacity_) {
            // Если контейнер заполнен, выделите больше памяти
            size_type new_capacity = capacity_ ? 2 * capacity_ : 1;
            T* new_data = allocator_type().allocate(new_capacity);
            copy(data_, data_ + size_, new_data);
            allocator_type().deallocate(data_, capacity_);
            data_ = new_data;
            capacity_ = new_capacity;
        }
        data_[size_++] = value;
    }

    // Функция-член для удаления последнего элемента из контейнера
    void pop_back() {
        // Вынимайте только в том случае, если контейнер не пуст
        if (size_ > 0) {
            --size_;
        }
    }

    // Функция-член для доступа к элементу с заданным индексом с проверкой границ
    reference at(size_type index) {
        if (index >= size_) {
            throw out_of_range("Index out of range"); // Генерирует исключение, если индекс недействителен
        }
        return data_[index];
    }

    // Постоянная версия функции 'at' для доступа к элементам в контексте, доступном только для чтения
    const_reference at(size_type index) const {
        if (index >= size_) {
            throw out_of_range("Index out of range"); // Генерирует исключение, если индекс недействителен
        }
        return data_[index];
    }

    // Функция-член для получения количества элементов, находящихся в данный момент в контейнере
    size_type size() const {
        return size_;
    }

    // Функция-член для проверки того, пуст ли контейнер
    bool empty() const {
        return size_ == 0;
    }

    // Функция-член для получения итератора, указывающего на первый элемент контейнера
    iterator begin() {
        return iterator(data_);
    }

    // Функция-член для получения итератора, указывающего на конец (один за последним элементом) контейнера
    iterator end() {
        return iterator(data_ + size_);
    }

private:
    T* data_; // Указатель на элементы контейнера    
    size_type size_; // Количество элементов в контейнере
    size_type capacity_; // Выделенная вместимость контейнера
};