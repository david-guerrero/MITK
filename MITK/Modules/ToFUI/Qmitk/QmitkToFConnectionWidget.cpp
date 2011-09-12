/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Module:    $RCSfile$
Language:  C++
Date:      $Date: 2009-05-20 13:35:09 +0200 (Mi, 20 Mai 2009) $
Version:   $Revision: 17332 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

//#define _USE_MATH_DEFINES
#include <QmitkToFConnectionWidget.h>

//QT headers
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qcombobox.h>

//mitk headers
#include <mitkToFImageGrabberCreator.h>

//itk headers
#include <itksys/SystemTools.hxx>

const std::string QmitkToFConnectionWidget::VIEW_ID = "org.mitk.views.qmitktofconnectionwidget";

QmitkToFConnectionWidget::QmitkToFConnectionWidget(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
  this->m_IntegrationTime = 0;
  this->m_ModulationFrequency = 0;
  this->m_ToFImageGrabber = NULL;

  m_Controls = NULL;
  CreateQtPartControl(this);
}

QmitkToFConnectionWidget::~QmitkToFConnectionWidget()
{
}

void QmitkToFConnectionWidget::CreateQtPartControl(QWidget *parent)
{
  if (!m_Controls)
  {
    // create GUI widgets
    m_Controls = new Ui::QmitkToFConnectionWidgetControls;
    m_Controls->setupUi(parent);
    this->CreateConnections();

    ShowParameterWidget();
  }
}

void QmitkToFConnectionWidget::CreateConnections()
{
  if ( m_Controls )
  {
    connect( (QObject*)(m_Controls->m_ConnectCameraButton), SIGNAL(clicked()),(QObject*) this, SLOT(OnConnectCamera()) );
    connect( m_Controls->m_SelectCameraCombobox, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelectCamera(int)) );
    connect( m_Controls->m_SelectCameraCombobox, SIGNAL(activated(int)), this, SLOT(OnSelectCamera(int)) );
    connect( m_Controls->m_SelectCameraCombobox, SIGNAL(activated(int)), this, SIGNAL(ToFCameraSelected(int)) );

    //connect( m_Controls->m_IntegrationTimeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(OnChangeIntegrationTimeSpinBox(int)) );
    //connect( m_Controls->m_ModulationFrequencySpinBox, SIGNAL(valueChanged(int)), this, SLOT(OnChangeModulationFrequencySpinBox(int)) );
  }
}

void QmitkToFConnectionWidget::ShowParameterWidget()
{
  int selectedCamera = m_Controls->m_SelectCameraCombobox->currentIndex();
  switch (selectedCamera)
  {
  case 0:
  case 1:
  case 2:   ShowPMDParameterWidget();
            break;
  case 3:   ShowMESAParameterWidget();
            break;
  default:  this->m_Controls->m_PMDParameterWidget->hide();
            this->m_Controls->m_MESAParameterWidget->hide();
  }
}

void QmitkToFConnectionWidget::ShowPMDParameterWidget()
{
  this->m_Controls->m_PMDParameterWidget->show();
  this->m_Controls->m_MESAParameterWidget->hide();
}

void QmitkToFConnectionWidget::ShowMESAParameterWidget()
{
  this->m_Controls->m_PMDParameterWidget->hide();
  this->m_Controls->m_MESAParameterWidget->show();
}

void QmitkToFConnectionWidget::ShowPlayerParameterWidget()
{
  this->m_Controls->m_PMDParameterWidget->hide();
  this->m_Controls->m_MESAParameterWidget->hide();
}

mitk::ToFImageGrabber* QmitkToFConnectionWidget::GetToFImageGrabber()
{
  return m_ToFImageGrabber;
}

