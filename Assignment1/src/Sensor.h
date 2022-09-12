#ifndef SENSOR_H // Sensor.h includes guard.
#define SENSOR_H

class Sensor {
public: 
    // Number of samples taken (and averaged) to get a measurement.
    int sample_size;

    // Scaling factor to apply to sensor measurements to get real result.
    double scaling_factor;

    /* 
     * @constructor.
     * 
     * @param: sample_size (int): Number of samples to average to get measurement.
     * @param: scaling_factor (double): Factor to use when scaling sensor measurements.
     */
    Sensor(int sample_size, double scaling_factor);

    /*
     * @brief: Take a single random sample (measurement) of the sensor.
     *
     * @param: center (int): The value samples should be centered around.
     * @param: offset (int): The max distance away from center for measurement.
     *
     * @return (int) random sample (unscaled) from sensor, range [center - offset, center + offset].
     */
    int sample(int center, int offset);

    /*
     * @brief: Take multiple sample measurements to get average and scale to get real value.
     *
     * @param: center (int): The value samples should be centered around.
     * @param: offset (int): The max distance away from center for measurement.
     *
     * @return (double) scaled average measurement from all sensor samples.
     */
    double measure(int center, int offset);
};


#endif // !SENSOR_H
