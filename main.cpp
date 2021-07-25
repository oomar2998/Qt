#include <ara/log/logging.h>
#include "mainwindow.h"

#include <QApplication>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>

#include "ara/com/internal/runtime.h"
#include "ara/core/string.h"
#include "ara/exec/execution_client.h"
#include "ara/log/logging.h"
#include "ara/log/logmanager.h"
#include "find_service.h"
#include "DriverApplication/DriverApplication.h"

using namespace ara::ucm::pkgmgr;
using ara::ucm::pkgmgr::proxy::VehiclePackageManagementProxy;

int main() {
  //std::cout << std::endl << std::endl;
 // std::cout << "      Driver Application STARTS    " << std::endl;
  //std::cout << "===========================" << std::endl;
  const std::string logDir = "/tmp/driver-logs";
  // report application state
  ara::exec::ExecutionClient exec_client;
  exec_client.ReportExecutionState(ara::exec::ExecutionState::kRunning);
  InitLogging("DriverApp", "Driver Application main context", ara::log::LogLevel::kVerbose,
              ara::log::LogMode::kConsole | ara::log::LogMode::kRemote);

  auto &logger = ara::log::CreateLogger("TEST", "Test Sequence context",
                                        ara::log::LogLevel::kVerbose);
  boost::filesystem::create_directories(logDir);


  DriverApplication driverApp;
  driverApp.Initialize();
  driverApp.Run();


  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();

}

