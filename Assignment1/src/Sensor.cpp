#include <stdlib.h>   //rand
#include <Sensor.hpp>
#include <time.h>

namespace bc = boost::chrono;


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

    _pHeartbeatSender = std::make_shared<Common::HeartbeatSender>(id, messageQueue);
}


int Sensor::sample(int center, int offset) {
    // Center the measurement along the provided center.
    int res = center;

    // Offset the measurement randomly in the range [-offset, offset].
    res += rand() % (2 * offset + 1) - offset;   // NOTE: rand() is 0 to RAND_MAX.

    return res;
}


float Sensor::measure(int center, int offset) {
    int rolling_sum = 0;
    float avg_sample = 0;

    // Take number of samples with rolling sum to calc average.
    for (int i = 0; i < Sensor::sample_size; i++) {
        rolling_sum += Sensor::sample(center, offset);
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

void Sensor::_run() {
    do
    {
        bc::milliseconds nextSleepTime;
        switch(_getState())
        {
        case INIT:
            nextSleepTime = _init();
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
        boost::this_thread::sleep_for(nextSleepTime);
    } while (!_getShutdown());

    _setState(DEAD);
}

bc::milliseconds Sensor::_init()
{
    _pHeartbeatSender->start();
    _setState(MEASURE);
    return bc::milliseconds(0);
}

bc::milliseconds Sensor::_measure()
{
    sampleVal = sample(3, 10);
    measureVal = measure(3, 10);

    std::cout << "Sample Value: " << sampleVal << " Measure Value: " << measureVal << std::endl;
    numRuns++;

    if(sampleVal == measureVal)
    {
        _setState(FAILURE);
        return bc::milliseconds(0);
    }
    return bc::milliseconds(0);
}

bc::milliseconds Sensor::_failure()
{
    std::cout << "Num Runs: " << numRuns << std::endl;
    _pHeartbeatSender->end();
    _setState(DEAD);
    return bc::milliseconds(0);
}
