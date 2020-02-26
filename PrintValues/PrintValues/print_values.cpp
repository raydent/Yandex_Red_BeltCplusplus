
#include "test_runner.h"
#include <ostream>
using namespace std;

/*#define PRINT_VALUES(out, x, y) { \
    out << (x) << endl;\
    out << (y) << endl;\
}*/
#define PRINT_VALUES(out, x, y) out << (x) << endl << (y) << endl


int main() {
    TestRunner tr;
    tr.RunTest([] {
        ostringstream output;
        int x = 1;
        if (false)
            PRINT_VALUES(output, 1, 0);
        else {
            return 0;
        }
        ASSERT_EQUAL(output.str(), "1\n0\n");
        }, "PRINT_VALUES usage example");
}