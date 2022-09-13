#ifndef SENSOR_HPP // Sensor.hpp includes guard.
#define SENSOR_HPP

#include <iostream>
#include <common/HeartbeatSender.hpp>

class Sensor : public Common::Thread{

public:
    enum State_e {
        INIT,
        MEASURE,
        FAILURE,
        DEAD
    };

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
     * @param: id (string): String ID for our heartbeat sender.
     */
    Sensor(int sample_size, double scaling_factor, std::string id);

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
     * @return (float) scaled average measurement from all sensor samples.
     */
    float measure(int center, int offset);



    /**
     * 
     */
    std::string GetID() { return id; }

private:
    // common::thread
    void _run() override;
    State_e _getState();
    void _setState(State_e);

private:
    boost::chrono::milliseconds _init();
    boost::chrono::milliseconds _measure();
    boost::chrono::milliseconds _failure();
    
private:
    std::shared_ptr<Common::HeartbeatSender> _pHeartbeatSender;

    std::string id;

    State_e _state;

    int sampleVal;
    float measureVal;

    int numRuns;
};


#endif // !SENSOR_HPP
