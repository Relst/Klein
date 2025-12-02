//
// Created by Karanbir Singh on 26/11/2025.
//

#pragma once
#include "App.h"
#include "Logger.h"


int main(int argc, char** argv){
    auto app = Klein::CreateApp();
    if (Klein::appWindowName){
        KleinLogger::Logger::EngineLog("Successfully Initialised Application | [%s]", (Klein::appWindowName ));
    } else {
        KleinLogger::Logger::EngineWarn("Application Name Not Initialised.");
        KleinLogger::Logger::EngineLog("Successfully Initialised Application | [%s]", Klein::appDefaultName);
    }
    KleinLogger::Logger::EngineLog("Application Window Initialised [%d x %d]", Klein::appWindowX, Klein::appWindowY);


    app->Run();
    delete app;
}
