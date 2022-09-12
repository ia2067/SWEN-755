#include <stdlib.h>   //rand
#include "Sensor.h"


Sensor::Sensor(int sample_size, double scaling_factor) {
    Sensor::sample_size = sample_size;
    Sensor::scaling_factor = scaling_factor;
}


int Sensor::sample(int center, int offset) {
    // Center the measurement along the provided center.
    int res = center;

    // Offset the measurement randomly in the range [-offset, offset].
    res += rand() % (2 * offset + 1) - offset;   // NOTE: rand() is 0 to RAND_MAX.

    return res;
}


double Sensor::measure(int center, int offset) {
    int rolling_sum = 0;
    double avg_sample = 0;

    // Take number of samples with rolling sum to calc average.
    for (int i = 0; i < Sensor::sample_size; i++) {
        rolling_sum += Sensor::sample(center, offset);
    }

    // Get Average based on rolling sum.
    return rolling_sum / (double) Sensor::sample_size * Sensor::scaling_factor;
}
