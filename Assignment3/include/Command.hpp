#ifndef ASSIGNMENT3_COMMAND_HPP
#define ASSIGNMENT3_COMMAND_HPP

namespace Assignment3
{
class Command
{
public:
    Command() {_hasRun = false;}
    virtual ~Command() = default;

private:
    int _result;    // Stored result of execution.
    bool _hasRun;

private: // Interface
    /**
     * @brief Operation to be performed by Command.  NOTE: Must be overriden to provide functionality.
     * 
     * @result Result of operation.
     */
    virtual int _operation();

public:
    /**
     * @brief If operation for this command has NOT been perform then run opreation and get result.
     */
    void execute() {if (!_hasRun) {_result = _operation(); _hasRun = true;} }


public:
    int getResult() {if (_hasRun) return _result;  /* TODO: Else Exception?*/ return -1;}


};
} // namespace Assignment3

#endif