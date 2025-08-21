/**
 * @file vtype.h
 * @author Vlad Kartsaev
 * @brief Voxel3d types
 * @version 0.1
 * @date 2024-03-06
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <algorithm>
#include <vector>
#include <iostream>

namespace vtype {
    /**
     * @brief Вектор с возможностью поиска по значению  
     * 
     * @tparam T Тип
     */
    template<typename T>
    class fndvector {
    private:
        std::vector<T> _data;

    public:
        void push_back(const T& value) {
            _data.push_back(value);
        }

        T at(size_t n) {
            return _data[n];
        }

        bool contains(const T &value) const {
            return std::find(_data.begin(), _data.end(), value) != _data.end();
        }

        int32_t indexOf(const T &value) {
            auto it = std::find(_data.begin(), _data.end(), value);
            if (it != _data.end()) {
                return std::distance(_data.begin(), it);
            } else {
                return -1;
            }
        }

        size_t size() const {
            return _data.size();
        }

        void clear() {
            _data.clear();
        }
    };

    /**
     * @brief Трехмерный массив
     * 
     * @tparam X Тип индекса
     * @tparam T Тип значения
     */
    template<typename T>
    class array3 {
    private:
        int _sizex = 0;
        int _sizey = 0;
        int _sizez = 0;
        size_t _capacity = 0;
        size_t _incr = 0;

        int*** _valueind = nullptr;  // Мапа: [x][y][z] → индекс
        T* _value = nullptr;         // Реальные данные

    public:
        array3(int sizex, int sizey, int sizez) {
            _sizex = sizex;
            _sizey = sizey;
            _sizez = sizez;
            _capacity = static_cast<size_t>(_sizex) * _sizey * _sizez;

            _value = new T[_capacity];
            for (size_t i = 0; i < _capacity; ++i)
                _value[i] = nullptr;

            _valueind = new int**[_sizex];
            for (int x = 0; x < _sizex; ++x) {
                _valueind[x] = new int*[_sizey];
                for (int y = 0; y < _sizey; ++y) {
                    _valueind[x][y] = new int[_sizez];
                    for (int z = 0; z < _sizez; ++z) {
                        _valueind[x][y][z] = -1;
                    }
                }
            }
        }

        // Конструктор копирования
        array3(const array3<T>& other) {
            _sizex = other._sizex;
            _sizey = other._sizey;
            _sizez = other._sizez;
            _capacity = other._capacity;

            // Копируем сам массив значений (указатели на Voxel)
            _value = new T[_capacity];
            for (size_t i = 0; i < _capacity; ++i) {
                _value[i] = other._value[i];  // поверхностное копирование указателей
            }

            // Копируем индексы
            _valueind = new int**[_sizex];
            for (int x = 0; x < _sizex; ++x) {
                _valueind[x] = new int*[_sizey];
                for (int y = 0; y < _sizey; ++y) {
                    _valueind[x][y] = new int[_sizez];
                    for (int z = 0; z < _sizez; ++z) {
                        _valueind[x][y][z] = other._valueind[x][y][z];
                    }
                }
            }
        }

        ~array3() {
            for (int x = 0; x < _sizex; ++x) {
                for (int y = 0; y < _sizey; ++y) {
                    delete[] _valueind[x][y];
                }
                delete[] _valueind[x];
            }
            delete[] _valueind;
            delete[] _value;
        }

        bool inBounds(int x, int y, int z) const {
            return (x >= 0 && x < _sizex) &&
                (y >= 0 && y < _sizey) &&
                (z >= 0 && z < _sizez);
        }

        size_t add(int x, int y, int z, T value) {
            if (!inBounds(x, y, z))
                throw std::out_of_range("array3: out of bounds");
            
                _value[_incr] = value;
            _valueind[x][y][z] = static_cast<int>(_incr);
            return _incr++;
        }

        T get(int x, int y, int z) const {
            if (!inBounds(x, y, z))
                throw std::out_of_range("array3: out of bounds");

            int index = _valueind[x][y][z];
            if (index == -1) return nullptr;
            return _value[index];
        }

        void assign(int x, int y, int z, T value) {
            if (!inBounds(x, y, z))
                throw std::out_of_range("array3: out of bounds");

            int index = _valueind[x][y][z];
            if (index == -1) return;
            _value[index] = value;
        }

        size_t size() const {
            return _incr;
        }
    };
}