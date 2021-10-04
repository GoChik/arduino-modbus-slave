#include <stdlib.h>

#define INPUTS_SIZE 1
#define OUTPUTS_SIZE 4

namespace {
    const unsigned long BAUDRATE {19200};
    const int SLAVE_ID{2};

    const int TX_PIN {1};
    const int OUTPUT_ENABLE_PIN {3};
    const int RECEIVER_ENABLE_PIN {2};

    const int OUTPUTS[OUTPUTS_SIZE] {
        14,
        15,
        16,
        17,
    };

    const int INPUTS[INPUTS_SIZE] {
        5,
    };

    const int OUTPUT_INVERTED = 1;
}