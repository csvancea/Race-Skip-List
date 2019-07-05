// Copyright 2019  Cosmin-Razvan Vancea

#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <ostream>
#include <limits>
#include <random>

template <typename T>
class SkipList {
 public:
    class Node {
 private:
        T _data;
        Node *_below, *_above, *_next, *_prev;

        // constructor privat, doar SkipList are voie sa creeze noduri
        explicit Node(T data) :
            _data(data), _below(nullptr), _above(nullptr),
            _next(nullptr), _prev(nullptr)
        { }

 public:
        T& getData() { return _data; }
        Node *getBelow() const { return _below; }
        Node *getAbove() const { return _above; }
        Node *getNext() const {    return _next; }
        Node *getPrev() const { return _prev; }
        friend class SkipList;
    };

    SkipList(T min = std::numeric_limits<T>::min(),
             T max = std::numeric_limits<T>::max()) :
        _min(min), _max(max), _head(nullptr), _tail(nullptr),
        _bottomHead(nullptr), _bottomTail(nullptr), _levels(0),
        _randEngine(std::random_device{}())
    {
        // creez primul nivel
        createNewLevel();
    }

    ~SkipList() {
        // se parcurge lista de la stanga la dreapta, pe ultimul nivel
        // se sterge fiecare nod, de jos in sus
        // nu se incearca refacerea legaturilor

        Node *aux = _bottomHead;
        while (aux) {
            Node *next = aux->_next;
            internalRemove(aux, false);
            aux = next;
        }
    }

    // nu permit copy-constructor sau copy-assignment
    SkipList(const SkipList&) = delete;
    SkipList& operator=(const SkipList&) = delete;

    // cauta `data` in skiplist
    //   daca exista:
    //     se returneaza nodul care contine `data` de pe nivelul cel mai de jos
    //   daca nu exista:
    //     se returneaza nodul dupa care ar trebui adaugat `data` ca lista sa
    //     ramana in ordine
    Node *find(const T& data) const {
        Node *node = _head;

        while (node->_data != data) {
            if (node->_next->_data <= data) {
                // cautare pe orizontala
                node = node->_next;
            } else if (node->_below) {
                // avansare pe verticala in jos
                node = node->_below;
            } else {
                // daca se ajunge aici, inseamna ca s-a ajuns la nivelul 0,
                // iar nodul din dreapta este mai mare decat ce se cauta
                return node;
            }
        }

        // avansez spre nodul cel mai de jos
        while (node->_below) {
            node = node->_below;
        }

        return node;
    }

    // insereaza un nod si returneaza un pointer la el (nivelul cel mai de jos)
    // daca `data` exista deja in lista, nu permite adaugarea
    Node *insert(const T& data) {
        int currLevel = 1;
        Node *below = nullptr;
        Node *prev = nullptr;
        Node *ret = nullptr;

        do {
            if (_levels < currLevel) {
                createNewLevel();
            }

            if (prev == nullptr) {
                prev = find(data);
                if (prev->_data == data) {
                    // daca `data` exista deja in lista,
                    // nu permite adaugarea inca o data
                    return nullptr;
                }
            } else {
                // daca se intra aici, inseamna ca este garantat ca
                // mai exista cel putin un nivel deasupra acestui nod
                while (!prev->_above) {
                    prev = prev->_prev;
                }
                prev = prev->_above;
            }

            Node *node = new Node(data);
            node->_next = prev->_next;
            node->_prev = prev;
            node->_below = below;
            prev->_next->_prev = node;
            prev->_next = node;
            if (below) {
                below->_above = node;
            }
            below = node;

            if (!ret) {
                ret = node;
            }
            currLevel++;
        } while (_randEngine() & 1);

        return ret;  // 1st level node
    }

    // sterge un nod si toate cele de deasupra lui, refacand legaturile
    void remove(Node *bottomNode) {
        internalRemove(bottomNode, true);
    }

    friend std::ostream& operator<< (std::ostream& out, const SkipList& list) {
        Node *aux = list._bottomHead->getNext();
        while (aux != list._bottomTail) {
            out << aux->getData() << " ";
            aux = aux->getNext();
        }
        return out;
    }

    Node *getBottomHead() const { return _bottomHead; }
    Node *getBottomTail() const { return _bottomTail; }

 private:
    void createNewLevel() {
        Node *node;

        // creeaza nodul -inf / head
        node = new Node(_min);
        node->_below = _head;
        if (_head) {
            _head->_above = node;
        }
        _head = node;

        // creeaza nodul +inf / tail
        node = new Node(_max);
        node->_below = _tail;
        if (_tail) {
            _tail->_above = node;
        }
        _tail = node;

        _head->_next = _tail;
        _tail->_prev = _head;
        _levels++;

        if (_levels == 1) {
            _bottomHead = _head;
            _bottomTail = _tail;
        }
    }

    void internalRemove(Node *bottomNode, bool redoNextPrevLinks) {
        Node *node = bottomNode;
        while (node) {
            if (redoNextPrevLinks) {
                // se intra pe aceasta ramura doar in cazul nodurilor inserate
                // cu `insert`, deci nodurile `_prev` si `_next` exista mereu

                node->_prev->_next = node->_next;
                node->_next->_prev = node->_prev;
            }

            Node *next = node->_above;
            delete node;
            node = next;
        }
    }

 private:
    T _min;  // valoarea ce va fi considerata -inf
    T _max;  // valoarea ce va fi considerata +inf

    Node *_head;  // nodul stanga-sus
    Node *_tail;  // nodul dreapta-sus
    Node *_bottomHead;  // nodul stanga-jos
    Node *_bottomTail;  // nodul dreapta-jos

    int _levels;  // numarul de nivele (minim 1)

    std::mt19937 _randEngine;  // pentru generarea aleatoare a nivelelor
};

#endif  // SKIPLIST_H_
