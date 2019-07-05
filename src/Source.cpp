// Copyright 2019  Cosmin-Razvan Vancea


#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <algorithm>

#include "./SkipList.h"
#include "./Racer.h"
#include "./ResizableArray.h"

void increasePoints(ResizableArray<int>& racerScores,
                    SkipList<Racer>& racerRankings, int idx, int points) {
    auto node = racerRankings.find(Racer(idx, racerScores[idx]));
    auto racer = node->getData();
    racer.increasePoints(points);

    // ma asigur ca skiplistul ramane ordonat
    racerRankings.remove(node);
    racerRankings.insert(racer);

    racerScores[idx] += points;
}

void printRankings(std::ostream& out, SkipList<Racer>& racerRankings) {
    // printez clasamentul si actualizez fiecare nod astfel incat sa salvez
    // pozitia in clasament (pentru a putea face diferenta la urmatorul print)

    auto node = racerRankings.getBottomTail()->getPrev();
    while (node != racerRankings.getBottomHead()) {
        auto aux = node;
        out << node->getData();

        while (aux) {
            aux->getData().updatePrevRanking();
            aux = aux->getAbove();
        }

        node = node->getPrev();
    }
    out << std::endl;
}

void redoRankings(SkipList<Racer>& racerRankings) {
    // resetez pozitia fiecarui jucator in functie de pozitia din skiplist

    auto node = racerRankings.getBottomTail()->getPrev();
    int rank = 0;
    while (node != racerRankings.getBottomHead()) {
        auto aux = node;
        while (aux) {
            aux->getData().setRanking(rank);
            aux = aux->getAbove();
        }
        node = node->getPrev();
        rank++;
    }
}

int main() {
    std::ifstream fin("races.in");
    std::ofstream fout("races.out");

    if (!fin.is_open() || !fout.is_open()) {
        std::cerr << "Error: Can't open files!" << std::endl;
        return -1;
    }

    int n, ignoreInt;
    fin >> n >> ignoreInt >> ignoreInt;
    SkipList<Racer> racerRankings(Racer::getMin(), Racer::getMax());
    ResizableArray<int> racerScores(n);
    for (int i = 0; i != n; ++i) {
        racerRankings.insert(Racer(i));
        racerScores[i] = 0;
    }

    // array ce retine timpul, pozitia si ID-ul pt fiecare jucator
    // folosit pentru acordarea punctajelor in functie de timp si loc anterior
    // int1: timpul in cursa curenta
    // int2: locul din clasamentul actual (inca nemodificat de cursa curenta)
    // int3: ID-ul jucatorului
    using entry_t = std::tuple<int, int, int>;
    ResizableArray<entry_t> times(n);

    std::string test_string;
    while (fin >> test_string) {
        if (test_string == "print") {
            printRankings(fout, racerRankings);
        } else {
            times.clear();

            for (int i = 0; i < n; ++i) {
                int time;

                if (i == 0) {
                    time = std::stoi(test_string);
                } else {
                    fin >> time;
                }

                if (time != 0) {
                    auto needle = Racer(i, racerScores[i]);
                    auto node = racerRankings.find(needle);

                    int lastPlacing = node->getData().getRanking();
                    times.addLast(std::make_tuple(time, lastPlacing, i));
                }
            }

            // sortare
            auto sortPred = [](const entry_t& a, const entry_t& b) -> bool {
                int time[2], lastPlacing[2], idx;
                std::tie(time[0], lastPlacing[0], idx) = a;
                std::tie(time[1], lastPlacing[1], idx) = b;

                if  (time[0] > time[1] ||
                    (time[0] == time[1] && lastPlacing[0] > lastPlacing[1])) {
                    return 0;
                }
                return 1;
            };
            std::sort(times.getData(), times.getData()+times.size(), sortPred);

            // acord punctajele in functie de timpi si
            // (ca depepartajare) de pozitia din clasamentul anterior
            for (int i = 0; i < times.size(); ++i) {
                int points = times.size() / 2 - i;

                // daca au fost un numar par de jucatori, nu acord punctaj 0
                if (points <= 0 && (times.size() & 1) == 0) {
                    points--;
                }

                increasePoints(racerScores, racerRankings,
                               std::get<2>(times[i]), points);
            }

            // este posibil ca skiplistul sa se fi modificat, deci trebuie sa
            // resetez pozitiile fiecarui jucator
            redoRankings(racerRankings);
        }
    }
    return 0;
}
