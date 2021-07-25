#include "DriverApplication/DriverApplication.h"
#include <regex>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QTimer>

QTimer *timer1=new QTimer();
QTimer *timer2=new QTimer();
//    auto getSwPackageDescriptionFuture = ucmMaster_->GetSwPackageDescription();
//    auto getSwPackageDescriptionResult = getSwPackageDescriptionFuture.get();
//    auto TransferProgress = ucmMaster_->GetSwTransferProgress().get().progress;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(timer1,
            SIGNAL(timeout()),
            this,
            SLOT(incrementar_1())
            );
    connect(timer2,
            SIGNAL(timeout()),
            this,
            SLOT(incrementar_2())
            );
}

MainWindow::~MainWindow()
{
    delete ui;
}

namespace ara {
namespace ucm {
namespace pkgmgr {

DriverApplication::DriverApplication():log_(ara::log::CreateLogger("OTA0", "Driver Application main program",
                                ara::log::LogLevel::kVerbose))
{

}

bool DriverApplication::Initialize() {
  auto foundService = ara::ucm::otaclient::FindPackageManagementService();
  ara::ucm::otaclient::ServiceProxyPtr service = nullptr;
  if (foundService) {
    log_.LogInfo() << "Found UCM Master *wink* *wink*";
    log_.LogInfo() << "foundService" << foundService.has_value();
    service = *foundService;
    auto state = service->CampaignStatus.Get();
    log_.LogInfo() << "Current State 0X" << int(state.get());

  } else {
    log_.LogError() << " Didn't Found UCM Master *wink* *wink*";
    return false;
  }

  ucmMaster_ = service;

  return true;
}
void DriverApplication::Run() {


  while(1)
  {
    // check if there is any approvals needed
    auto approvalReq = ucmMaster_->ApprovalRequired.Get().get();
    //std::cout <<"AFter ucmMaster_->ApprovalRequired.Get().get(); "  <<"\n" ;
    if (approvalReq == true)//Start Driver Approval Sequence
    {
      //New update Detected(GUI)
      ui->stackedWidget->setCurrentIndex(1);
      //std::cout <<"if (approvalReq == true)"  <<"\n" ;
      // Transfer or Process Approval
      //1- Get the State
      auto state = ucmMaster_->CampaignStatus.Get();
      //2-Approval for Transfer Or Process
      switch (state.get())
      {
        case CampaignStateType::kVehiclePackageTransferring :
        {

          // Approval to Transfer vehicle Sw Packs
          std::cout << "CampaignStateType::kVehiclePackageTransferring " << "\n" ;
          bool DriverResponse = this->ApprovalForTransfer();

          if(DriverResponse == true)
          {
          std::cout << "if(DriverResponse == true)" << "\n" ;
            // Watch the Progress of SW packs Transfering
            PrintTransferProgress();
          }
          break;
        }

        case CampaignStateType::kSoftwarePackage_Transferring :
        {
          // Approval to Process vehicle Sw Packs
          std::cout << "CampaignStateType::kSoftwarePackage_Transferring " << "\n" ;
          bool DriverResponse = DriverApplication::ApprovalForProcess();
          if(DriverResponse == true)
          {
            // Watch the Progress of SW packs Processing
            auto state = ucmMaster_->CampaignStatus.Get().get();
            std::cout << "State : " << (int)state << std::endl;
            while (state !=
                   ara::ucm::pkgmgr::CampaignStateType::kProcessing) {

              std::cout << "State (inside while loop) : " << (int)state << std::endl;
              state = ucmMaster_->CampaignStatus.Get().get();
            }
            // DELAY WAS HERE(PROPLEM SOLVED)
            PrintProcessProgress();
          }
          break ;
        }

        default:
          std::cout << "defualt in the switch" << "\n" ;

          break;
      }
    }
    else // Sleep for 5 seconds
    {
      //std::cout <<"else : (approvalReq == false)"  <<"\n" ;
      std::chrono::milliseconds(5000);
    }
  }

  bool DriverApplication::ApprovalForTransfer()
  {
    bool DriverResponse = false ;
    //std::cout << "before GetSwPackageDescription in ApprovalForTransfer  \n" ;
    auto getSwPackageDescriptionFuture = ucmMaster_->GetSwPackageDescription();
    //std::cout << "After GetSwPackageDescription in ApprovalForTransfer  \n" ;

    // std::cout << "before getSwPackageDescriptionFuture.get(); in ApprovalForTransfer  \n" ;
    auto getSwPackageDescriptionResult = getSwPackageDescriptionFuture.get();
    //std::cout << "After getSwPackageDescriptionFuture.get(); in ApprovalForTransfer  \n" ;

   // std::cout << "Gamadan incoming \n";
   // std::cout << "Software packages in the current Update" << std::endl;
    for (uint16_t i = 0 ; i < getSwPackageDescriptionResult.Packages.size() ; i++)
    {
      //Add labeles(GUI)
      //std::cout << "Software package " << i << " Description : \n" ;
      ui ->textBrowser_6->setText("Name",getSwPackageDescriptionResult.Packages.at(i).Name);
      ui ->textBrowser_6->setText("Version",getSwPackageDescriptionResult.Packages.at(i).Version);
      ui ->textBrowser_6->setText("Size",getSwPackageDescriptionResult.Packages.at(i).Size);
      //std::cout << "Software package " << i << " Description : \n" ;
      //std::cout << "Name : " << getSwPackageDescriptionResult.Packages.at(i).Name << std::endl;
      //std::cout << "Version : " << getSwPackageDescriptionResult.Packages.at(i).Version << std::endl;
      //std::cout << "Size : " << getSwPackageDescriptionResult.Packages.at(i).Size << std::endl;
    }
    //Driver is reviewing the Descriptions
    std::chrono::milliseconds(1000);
    //std::cout << "Allow To Download The Previous Software Packages? (Yes/No) \n";
    //bool driverDecision=false;
    While(driverDecision==driverDecisionNull);//Wait until driver response for tarnsfer

    //std::cin >> driverDecision;
    if (driverDecision==driverDecisionAccept)
    {
      ucmMaster_->DriverApproval( true , "void" );
      DriverResponse = true ;
      driverDecision=driverDecisionNull;
    }
    else
    {
      ucmMaster_->DriverApproval( false , "void" );
      DriverResponse = false ;
      driverDecision=driverDecisionNull;
    }
    return DriverResponse ;
  }

  //DRIVER APP
}
void MainWindow::incrementar_1()
{
    int aux;
    aux=ui->progressBar_1->value();
    aux++;
    ui->progressBar_1->setValue((aux));
}
void MainWindow::incrementar_2()
{
    int aux;
    aux=ui->progressBar_2->value();
    aux++;
    ui->progressBar_2->setValue((aux));
}


void MainWindow::on_pushButton_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
  //ui ->textBrowser_6->setText("Name",getSwPackageDescriptionResult.Packages.Name,"/nVersion",getSwPackageDescriptionResult.Packages.Version,"/nSize", getSwPackageDescriptionResult.Packages.Size);
}

void MainWindow::on_pushButton_2_clicked()
{
    DriverApplication::driverDecision = DriverDecisionRefuse;
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    timer2->start(100);
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_5_clicked()
{

   ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_10_clicked()
{
    DriverApplication::driverDecision = DriverDecisionAccept;
    ui->stackedWidget->setCurrentIndex(2);
    timer1->start(100);

}

void MainWindow::on_pushButton_clicked()
{
    close();
}



