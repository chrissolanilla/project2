#ifndef GSHARE_PREDICTOR_H
#define GSHARE_PREDICTOR_H

#include <cstdint>
#include <vector>
#include <ostream>

class GsharePredictor {
private:
    int m;
    int n;
    unsigned int globalHistory;
    std::vector<int> table;
    unsigned long long numPredictions;
    unsigned long long numMispredictions;

public:
    //table size is still exponential but only the lowest 4 bits are xored in the register
    explicit GsharePredictor(int mBits, int registerBits)
        : m(mBits),
          n(registerBits),
          globalHistory(0),
          table(1u << mBits, 4),
          numPredictions(0),
          numMispredictions(0) {}

    //we need to xor the lowest n bits of the index this time
    unsigned int getIndex(uint32_t pc) const {
        unsigned int pcMask = (1u << m) - 1u;
        unsigned int pcIndex = (pc >> 2) & pcMask;

        if (n == 0) {
            return pcIndex;
        }

        unsigned int lowerNMask = (1u << n) - 1u;
        unsigned int lowerPCBits = pcIndex & lowerNMask;
        unsigned int upperPCBits = pcIndex & ~lowerNMask;

        unsigned int xoredLowerBits = lowerPCBits ^ globalHistory;
        //final index i just top + the new bottom
        return upperPCBits | xoredLowerBits;
    }

    bool predict(uint32_t pc) const {
        unsigned int index = getIndex(pc);
        return table[index] >= 4;
    }

    void updateCounter(unsigned int index, bool actualTaken) {
        if (actualTaken) {
            if (table[index] < 7) {
                table[index]++;
            }
        } else {
            if (table[index] > 0) {
                table[index]--;
            }
        }
    }

    void updateCounterOnly(uint32_t pc, bool actualTaken) {
        unsigned int index = getIndex(pc);
        updateCounter(index, actualTaken);
    }

    void updateGlobalHistory(bool actualTaken) {
        if (n == 0) {
            return;
        }

        globalHistory >>= 1;

        if (actualTaken) {
            globalHistory |= (1u << (n - 1));
        }

        globalHistory &= (1u << n) - 1u;
    }

    void updateHistoryOnly(bool actualTaken) {
        updateGlobalHistory(actualTaken);
    }

    void processBranch(uint32_t pc, bool actualTaken) {
        unsigned int index = getIndex(pc);
        bool predictedTaken = (table[index] >= 4);

        numPredictions++;

        if (predictedTaken != actualTaken) {
            numMispredictions++;
        }

        updateCounter(index, actualTaken);
        updateGlobalHistory(actualTaken);
    }

    unsigned long long getNumPredictions() const {
        return numPredictions;
    }

    unsigned long long getNumMispredictions() const {
        return numMispredictions;
    }

    double getMispredictionRate() const {
        if (numPredictions == 0) {
            return 0.0;
        }

        return 100.0 * static_cast<double>(numMispredictions)
                     / static_cast<double>(numPredictions);
    }

    void printFinalContentsWithoutHeader(std::ostream& out) const {
        for (size_t i = 0; i < table.size(); i++) {
            out << i << "\t" << table[i] << "\n";
        }
    }

    void printFinalContents(std::ostream& out) const {
        out << "FINAL GSHARE CONTENTS\n";
        printFinalContentsWithoutHeader(out);
    }
};

#endif
