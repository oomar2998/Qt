#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#ifndef OTA_CLIENT_HEADER
#define OTA_CLIENT_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include<unistd.h>

#include "ara/log/logging.h"
#include "ara/ucm/pkgmgr/impl_type_swclusterinfovectortype.h"
#include "boost/filesystem.hpp"
#include "find_service.h"

#include <QMainWindow>

#define DriverDecisionNull 0
#define DriverDecisionAccept 1
#define DriverDecisionRefuse 2

namespace ara {
namespace ucm {
namespace pkgmgr {

class DriverApplication {
 public:
  DriverApplication();
  ~DriverApplication() = default;


  static int driverDecision =DriverDecisionNull;
  bool Initialize() ;
  void Run() ;
  bool ApprovalForTransfer() ;
  bool ApprovalForProcess();
  void PrintTransferProgress();
  void PrintProcessProgress();


 private:

  ara::log::Logger& log_;
  ara::ucm::otaclient::ServiceProxyPtr ucmMaster_;

};

}  // namespace pkgmgr
}  // namespace ucm
}  // namespace ara


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow , public DriverApplication
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


//    auto getSwPackageDescriptionFuture = ucmMaster_->GetSwPackageDescription();
//    auto getSwPackageDescriptionResult = getSwPackageDescriptionFuture.get();
//    auto TransferProgress = ucmMaster_->GetSwTransferProgress().get().progress;
private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_8_clicked();
    void incrementar_1();
    void incrementar_2();


    void on_pushButton_10_clicked();

    void on_pushButton_clicked();


private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
#endif // OTA_CLIENT_HEADER
