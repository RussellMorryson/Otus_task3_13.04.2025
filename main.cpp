#include <iostream>
#include <memory>
#include <vector>
#include <map>

#include "my_container.cpp"

// Аллокатор
template <typename T>
struct my_allocator {
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template <typename U>
    struct rebind {
        using other = my_allocator<U>;
    };

    my_allocator() = default;
    ~my_allocator() = default;

    template <typename U>
    my_allocator(const my_allocator<U>&) {}

    T* allocate(std::size_t n) {
        auto p = std::malloc(n * sizeof(T));
        if (!p) {
            throw std::bad_alloc();
        }
        return reinterpret_cast<T*>(p);
    }

    void deallocate(T* p, std::size_t n) {
        std::free(p);
    }

    template <typename U, typename... Args>
    void construct(U* p, Args &&...args) {
        new (p) U(std::forward<Args>(args)...);
    };

    template <typename U>
    void destroy(U* p) {
        p->~U();
    }
};

//====================================================================
// Запись значений факториала в словарь
void factorial_to_map(std::map<int, int>& map_a) {
    int value = 0;
    for (int i = 0; i < 10; i++) {
        if (i == 0 || i == 1) {
            map_a[i] = 1;
        }
        else {
            value = i;
            for (int j = i - 1; j > 0; j--) {
                value *= j;
            }
            map_a[i] = value;
        }
    }
}

// Вывод на экран словаря
void print_map(std::map<int, int>& map_a) {
    for (std::map<int, int>::const_iterator it = map_a.begin(); it != map_a.end(); ++it) {
        std::cout << it->first << " " << it->second << "\n";
    }
    std::cout << "Size: " << map_a.size() << "\n";
}
//====================================================================

// Перегрузка функций factorial_to_map и print_map
//====================================================================
// Запись значений факториала в словарь
void factorial_to_map(std::map<int, int, std::less<int>, my_allocator<std::pair<const int, int>>>& map_a) {
    int value = 0;
    for (int i = 0; i < 10; i++) {
        if (i == 0 || i == 1) {
            map_a[i] = 1;
        }
        else {
            value = i;
            for (int j = i - 1; j > 0; j--) {
                value *= j;
            }
            map_a[i] = value;
        }
    }
}

// Вывод на экран словаря
void print_map(std::map<int, int, std::less<int>, my_allocator<std::pair<const int, int>>>& map_a) {
    for (std::map<int, int>::const_iterator it = map_a.begin(); it != map_a.end(); ++it) {
        std::cout << it->first << " " << it->second << "\n";
    }
    std::cout << "Size: " << map_a.size() << "\n\n";
}
//====================================================================

int main() {
    // создание экземпляра std::map<int, int>
    std::map <int, int> map_1;
    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    factorial_to_map(map_1);
    map_1.clear();

    // создание экземпляра std::map<int, int> с новым аллокатором, ограниченным 10 элементами
    auto map_2 = std::map<int, int, std::less<int>, my_allocator <std::pair<const int, int>>>{};
    // заполнение 10 элементами, где ключ - это число от 0 до 9, а значение - факториал ключа
    factorial_to_map(map_2);
    // вывод на экран всех значений (ключ и значение разделены пробелом) хранящихся в контейнере
    cout << "Print map values: " << endl;
    print_map(map_2);

    // создание экземпляра своего контейнера для хранения значений типа int    
    MyContainer<int> my_container_1;
    // заполнение 10 элементами от 0 до 9    
    for (int i = 0; i < 10; i++) {
        my_container_1.push_back(i);
    }

    // создание экземпляра своего контейнера для хранения значений типа int с новым аллокатором, ограниченным 10 элементами  
    auto my_container_2 = MyContainer<int, my_allocator<int> >(10);   
    // заполнение 10 элементами от 0 до 9
    for (int i = 0; i < 10; i++) {        
        my_container_2.push_back(i);
    }
    cout << "Print my_container values: " << endl;
    // вывод на экран всех значений, хранящихся в контейнере
    for (auto i : my_container_2) {
        cout << i << ' ';
    }
}