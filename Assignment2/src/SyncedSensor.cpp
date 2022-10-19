#include <stdlib.h>   //rand
#include <SyncedSensor.hpp>
#include <time.h>
#include <boost/bind/bind.hpp>

namespace Assignment2
{

Sensor::Sensor(int sample_size, float scaling_factor, 
               std::string hbQueueName, std::string fhQueueName, std::string id,
               std::shared_ptr<Sync::Sender> syncSender)
: _pHeartbeatSender(nullptr),
  _pSyncSender(syncSender),
  _pSyncReceiver(nullptr),
  _state(INIT),
  sampleVal(0),
  measureVal(0),
  numRuns(0), 
  _syncCounter(0)
{
    srand(time(NULL));

    Sensor::sample_size = sample_size;
    Sensor::scaling_factor = scaling_factor;
    Sensor::id = id;

    _pHeartbeatSender = std::make_shared<Heartbeat::Sender>(id, hbQueueName);

    _pHandleServerfault = std::make_shared<FaultHandle::Server>(fhQueueName);
    _pHandleServerfault->sigWakeUp.connect(boost::bind(&Sensor::handleWakeup, this));
    _pHandleServerfault->sigGetData.connect(boost::bind(&Sensor::handleGetData, this));
}

Sensor::Sensor(int sample_size, float scaling_factor, 
               std::string hbQueueName, std::string fhQueueName, std::string id,
               std::shared_ptr<Sync::Receiver> syncReceiver)
: _pHeartbeatSender(nullptr),
  _pSyncSender(nullptr),
  _pSyncReceiver(syncReceiver),
  _state(INIT),
  sampleVal(0),
  measureVal(0),
  numRuns(0),
  _syncCounter(0)
{
    srand(time(NULL));

    Sensor::sample_size = sample_size;
    Sensor::scaling_factor = scaling_factor;
    Sensor::id = id;

    _pHeartbeatSender = std::make_shared<Heartbeat::Sender>(id, hbQueueName);
    _pSyncReceiver->RxSignal.connect(boost::bind(&Sensor::handleRxValues, this, boost::placeholders::_1));

    _pHandleServerfault = std::make_shared<FaultHandle::Server>(fhQueueName);
    _pHandleServerfault->sigWakeUp.connect(boost::bind(&Sensor::handleWakeup, this));
    _pHandleServerfault->sigGetData.connect(boost::bind(&Sensor::handleGetData, this));
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

void Sensor::handleRxValues(std::list<int> rxValues)
{
    _prevSamples = rxValues;
}

void Sensor::handleWakeup()
{
    // std::cout << "SENSOR WAKING UP!" << std::endl;
    _setActive(true);
}

std::list<int> Sensor::handleGetData()
{
    return _prevSamples;
}

Sensor::State_e Sensor::_getState()
{
    return _state;
}

void Sensor::_setState(State_e state)
{
    // std::cout << __FUNCTION__ << "(" << id << "): " << state << std::endl;
    _state = state;
}
bool Sensor::_getActive()
{
    std::lock_guard<std::mutex> lock(_mutex);
    return active;
}
void Sensor::_setActive(bool a)
{
    // std::cout << __FUNCTION__ << ": " << (a ? "TRUE" : "FALSE") << std::endl;
    std::lock_guard<std::mutex> lock(_mutex);
    active = a;
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
        case INACTIVE:
            nextSleepTime = _inactive();
            break;
        case FAILURE:
            nextSleepTime = _failure();
            break;
        case DEAD:
            nextSleepTime = _dead();
            break;
        default:
            break;
        }
        std::this_thread::sleep_for(nextSleepTime);
    } while (!_getShutdown());

    _setState(DEAD);
    _pHandleServerfault->end();
}

std::chrono::milliseconds Sensor::_init()
{
    if (!_pHeartbeatSender->started())
        _pHeartbeatSender->start();

    if (_pSyncSender && !_pSyncSender->started())
        _pSyncSender->start();

    if (_pSyncReceiver && !_pSyncReceiver->started())
        _pSyncReceiver->start();

    if (!_pHandleServerfault->started())
        _pHandleServerfault->start();
    
    // Start Sensor as inactive (wait until told to start).
    _setActive(false);
    _setState(PREFILL);

    return std::chrono::milliseconds(0);
}

std::chrono::milliseconds Sensor::_prefill()
{
    // Get samples until the sample buffer is filled.
    for(int i = _prevSamples.size() ; i < Sensor::sample_size; i++)
    {
        sample(20,20);
    }

    if(_pSyncSender)
        _pSyncSender->cacheValues(_prevSamples);
    
    _setState(INACTIVE);
    return std::chrono::milliseconds(0);
}

std::chrono::milliseconds Sensor::_measure()
{
    sampleVal = sample(20, 20);
    measureVal = measure();

    // if(_syncCounter++ > 9 && _pSyncSender)
    // {
    //     std::cout << "SYNCING" << std::endl;
    if(_pSyncSender)
        _pSyncSender->cacheValues(_prevSamples);
    //     _syncCounter = 0;
    // }

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

std::chrono::milliseconds Sensor::_inactive()
{
    // std::cout << "SENSOR INACTIVE: " << Sensor::id << std::endl;
    
    // Sensor should be taken out of inactive as requested.
    if (_getActive())
    {
        // Start Sensor Prefilling (NOTE: Relies on prefill going to measure if enough values)
        _setState(MEASURE);
    }
    return std::chrono::milliseconds(100);
}

std::chrono::milliseconds Sensor::_failure()
{
    // std::cout << "Num Runs: " << numRuns << std::endl;
    std::cout << "SENSOR FAILURE: " << Sensor::id << std::endl;
    _pHeartbeatSender->end();
    _setActive(false);
    _setState(DEAD);
    return std::chrono::milliseconds(100);
}

std::chrono::milliseconds Sensor::_dead()
{
    if (_getActive())
    {
        std::cout << "Activate dead sensor " << Sensor::id << std::endl;
        _setState(INIT);
    }

    return std::chrono::milliseconds(100);
}

} // namespace Assignment2