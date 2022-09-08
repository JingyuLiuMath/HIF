#include "HIF.h"

namespace {
// Global Variable
std::ofstream g_logFile;
bool g_logFileAppend = false;
DEBUG_HIF(std::stack<std::string> callStack)
TIMER_HIF(std::vector<El::Timer> timerQueue)
double g_tol;
bool g_button;
} // namespace

namespace HIF {

void OpenLog(const char *filename) {
  if (::g_logFileAppend) {
    ::g_logFile.open(filename, std::fstream::app);
  } else {
    ::g_logFile.open(filename);
  }
}

void LogAppend(bool logAppend) { ::g_logFileAppend = logAppend; }

std::ostream &LogOS() {
  if (!::g_logFile.is_open()) {
    std::ostringstream fileOS;
    fileOS << "./log";
    fs::path filedir(fileOS.str().c_str());
    if (!fs::exists(filedir)) {
      fs::create_directory(filedir);
    }
    fileOS << "/log.log";
    OpenLog(fileOS.str().c_str());
  }
  return ::g_logFile;
}

void CloseLog() {
  if (::g_logFile.is_open()) {
    ::g_logFile << "\n\n";
  }
  ::g_logFile.close();
}

DEBUG_HIF(
    void PushCallStack(std::string s) { ::callStack.push(s); }

    void PopCallStack() { ::callStack.pop(); }

    void DumpCallStack() {
      std::ostringstream msg;
      while (!::callStack.empty()) {
        msg << "[" << ::callStack.size() << "]: " << ::callStack.top() << "\n";
        ::callStack.pop();
      }
      std::cerr << msg.str();
      std::cerr.flush();
    })

TIMER_HIF(
    void InitTimer() { timerQueue.resize(NUM_OF_TIMER_TYPE); }

    void TimerStart(TimerType typ) {
      if (timerQueue.size() < NUM_OF_TIMER_TYPE) {
        InitTimer();
      }
      timerQueue[typ].Start();
    }

    double TimerStop(TimerType typ) {
      if (timerQueue.size() < NUM_OF_TIMER_TYPE) {
        return 0.0;
      } else {
        return timerQueue[typ].Stop();
      }
    }

    double TimerPartial(TimerType typ) {
      if (timerQueue.size() < NUM_OF_TIMER_TYPE) {
        return 0.0;
      } else {
        return timerQueue[typ].Partial();
      }
    }

    double TimerTotal(TimerType typ) {
      if (timerQueue.size() < NUM_OF_TIMER_TYPE) {
        return 0.0;
      } else {
        return timerQueue[typ].Total();
      }
    })

void SetTol(double tol) { ::g_tol = tol; }

double Tol() { return ::g_tol; }

void SetButton(bool button) { ::g_button = button; }

bool Button() { return ::g_button; }

} // namespace HIF.
