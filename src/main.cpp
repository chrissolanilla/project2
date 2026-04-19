#include <iostream>
#include <string>
#include <fstream>

int main(int argc, char *argv[]) {
	// std::cout << "hello" << "\n";
	// const int default_runs = 10;
	bool bimodal = false;
	bool gshare = false;
    bool hybrid = false;
    int chooserTableK = 0;
    int numPCBits = 0;
    int registerBits = 0;
    int numBimodalPCBits = 0;
    std::string traceFile = "";

	if(argc < 2) {
		std::cout << "Please provide arugments" << "\n";
		std::cout << "To run bimodal: ./sim bimodal <M2> <tracefile>    (M2 = PC bits)" << "\n";
		std::cout << "To run gshare: ./sim gshare <M1> <N> <tracefile>    (M1 = PC bits, N = global branch history register bits)" << "\n";
        std::cout << "To run hybrid: ./sim hybrid <K> <M1> <N> <M2> <tracefile>   (K= number of PC bits used to index the chooser table, M1 = number PC bits, N global history register bits, M2 = number PC bits for bimdal table)" << "\n";
	}

	if(argc >= 2) {
		// std::cout << "argument count: " << argc << "\n";
        std::string arg = argv[1];
        if(arg == "bimodal") {
            bimodal = true;
        }
        else if(arg == "gshare") {
            gshare = true;
        }
        else if(arg == "hybrid") {
            hybrid = true;
        }
        if(!bimodal && !gshare && !hybrid) {
            std::cout << "Error: please provide either bimodal, hybrid, or gshare as the first argument" << "\n";
            std::cout << "Your argument was: " << arg << "\n";
            return 1;
        }

        if(bimodal) {
            if(argc != 4) {
                std::cout << "ERROR: Bimodal requires 3 arguments" << "\n";
		        std::cout << "To run Bimodal: ./sim bimodal <M2> <tracefile>    (M2 = PC bits)" << "\n";
                return 1;
            }
            for(int i=0;i<2;i++) {
                std::string arg = argv[i+2];
                std::cout <<" arg is: " << arg << "\n";
            }
            numPCBits = atoi(argv[2]);
            std::cout << "numPCBits is " << numPCBits << "\n";
            traceFile = argv[3];
            std::cout << "traceFile is " << traceFile << "\n";
        }
        else if(gshare) {
            if(argc != 5) {
                std::cout << "ERROR: gshare requires 4 arguments" << "\n";
		        std::cout << "To run gshare: ./sim gshare <M2> <N> <tracefile>    (M1 = PC bits, N = global branch history register bits)" << "\n";
                return 1;
            }
            for(int i=0;i<3;i++) {
                std::string arg = argv[i+2];
                std::cout <<" arg is: " << arg << "\n";
            }
            numPCBits = atoi(argv[2]);
            std::cout << "numPCBits is " << numPCBits << "\n";
            registerBits = atoi(argv[3]);
            std::cout << "registerBits is " << registerBits << "\n";
            traceFile = argv[4];
            std::cout << "traceFile is " << traceFile << "\n";
        }
        else if(hybrid) {
            if (argc != 6) {
                std::cout << "ERROR: hybrid requires 5 arguments" << "\n";
                std::cout << "To run hybrid: ./sim hybrid <K> <M1> <N> <M2> <tracefile>   (K= number of PC bits used to index the chooser table, M1 = number PC bits, N global history register bits, M2 = number PC bits for bimdal table)" << "\n";
                return 1;
            }
            for(int i=0;i<4;i++) {
                std::string arg = argv[i+2];
                std::cout <<" arg is: " << arg << "\n";
            }
            chooserTableK = atoi(argv[2]);
            std::cout << "chooserTableK is " << chooserTableK << "\n";
            numPCBits = atoi(argv[3]);
            std::cout << "numPCBits is " << numPCBits << "\n";
            registerBits = atoi(argv[4]);
            std::cout << "registerBits is " << registerBits << "\n";
            numBimodalPCBits = atoi(argv[5]);
            std::cout << "numBimodalPCBits is " << numBimodalPCBits << "\n";
            traceFile = argv[6];
            std::cout << "traceFile is " << traceFile << "\n";
        }

        if(traceFile == "") {
            std::cout << "ERROR: tracefile argument is missing" << "\n";
            std::cout << "tracefile: " << traceFile << "\n";
            return 1;
        }
        std::ifstream file(traceFile);
        std::string line;
        if(file.is_open()) {
            while(std::getline(file, line)) {
                std::cout << line << "\n";
            }
            file.close();
        }
        else  {
            std::cerr << "Unable to open file";
        }

	}
	return 0;
}
