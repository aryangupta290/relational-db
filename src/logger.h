#ifndef __LOGGER_H
#define __LOGGER_H

#include <bits/stdc++.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>

using namespace std;

class Logger {
    string logFile = "log";
    ofstream fout;

   public:
    Logger();
    void log(string logString);
};

extern Logger logger;

#endif