void QmitkToFConnectionWidget::OnSelectCamera(int index)
{
  if (index == 0) // PMD camcube 2
  {
    //m_Controls->m_IntegrationTimeSpinBox->setEnabled(true);
    //m_Controls->m_ModulationFrequencySpinBox->setEnabled(true);
    //m_Controls->m_CalibrationParameterGroupBox->setEnabled(true);
    ShowPMDParameterWidget();
  }
  else if (index == 1) // pmd camboard
  {
    //m_Controls->m_IntegrationTimeSpinBox->setEnabled(true);
    //m_Controls->m_ModulationFrequencySpinBox->setEnabled(true);
    //m_Controls->m_CalibrationParameterGroupBox->setEnabled(false);
    ShowPMDParameterWidget();
  }
  else if (index == 2) // pmd O3d
  {
    //m_Controls->m_IntegrationTimeSpinBox->setEnabled(true);
    //m_Controls->m_ModulationFrequencySpinBox->setEnabled(true);
    //m_Controls->m_CalibrationParameterGroupBox->setEnabled(false);
    ShowPMDParameterWidget();
  }
  else if (index == 3) // MESA 4000
  {
    ShowMESAParameterWidget();
  }
  else if (index == 4) // pmd file player
  {
    //m_Controls->m_IntegrationTimeSpinBox->setEnabled(false);
    //m_Controls->m_ModulationFrequencySpinBox->setEnabled(false);
    //m_Controls->m_CalibrationParameterGroupBox->setEnabled(false);
    ShowPlayerParameterWidget();
  }
  else if (index == 5) // pmd raw data player
  {
    //m_Controls->m_IntegrationTimeSpinBox->setEnabled(false);
    //m_Controls->m_ModulationFrequencySpinBox->setEnabled(false);
    //m_Controls->m_CalibrationParameterGroupBox->setEnabled(false);
    ShowPlayerParameterWidget();
  }
  else if (index == 6) // mitk player
  {
    //m_Controls->m_IntegrationTimeSpinBox->setEnabled(false);
    //m_Controls->m_ModulationFrequencySpinBox->setEnabled(false);
    //m_Controls->m_CalibrationParameterGroupBox->setEnabled(false);
    ShowPlayerParameterWidget();
  }
}

