#include <iostream>

int main(int argc, char *argv[]) {
	// std::cout << "hello" << "\n";
	const int default_runs = 10;
	bool bimodal = false;
	bool gshare = false;
	if(argc < 2) {
		std::cout << "Please provide arugments" << "\n";
		std::cout << "To run Bimodal: ./sim bimodal <M2> <tracefile>    (M2 = PC bits)" << "\n";
		std::cout << "To run gshare: ./sim gshare <M2> <N> <tracefile>    (M1 = PC bits, N = global branch history register bits)" << "\n";
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
        if(!bimodal && !gshare) {
            std::cout << "Error: please provide either bimodal or gshare as the first argument" << "\n";
            std::cout << "Your argument was: " << arg << "\n";
            return 1;
        }

        if(bimodal) {
            if(argc != 4) {
                std::cout << "Bimodal requires 3 arguments" << "\n";
		        std::cout << "To run Bimodal: ./sim bimodal <M2> <tracefile>    (M2 = PC bits)" << "\n";
                return 1;
            }
            for(int i=0;i<2;i++) {
                std::string arg = argv[i+2];
                std::cout <<" arg is: " << arg << "\n";
            }
        }
        else if(gshare) {
            if(argc != 5) {
                std::cout << "gshare requires 4 arguments" << "\n";
		        std::cout << "To run gshare: ./sim gshare <M2> <N> <tracefile>    (M1 = PC bits, N = global branch history register bits)" << "\n";
                return 1;
            }
            for(int i=0;i<3;i++) {
                std::string arg = argv[i+2];
                std::cout <<" arg is: " << arg << "\n";
            }
        }
	}
	return 0;
}
