#ifndef SENSOR_HPP // Sensor.hpp includes guard.
#define SENSOR_HPP

#include <iostream>
#include <mutex>
#include <list>

#include <heartbeat/Sender.hpp>

namespace Assignment1
{

/**
 * @brief A Sensor
 * 
 */
class Sensor : public Core::Thread{

public:
/**
 * @brief The States of a Sensor 
 * 
 */
    enum State_e {
        INIT,
        PREFILL,
        MEASURE,
        FAILURE,
        DEAD
    };

public: 
    /**
     * @brief Number of samples taken (and averaged) to get a measurement.
     */
    int sample_size;

    /**
     * @brief Scaling factor to apply to sensor measurements to get real result.
     * 
     */
    float scaling_factor;

    /**
     * @brief Construct a new Sensor object
     * 
     * @param sample_size (int): Number of samples to average to get measurement.
     * @param scaling_factor (float): Factor to use when scaling sensor measurements.
     * @param messageQueue (string): String ID for our message queue.
     * @param id (string): String ID for our heartbeat sender.
     */
    Sensor(int sample_size, float scaling_factor, 
           std::string messageQueue, std::string id);

    /**
     * @brief: Take a single random sample (measurement) of the sensor.
     *
     * @param: center (int): The value samples should be centered around.
     * @param: offset (int): The max distance away from center for measurement.
     *
     * @return (int) random sample (unscaled) from sensor, range [center - offset, center + offset].
     */
    int sample(int center, int offset);

    /**
     * @brief: Take multiple sample measurements to get average and scale to get real value.
     *
     * @param: center (int): The value samples should be centered around.
     * @param: offset (int): The max distance away from center for measurement.
     *
     * @return (float) scaled average measurement from all sensor samples.
     */
    float measure();

private:
    // Core::Thread
    /**
     * @brief Sensor's thread, a state machine loop
     * 
     */
    void _run() override;

    /**
     * @brief Returns the current state of the Sensor State Machien
     * 
     * @return State_e The current state of the State Machine
     */
    State_e _getState();

    /**
     * @brief set the next state of the state machine
     * 
     */
    void _setState(State_e);

private:
    /**
     * @brief cache an individual sample, adds it to the trailing list of 10 samples
     * 
     */
    void _cacheSample(int);

private:
    /**
     * @brief Function for the init state
     * 
     * @return std::chrono::milliseconds milliseconds to wait before next state
     */
    std::chrono::milliseconds _init();

    /**
     * @brief Function for the prefill state
     * 
     * @return std::chrono::milliseconds milliseconds to wait before next state
     */
    std::chrono::milliseconds _prefill();

    /**
     * @brief Function for the measure state
     * 
     * @return std::chrono::milliseconds milliseconds to wait before next state
     */
    std::chrono::milliseconds _measure();

    /**
     * @brief Function for the failure state
     * 
     * @return std::chrono::milliseconds milliseconds to wait before next state
     */
    std::chrono::milliseconds _failure();
    
private:
    /**
     * @brief Mutex to be used for thread-safety
     * 
     */
    std::mutex _mutex;

    /**
     * @brief The child thread which is responsible for sending out heart beats
     * 
     */
    std::shared_ptr<Heartbeat::Sender> _pHeartbeatSender;

    /**
     * @brief the ID of this Sensor
     * 
     */
    std::string id;

    /**
     * @brief The name of the messaging queue this sensor belongs too (for Heart Beats)
     * 
     */
    std::string messageQueue;

    /**
     * @brief The current state
     * 
     */
    State_e _state;

    int sampleVal;
    float measureVal;

    /**
     * @brief The previous 10 samples taken
     * 
     */
    std::list<int> _prevSamples;

    int numRuns;
};

    
} // namespace Assignment1

#endif // !SENSOR_HPP
