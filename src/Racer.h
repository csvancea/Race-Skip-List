// Copyright 2019  Cosmin-Razvan Vancea

#ifndef RACER_H_
#define RACER_H_

#include <ostream>
#include <limits>

class Racer {
 public:
    explicit Racer(int id) :
        _id(id), _currRank(id), _currPoints(0), _prevRank(id), _prevUpdated(0)
    { }

    Racer(int id, int points) :
        _id(id), _currRank(id), _currPoints(points), _prevRank(id), _prevUpdated(0)
    { }

    int updatePrevRanking() {
        _prevUpdated = 1;
        return _prevRank = _currRank;
    }
    int getRanking() const { return _currRank; }
    int setRanking(int rank) { return _currRank = rank; }

    int getId() const { return _id; }

    int getCurrentPoints() const { return _currPoints; }
    int increasePoints(int points) { return _currPoints += points; }

    // implementarea skiplist-ului necesita urmatorii operatori implementati
    // as fi putut implementa doar `<`, dar ar fi complicat codul pt skiplist
    bool operator< (const Racer& other) {
        // ordonez crescator dupa numarul de puncte
        if (_currPoints < other._currPoints)
            return 1;

        // la puncte egale, ordonez descrescator dupa ID
        if (_currPoints == other._currPoints && _id > other._id)
            return 1;

        return 0;
    }
    bool operator== (const Racer& other) {
        return _currPoints == other._currPoints && _id == other._id;
    }
    bool operator!= (const Racer& other) {
        return !(*this == other);
    }
    bool operator<= (const Racer& other) {
        return *this < other || *this == other;
    }

    // initializarea skiplist-ul necesita 2 valori ce vor fi considerate
    // valorile extreme intre care se plimba datele introduse in skiplist

    // aceste metode produc acele valori
    static const Racer& getMin() {
        int min = std::numeric_limits<int>::min();
        static Racer r(min, min);
        return r;
    }

    static const Racer& getMax() {
        int max = std::numeric_limits<int>::max();
        static Racer r(max, max);
        return r;
    }

    friend std::ostream& operator<< (std::ostream& out, const Racer& racer) {
        int diffRank = 0;
        if (racer._prevUpdated) {
            // se calculeaza diferenta de clasament doar
            // daca s-a actualizat anterior pozitia
            diffRank = racer._prevRank - racer._currRank;
        }

        out << racer._id + 1 << " ";
        out << racer._currPoints << " ";
        out << diffRank << std::endl;

        return out;
    }

 private:
    int _id;
    int _currRank;
    int _currPoints;
    int _prevRank;
    bool _prevUpdated;  // retine daca s-a actualizat vreodata pozitia
                        // necesara la afisarea diferentei de clasament
};

#endif  // RACER_H_
