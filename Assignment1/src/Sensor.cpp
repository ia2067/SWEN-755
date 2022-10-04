#include <stdlib.h>   //rand
#include <Sensor.hpp>
#include <time.h>

namespace Assignment1
{

Sensor::Sensor(int sample_size, float scaling_factor, 
               std::string messageQueue, std::string id)
: _pHeartbeatSender(nullptr),
  _state(INIT),
  sampleVal(0),
  measureVal(0),
  numRuns(0)
{
    srand(time(NULL));

    Sensor::sample_size = sample_size;
    Sensor::scaling_factor = scaling_factor;
    Sensor::id = id;
    Sensor::messageQueue = messageQueue;

    _pHeartbeatSender = std::make_shared<Heartbeat::Sender>(id, messageQueue);
}


int Sensor::sample(int center, int offset) {
    // Center the measurement along the provided center.
    int res = center;

    // Offset the measurement randomly in the range [-offset, offset].
    res += rand() % (2 * offset + 1) - offset;   // NOTE: rand() is 0 to RAND_MAX.

    _cacheSample(res);

    return res;
}


float Sensor::measure() {
    int rolling_sum = 0;
    float avg_sample = 0;

    {
        std::lock_guard<std::mutex> lock(_mutex);
        // Take number of previous samples with rolling sum to calc average.
        for(auto samp : _prevSamples)
        {
            rolling_sum += samp;
        }
    }


    // Get Average based on rolling sum.
    return rolling_sum / (float) Sensor::sample_size * Sensor::scaling_factor;
}

Sensor::State_e Sensor::_getState()
{
    return _state;
}

void Sensor::_setState(State_e state)
{
    _state = state;
}
void Sensor::_cacheSample(int newSample)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _prevSamples.push_back(newSample);
    while(_prevSamples.size() >= Sensor::sample_size)
    {
        _prevSamples.pop_front();
    }
}

void Sensor::_run() {
    do
    {
        std::chrono::milliseconds nextSleepTime;
        switch(_getState())
        {
        case INIT:
            nextSleepTime = _init();
            break;
        case PREFILL:
            nextSleepTime = _prefill();
            break;
        case MEASURE:
            nextSleepTime = _measure();
            break;
        case FAILURE:
            nextSleepTime = _failure();
            break;
        default:
            break;
        }
        std::this_thread::sleep_for(nextSleepTime);
    } while (!_getShutdown());

    _setState(DEAD);
}

std::chrono::milliseconds Sensor::_init()
{
    _pHeartbeatSender->start();
    _setState(PREFILL);
    return std::chrono::milliseconds(0);
}

std::chrono::milliseconds Sensor::_prefill()
{
    for(int i = 0 ; i < Sensor::sample_size; i++)
    {
        sample(20,20);
    }
    
    _setState(MEASURE);
    return std::chrono::milliseconds(0);
}

std::chrono::milliseconds Sensor::_measure()
{
    sampleVal = sample(20, 20);
    measureVal = measure();

    // std::cout << "Sample Value: " << sampleVal << " Measure Value: " << measureVal << std::endl;
    // numRuns++;
    float percent_error = 0;

    if(measureVal > 0)
        percent_error = std::abs((sampleVal - measureVal)/ measureVal) * 100;

    // std::cout << "\% err(" << Sensor::id << "): " << std::setprecision(5) <<  percent_error << std::endl;
    if(percent_error >= 100)
    {
        _setState(FAILURE);
        return std::chrono::milliseconds(0);
    }
    return std::chrono::milliseconds(500);
}

std::chrono::milliseconds Sensor::_failure()
{
    // std::cout << "Num Runs: " << numRuns << std::endl;
    std::cout << "SENSOR FAILURE: " << Sensor::id << std::endl;
    _pHeartbeatSender->end();
    _setState(DEAD);
    return std::chrono::milliseconds(0);
}

    
} // Assignment1