void QmitkToFConnectionWidget::OnConnectCamera()
{
  bool playerMode = false;

  if (m_Controls->m_ConnectCameraButton->text()=="Connect")
  {   
    //reset the status of the GUI buttons
    m_Controls->m_ConnectCameraButton->setEnabled(false);
    m_Controls->m_SelectCameraCombobox->setEnabled(false);
//    m_Controls->m_CalibrationParameterGroupBox->setEnabled(false);
    //repaint the widget
    this->repaint();

    QString tmpFileName("");
    QString fileFilter("");

    //select the camera to connect with
    int selectedCamera = m_Controls->m_SelectCameraCombobox->currentIndex();
    if (selectedCamera == 0)
    { //PMD CamCube
      this->m_ToFImageGrabber = mitk::ToFImageGrabberCreator::GetInstance()->GetPMDCamCubeImageGrabber();
    }
    else if (selectedCamera == 1)
    { //PMD CamBoard
      this->m_ToFImageGrabber = mitk::ToFImageGrabberCreator::GetInstance()->GetPMDCamBoardImageGrabber();
    }
    else if (selectedCamera == 2)
    {//PMD O3
      this->m_ToFImageGrabber = mitk::ToFImageGrabberCreator::GetInstance()->GetPMDO3ImageGrabber();
    }
    else if (selectedCamera == 3)
    {//MESA SR4000
      this->m_ToFImageGrabber = mitk::ToFImageGrabberCreator::GetInstance()->GetMESASR4000ImageGrabber();
    }
    else if (selectedCamera == 4)
    {//PMD player
      playerMode = true;
      fileFilter.append("PMD Files (*.pmd)");
      this->m_ToFImageGrabber = mitk::ToFImageGrabberCreator::GetInstance()->GetPMDPlayerImageGrabber();
    }
    else if (selectedCamera == 5)
    {//PMD MITK player
      playerMode = true;
      fileFilter.append("MITK Images (*.pic)");
      this->m_ToFImageGrabber = mitk::ToFImageGrabberCreator::GetInstance()->GetPMDMITKPlayerImageGrabber();
    }
    else if (selectedCamera == 6)
    {//MITK player
      playerMode = true;
      fileFilter.append("MITK Images (*.pic)");
      this->m_ToFImageGrabber = mitk::ToFImageGrabberCreator::GetInstance()->GetMITKPlayerImageGrabber();
    }

    // if a player was selected ...
    if (playerMode)
    { //... open a QFileDialog to chose the corresponding file from the disc
      tmpFileName = QFileDialog::getOpenFileName(NULL, "Play Image From...", "", fileFilter);
      if (tmpFileName.isEmpty())
      {
        m_Controls->m_ConnectCameraButton->setChecked(false);
        m_Controls->m_ConnectCameraButton->setEnabled(true);
        m_Controls->m_SelectCameraCombobox->setEnabled(true);
//        m_Controls->m_CalibrationParameterGroupBox->setEnabled(true);
        OnSelectCamera(m_Controls->m_SelectCameraCombobox->currentIndex());
        QMessageBox::information( this, "Template functionality", "Please select a valid image before starting some action.");
        return;
      }
      if(selectedCamera == 4)
      { //set the PMD file name
        this->m_ToFImageGrabber->SetStringProperty("PMDFileName", tmpFileName.toStdString().c_str() );
      }
      if (selectedCamera == 5 || selectedCamera == 6)
      {
        std::string msg = "";
        try
        {
          //get 3 corresponding file names
          std::string dir = itksys::SystemTools::GetFilenamePath( tmpFileName.toStdString() );
          std::string baseFilename = itksys::SystemTools::GetFilenameWithoutLastExtension( tmpFileName.toStdString() );
          std::string extension = itksys::SystemTools::GetFilenameLastExtension( tmpFileName.toStdString() );

          if (extension != ".pic")
          {
            msg = msg + "Invalid file format, please select a \".pic\"-file";
            throw std::logic_error(msg.c_str());
          }
          int found = baseFilename.rfind("_DistanceImage");
          if (found == std::string::npos)
          {
            found = baseFilename.rfind("_AmplitudeImage");
          }
          if (found == std::string::npos)
          {
            found = baseFilename.rfind("_IntensityImage");
          }
          if (found == std::string::npos)
          {
            msg = msg + "Input file name must end with \"_DistanceImage.pic\", \"_AmplitudeImage.pic\" or \"_IntensityImage.pic\"!";
            throw std::logic_error(msg.c_str());
          }
          std::string baseFilenamePrefix = baseFilename.substr(0,found);

          std::string distanceImageFileName = dir + "/" + baseFilenamePrefix + "_DistanceImage" + extension;
          std::string amplitudeImageFileName = dir + "/" + baseFilenamePrefix + "_AmplitudeImage" + extension;
          std::string intensityImageFileName = dir + "/" + baseFilenamePrefix + "_IntensityImage" + extension;

          if (!itksys::SystemTools::FileExists(distanceImageFileName.c_str(), true))
          {
            msg = msg + "Inputfile not exist! " + distanceImageFileName;
            throw std::logic_error(msg.c_str());
          }
          if (!itksys::SystemTools::FileExists(amplitudeImageFileName.c_str(), true))
          {
            msg = msg + "Inputfile not exist! " + amplitudeImageFileName;
            throw std::logic_error(msg.c_str());
          }
          if (!itksys::SystemTools::FileExists(intensityImageFileName.c_str(), true))
          {
            msg = msg + "Inputfile not exist! " + intensityImageFileName;
            throw std::logic_error(msg.c_str());
          }
          //set the file names
          this->m_ToFImageGrabber->SetStringProperty("DistanceImageFileName", distanceImageFileName.c_str());
          this->m_ToFImageGrabber->SetStringProperty("AmplitudeImageFileName", amplitudeImageFileName.c_str());
          this->m_ToFImageGrabber->SetStringProperty("IntensityImageFileName", intensityImageFileName.c_str());

        }
        catch (std::exception &e)
        {
          MITK_ERROR << e.what();
          QMessageBox::critical( this, "Error", e.what() );
          m_Controls->m_ConnectCameraButton->setChecked(false);
          m_Controls->m_ConnectCameraButton->setEnabled(true);
          m_Controls->m_SelectCameraCombobox->setEnabled(true);
//          m_Controls->m_CalibrationParameterGroupBox->setEnabled(true);
          OnSelectCamera(m_Controls->m_SelectCameraCombobox->currentIndex());
          return;
        }
      }

    }
    //if a connection could be established
    if (this->m_ToFImageGrabber->ConnectCamera())
    {
      this->m_Controls->m_PMDParameterWidget->SetToFImageGrabber(this->m_ToFImageGrabber);
      this->m_Controls->m_MESAParameterWidget->SetToFImageGrabber(this->m_ToFImageGrabber);

      switch (selectedCamera)
      {
      case 0:
      case 1:
      case 2: this->m_Controls->m_PMDParameterWidget->ActivateAllParameters();
              break;
      case 3: this->m_Controls->m_MESAParameterWidget->ActivateAllParameters();
              break;
      }

      
/*
      //get the integration time and modulation frequency
      this->m_IntegrationTime = m_Controls->m_IntegrationTimeSpinBox->value();
      this->m_ModulationFrequency = m_Controls->m_ModulationFrequencySpinBox->value();

      //set the integration time and modulation frequency in the grabber
      this->m_IntegrationTime = this->m_ToFImageGrabber->SetIntegrationTime(this->m_IntegrationTime);
      this->m_ModulationFrequency = this->m_ToFImageGrabber->SetModulationFrequency(this->m_ModulationFrequency);

      //set the PMD calibration according to the check boxes
      bool boolValue = false;
      boolValue = m_Controls->m_FPNCalibCB->isChecked();
      this->m_ToFImageGrabber->SetBoolProperty("SetFPNCalibration", boolValue);
      boolValue = m_Controls->m_FPPNCalibCB->isChecked();
      this->m_ToFImageGrabber->SetBoolProperty("SetFPPNCalibration", boolValue);
      boolValue = m_Controls->m_LinearityCalibCB->isChecked();
      this->m_ToFImageGrabber->SetBoolProperty("SetLinearityCalibration", boolValue);
      boolValue = m_Controls->m_LensCorrection->isChecked();
      this->m_ToFImageGrabber->SetBoolProperty("SetLensCalibration", boolValue);
      boolValue = m_Controls->m_ExposureModeCB->isChecked();
      this->m_ToFImageGrabber->SetBoolProperty("SetExposureMode", boolValue);

      //reset the GUI elements
      m_Controls->m_IntegrationTimeSpinBox->setValue(this->m_IntegrationTime);
      m_Controls->m_ModulationFrequencySpinBox->setValue(this->m_ModulationFrequency);
*/
      m_Controls->m_ConnectCameraButton->setText("Disconnect");

      // send connect signal to the caller functionality
      emit ToFCameraConnected();
    }
    else
    {
      QMessageBox::critical( this, "Error", "Connection failed. Check if you have installed the latest driver for your system." );
      m_Controls->m_ConnectCameraButton->setChecked(false);
      m_Controls->m_ConnectCameraButton->setEnabled(true);
      m_Controls->m_SelectCameraCombobox->setEnabled(true);
//      m_Controls->m_CalibrationParameterGroupBox->setEnabled(true);
      OnSelectCamera(m_Controls->m_SelectCameraCombobox->currentIndex());
      return;

    }
    m_Controls->m_ConnectCameraButton->setEnabled(true);

  }
  else if (m_Controls->m_ConnectCameraButton->text()=="Disconnect")
  {
    //send camera stop to the caller functionality
    emit ToFCameraStop();

    this->m_ToFImageGrabber->StopCamera();
    this->m_ToFImageGrabber->DisconnectCamera();
    m_Controls->m_ConnectCameraButton->setText("Connect");
    m_Controls->m_SelectCameraCombobox->setEnabled(true);
//    m_Controls->m_CalibrationParameterGroupBox->setEnabled(true);
    OnSelectCamera(m_Controls->m_SelectCameraCombobox->currentIndex());

    this->m_ToFImageGrabber = NULL;
    // send disconnect signal to the caller functionality
    emit ToFCameraDisconnected();

  }
}
/*
void QmitkToFConnectionWidget::OnChangeIntegrationTimeSpinBox(int value)
{
  if (this->m_ToFImageGrabber != NULL)
  {
    // stop camera if active
    bool active = m_ToFImageGrabber->IsCameraActive();
    if (active)
    {
      m_ToFImageGrabber->StopCamera();
    }
    this->m_IntegrationTime = m_Controls->m_IntegrationTimeSpinBox->value();
    this->m_IntegrationTime = this->m_ToFImageGrabber->SetIntegrationTime(this->m_IntegrationTime);
    if (active)
    {
      m_ToFImageGrabber->StartCamera();
    }
  }
}

void QmitkToFConnectionWidget::OnChangeModulationFrequencySpinBox(int value)
{
  if (this->m_ToFImageGrabber != NULL)
  {
    // stop camera if active
    bool active = m_ToFImageGrabber->IsCameraActive();
    if (active)
    {
      m_ToFImageGrabber->StopCamera();
    }
    this->m_ModulationFrequency = m_Controls->m_ModulationFrequencySpinBox->value();
    this->m_ModulationFrequency = this->m_ToFImageGrabber->SetModulationFrequency(this->m_ModulationFrequency);
    if (active)
    {
      m_ToFImageGrabber->StartCamera();
    }
  }
}
*/
