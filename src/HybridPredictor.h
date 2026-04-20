#ifndef HYBRID_PREDICTOR_H
#define HYBRID_PREDICTOR_H

#include <cstdint>
#include <vector>
#include <ostream>

#include "BimodalPredictor.h"
#include "GsharePredictor.h"

class HybridPredictor {
private:
    int k;
    std::vector<int> chooserTable; // 2-bit counters: 0..3

    BimodalPredictor bimodal;
    GsharePredictor gshare;

    unsigned long long numPredictions;
    unsigned long long numMispredictions;

public:
    explicit HybridPredictor(int chooserBits, int gshareMBits, int gshareNBits, int bimodalMBits)
        : k(chooserBits),
          chooserTable(1u << chooserBits, 1),
          bimodal(bimodalMBits),
          gshare(gshareMBits, gshareNBits),
          numPredictions(0),
          numMispredictions(0) {}

    unsigned int getChooserIndex(uint32_t pc) const {
        unsigned int mask = (1u << k) - 1u;
        return (pc >> 2) & mask;
    }

    void updateChooser(unsigned int chooserIndex, bool gsharePred, bool bimodalPred, bool actualTaken) {
        if (gsharePred == bimodalPred) {
            return;
        }

        if (gsharePred == actualTaken && bimodalPred != actualTaken) {
            if (chooserTable[chooserIndex] < 3) {
                chooserTable[chooserIndex]++;
            }
        }
        else if (bimodalPred == actualTaken && gsharePred != actualTaken) {
            if (chooserTable[chooserIndex] > 0) {
                chooserTable[chooserIndex]--;
            }
        }
    }

    void processBranch(uint32_t pc, bool actualTaken) {
        bool gsharePred = gshare.predict(pc);
        bool bimodalPred = bimodal.predict(pc);

        unsigned int chooserIndex = getChooserIndex(pc);
        bool useGshare = chooserTable[chooserIndex] >= 2;
        bool finalPrediction = useGshare ? gsharePred : bimodalPred;

        numPredictions++;

        if (finalPrediction != actualTaken) {
            numMispredictions++;
        }

        if (useGshare) {
            gshare.updateCounterOnly(pc, actualTaken);
        } else {
            bimodal.updateCounterOnly(pc, actualTaken);
        }

        gshare.updateHistoryOnly(actualTaken);

        updateChooser(chooserIndex, gsharePred, bimodalPred, actualTaken);
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

    void printFinalContents(std::ostream& out) const {
        out << "FINAL CHOOSER CONTENTS\n";
        for (size_t i = 0; i < chooserTable.size(); i++) {
            out << i << "\t" << chooserTable[i] << "\n";
        }

        out << "FINAL GSHARE CONTENTS\n";
        gshare.printFinalContentsWithoutHeader(out);

        out << "FINAL BIMODAL CONTENTS\n";
        bimodal.printFinalContentsWithoutHeader(out);
    }
};

#endif
