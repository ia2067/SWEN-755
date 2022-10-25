#ifndef ASSIGNMENT3_COMMAND_HPP
#define ASSIGNMENT3_COMMAND_HPP

#include <mutex>

namespace Assignment3
{
class Command
{
public:
    Command() {_hasRun = false;}
    virtual ~Command() = default;

private:
    double _result;    // Stored result of execution.
    bool _hasRun;

private: // Interface
    /**
     * @brief Operation to be performed by Command.  NOTE: Must be overriden to provide functionality.
     * 
     * @result Result of operation.
     */
    virtual double _operation() = 0;

public:
    /**
     * @brief If operation for this command has NOT been perform then run opreation and get result.
     */
    void execute() {if (!_getHasRun()) {_result = _operation(); _setHasRun(true);} }


public:
    double getResult() {if (_getHasRun()) return _result;  /* TODO: Else Exception?*/ return -1;}
    virtual int getId() = 0;    //  NOTE: Must be overriden to provide new ID.

private:
    std::mutex _mutex;

    void _setHasRun(bool hasRun)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _hasRun = hasRun;
    }

    bool _getHasRun()
    {
        std::lock_guard<std::mutex> lock(_mutex);
        return _hasRun;
    }

};
} // namespace Assignment3

#endif