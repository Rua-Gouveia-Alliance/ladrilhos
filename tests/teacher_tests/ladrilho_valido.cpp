#include <iostream>

using namespace std;

// Dimensions
int _N;
int _M;

//-------------------------------------------------------------------

void printUsage(char *progname) {
	cout << "Usage: " << progname << " #N #M seed" << endl;
	cout << "\t#N: number of lines" << endl;
	cout << "\t#M: number of columns" << endl;
	cout << "\tseed: random seed number (optional)" << endl;
	exit(0);
}

void parseArgs(int argc, char **argv) {
  int seed = 0;

  if (argc < 3 || argc > 4) {
		cout << "ERROR: invalid # arguments" << endl;
		printUsage(argv[0]);
	}

  sscanf(argv[1], "%d", &_N);
  if (_N < 0) {
    cout << "ERROR: #N lines must be >= 0" << endl;
    printUsage(argv[0]);
  }

  sscanf(argv[2], "%d", &_M);
  if (_M < 0) {
    cout << "ERROR: #M columns must be >= 0" << endl;
    printUsage(argv[0]);
  }

  if (argc == 4) { // Init rand seed
    sscanf(argv[3], "%d", &seed);
    srand(seed);
  } else { // pseudo-rand seed
    srand((unsigned int)time(NULL));
  }
}

inline int randomValue(int max) {
  return rand() % max; // [0, max - 1]
}

int main(int argc, char *argv[]) {
  // parse arguments
  parseArgs(argc, argv);

	// print header
	cout << _N << endl << _M << endl;

	// print c_i's
	for (int i = 0, last=0; i < _N; i++) {
		last = last + randomValue(_M+1 - last);
		cout << last << endl;
	}
	return 0;
}
