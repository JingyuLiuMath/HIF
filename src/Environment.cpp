#include "HIF.hpp"

namespace {
// Global Variable
std::ofstream logFile;
bool logFileAppend = false;
DEBUG_HIF(std::stack<std::string> callStack)
TIMER_HIF(std::vector<El::Timer> timerQueue)
double TOL;
}

namespace HIF {

void OpenLog(const char* filename)
{
    if (::logFileAppend)
    {
        ::logFile.open(filename, std::fstream::app);
    }
    else
    {
        ::logFile.open(filename);
    }
}

void LogAppend(bool logAppend)
{
    ::logFileAppend = logAppend;
}

std::ostream& LogOS()
{
    if (!::logFile.is_open())
    {
        std::ostringstream fileOS;
        fileOS << "./log";
        fs::path filedir(fileOS.str().c_str());
        if (!fs::exists(filedir))
        {
            fs::create_directory(filedir);
        }
    }
    return ::logFile;
}

void CloseLog()
{
    if (::logFile.is_open())
    {
        ::logFile << "\n\n";
    }
    ::logFile.close();
}

DEBUG_HIF(
    void PushCallStack(std::string s)
    {
        ::callStack.push(s);
    }

    void PopCallStack()
    {
        ::callStack.pop();
    }

    void DumpCallStack()
    {
        std::ostringstream msg;
        while (!::callStack.empty())
        {
            msg << "[" << ::callStack.size() << "]: " << ::callStack.top()
                << "\n";
            ::callStack.pop();
        }
        std::cerr << msg.str();;
        std::cerr.flush();
    }
)

TIMER_HIF(
    void InitTimer()
    {
        timerQueue.resize(NUM_OF_TIMER_TYPE);
    }

    void TimerStart(TimerType typ)
    {
        if (timerQueue.size() < NUM_OF_TIMER_TYPE)
        {
            InitTimer();
        }
        timerQueue[typ].Start();
    }

    double TimerStop(TimerType typ)
    {
        if (timerQueue.size() < NUM_OF_TIMER_TYPE)
        {
            return 0.0;
        }
        else
        {
            return timerQueue[typ].Stop();
        }
    }

    double TimerPartial(TimerType typ)
    {
        if (timerQueue.size() < NUM_OF_TIMER_TYPE)
        {
            return 0.0;
        }
        else
        {
            return timerQueue[typ].Partial();
        }
    }

    double TimerTotal(TimerType typ)
    {
        if (timerQueue.size() < NUM_OF_TIMER_TYPE)
        {
            return 0.0;
        }
        else
        {
            return timerQueue[typ].Total();
        }
    }
)

<<<<<<< HEAD
void SetTol(double tol)
{
    ::TOL = tol;
}

double Tol()
{
    return ::TOL;
}

} // namespace HIF.

