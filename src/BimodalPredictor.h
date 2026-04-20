#ifndef BIMODAL_PREDICTOR_H
#define BIMODAL_PREDICTOR_H

#include <cstdint>
#include <vector>
#include <ostream>

class BimodalPredictor {
private:
    int m;
    std::vector<int> table;
    unsigned long long numPredictions;
    unsigned long long numMispredictions;

public:
    //the table is built to be off of powers of 2. unsigned
    //all entries initalized with value of 4
    explicit BimodalPredictor(int mBits)
        : m(mBits),
          table(1u << mBits, 4),
          numPredictions(0),
          numMispredictions(0) {}

    unsigned int getIndex(uint32_t pc) const {
        unsigned int mask = (1u << m) - 1u;
        return (pc >> 2) & mask;
    }

    bool predict(uint32_t pc) const {
        unsigned int index = getIndex(pc);
        return table[index] >= 4;
    }


    void update(unsigned int index, bool actualTaken) {
        if(actualTaken) {
            if (table[index] < 7) {
                table[index]++;
            }
        }
        else {
            if(table[index] > 0) {
                table[index]--;
            }
        }
    }

    void updateCounterOnly(uint32_t pc, bool actualTaken) {
        unsigned int index = getIndex(pc);
        update(index, actualTaken);
    }

    void processBranch(uint32_t pc, bool actualTaken) {
        unsigned int index = getIndex(pc);
        bool predictedTaken = (table[index] >= 4);

        numPredictions++;

        if(predictedTaken != actualTaken) {
            numMispredictions++;
        }

        update(index, actualTaken);
    }

    unsigned long long getNumPredictions() const {
        return numPredictions;
    }

    unsigned long long getNumMispredictions() const {
        return numMispredictions;
    }

    double getMispredictionRate() const {
        if(numPredictions == 0) {
            return 0.0;
        }
        return 100.0 * static_cast<double>(numMispredictions)
                     / static_cast<double>(numPredictions);
    }

    //need this for using it in HybridPredictor
    void printFinalContentsWithoutHeader(std::ostream& out) const {
        for (size_t i = 0; i < table.size(); i++) {
            out << i << "\t" << table[i] << "\n";
        }
    }

    void printFinalContents(std::ostream& out) const {
        out << "FINAL BIMODAL CONTENTS\n";
        printFinalContentsWithoutHeader(out);
    }
};

#endif
