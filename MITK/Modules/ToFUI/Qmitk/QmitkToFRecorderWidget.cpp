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

#define _USE_MATH_DEFINES
#include "QmitkToFRecorderWidget.h"

//QT headers
#include <qmessagebox.h>
#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QGridLayout>
#include <QCheckBox>

//mitk headers
#include <mitkToFImageWriter.h>

//itk headers
#include <itkEventObject.h>
#include <itksys/SystemTools.hxx>

struct QFileDialogArgs;
class QFileDialogPrivate;

const std::string QmitkToFRecorderWidget::VIEW_ID = "org.mitk.views.qmitktofrecorderwidget";

QmitkToFRecorderWidget::QmitkToFRecorderWidget(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
  this->m_ToFImageRecorder = NULL;
  this->m_ToFImageGrabber = NULL;
  this->m_RecordMode = mitk::ToFImageRecorder::PerFrames;

  this-> m_Controls = NULL;
  CreateQtPartControl(this);
}

QmitkToFRecorderWidget::~QmitkToFRecorderWidget()
{
}

void QmitkToFRecorderWidget::CreateQtPartControl(QWidget *parent)
{
  if (!m_Controls)
  {
    // create GUI widgets
    this->m_Controls = new Ui::QmitkToFRecorderWidgetControls;
    this->m_Controls->setupUi(parent);
    this->CreateConnections();
  }
}

void QmitkToFRecorderWidget::CreateConnections()
{
  if ( m_Controls )
  {
    connect( (QObject*)(m_Controls->m_PlayButton), SIGNAL(clicked()),(QObject*) this, SLOT(OnPlay()) );
    connect( (QObject*)(m_Controls->m_StopButton), SIGNAL(clicked()),(QObject*) this, SLOT(OnStop()) );
    connect( (QObject*)(m_Controls->m_StartRecordingButton), SIGNAL(clicked()),(QObject*) this, SLOT(OnStartRecorder()) );
    connect( (QObject*)(m_Controls->m_RecordModeComboBox), SIGNAL(currentIndexChanged(int)),(QObject*) this, SLOT(OnChangeRecordModeComboBox(int)) );    

    connect(this, SIGNAL(RecordingStopped()), this, SLOT(OnRecordingStopped()), Qt::BlockingQueuedConnection);
  }
}

void QmitkToFRecorderWidget::SetParameter(mitk::ToFImageGrabber* ToFImageGrabber, mitk::ToFImageRecorder* toFImageRecorder)
{
  this->m_ToFImageGrabber = ToFImageGrabber;

  this->m_ToFImageRecorder = toFImageRecorder;

  this->m_StopRecordingCommand = CommandType::New();
  this->m_StopRecordingCommand->SetCallbackFunction(this, &QmitkToFRecorderWidget::StopRecordingCallback);
  this->m_ToFImageRecorder->RemoveAllObservers();
  this->m_ToFImageRecorder->AddObserver(itk::AbortEvent(), this->m_StopRecordingCommand);

  m_Controls->m_PlayButton->setChecked(false);
  m_Controls->m_PlayButton->setEnabled(true);
  m_Controls->m_StartRecordingButton->setChecked(false);
  m_Controls->m_RecorderGroupBox->setEnabled(true);
}

void QmitkToFRecorderWidget::StopRecordingCallback()
{
  emit RecordingStopped();
}

void QmitkToFRecorderWidget::ResetGUIToInitial()
{
  m_Controls->m_PlayButton->setChecked(false);
  m_Controls->m_PlayButton->setEnabled(true);
  m_Controls->m_RecorderGroupBox->setEnabled(false);
}

void QmitkToFRecorderWidget::OnRecordingStopped()
{  
  m_Controls->m_StartRecordingButton->setChecked(false);
  m_Controls->m_RecorderGroupBox->setEnabled(true);
}

void QmitkToFRecorderWidget::OnStop()
{
  StopCamera();
  StopRecorder();

  ResetGUIToInitial();

  emit ToFCameraStopped();
}

void QmitkToFRecorderWidget::OnPlay()
{
  m_Controls->m_PlayButton->setChecked(true);
  m_Controls->m_PlayButton->setEnabled(false);
  m_Controls->m_RecorderGroupBox->setEnabled(true);
  this->repaint();

  StartCamera();

  emit ToFCameraStarted();
}

void QmitkToFRecorderWidget::StartCamera()
{
  bool ok = false;
  if (!m_ToFImageGrabber->IsCameraActive())
  {
    m_ToFImageGrabber->StartCamera();
  }
}

void QmitkToFRecorderWidget::StopCamera()
{
  m_ToFImageGrabber->StopCamera();
}

void QmitkToFRecorderWidget::StopRecorder()
{
  this->m_ToFImageRecorder->StopRecording();
}

void QmitkToFRecorderWidget::OnStartRecorder()
{
  m_Controls->m_StartRecordingButton->setChecked(true);
  m_Controls->m_RecorderGroupBox->setEnabled(false);
  this->repaint();

  int numOfFrames = m_Controls->m_NumOfFramesSpinBox->value();
  try
  {
    bool fileOK = true;
    bool distanceImageSelected = true;
    bool amplitudeImageSelected = true;
    bool intensityImageSelected = true;
    bool rawDataSelected = false;

    QString tmpFileName("");
    QString selectedFilter("");
    QString imageFileName("");
    mitk::ToFImageWriter::ToFImageType tofImageType;
    tmpFileName = QmitkToFRecorderWidget::getSaveFileName(tofImageType, 
      distanceImageSelected, amplitudeImageSelected, intensityImageSelected, rawDataSelected,
      NULL, "Save Image To...", imageFileName, "MITK Images (*.pic);;Text (*.csv)", &selectedFilter);

    if (tmpFileName.isEmpty())
    {
      fileOK = false;
    }
    else
    {
      imageFileName = tmpFileName;
    }

    if (fileOK)
    {
      std::string dir = itksys::SystemTools::GetFilenamePath( imageFileName.toStdString() );
      std::string baseFilename = itksys::SystemTools::GetFilenameWithoutLastExtension( imageFileName.toStdString() );
      std::string extension = itksys::SystemTools::GetFilenameLastExtension( imageFileName.toStdString() );

      int integrationTime = this->m_ToFImageGrabber->GetIntegrationTime();
      int modulationFreq = this->m_ToFImageGrabber->GetModulationFrequency();

      QString integrationTimeStr;
      integrationTimeStr.setNum(integrationTime);
      QString modulationFreqStr;
      modulationFreqStr.setNum(modulationFreq);
      QString numOfFramesStr("");
      if (this->m_RecordMode == mitk::ToFImageRecorder::PerFrames)
      {
        numOfFramesStr.setNum(numOfFrames);
      }

      std::string distImageFileName = prepareFilename(dir, baseFilename, modulationFreqStr.toStdString(), 
        integrationTimeStr.toStdString(), numOfFramesStr.toStdString(), extension, "_DistanceImage");
      MITK_INFO << "Save distance data to: " << distImageFileName;

      std::string amplImageFileName = prepareFilename(dir, baseFilename, modulationFreqStr.toStdString(), 
        integrationTimeStr.toStdString(), numOfFramesStr.toStdString(), extension, "_AmplitudeImage");
      MITK_INFO << "Save amplitude data to: " << amplImageFileName;

      std::string intenImageFileName = prepareFilename(dir, baseFilename, modulationFreqStr.toStdString(), 
        integrationTimeStr.toStdString(), numOfFramesStr.toStdString(), extension, "_IntensityImage");
      MITK_INFO << "Save intensity data to: " << intenImageFileName;

      if (selectedFilter.compare("Text (*.csv)") == 0)
      {
        this->m_ToFImageRecorder->SetFileFormat("csv");
      }
      else
      {
        //default
        this->m_ToFImageRecorder->SetFileFormat("pic");
      }

      numOfFrames = m_Controls->m_NumOfFramesSpinBox->value();
      this->m_ToFImageRecorder->SetDistanceImageFileName(distImageFileName);
      this->m_ToFImageRecorder->SetAmplitudeImageFileName(amplImageFileName);
      this->m_ToFImageRecorder->SetIntensityImageFileName(intenImageFileName);
      this->m_ToFImageRecorder->SetToFImageType(tofImageType);
      this->m_ToFImageRecorder->SetDistanceImageSelected(distanceImageSelected);
      this->m_ToFImageRecorder->SetAmplitudeImageSelected(amplitudeImageSelected);
      this->m_ToFImageRecorder->SetIntensityImageSelected(intensityImageSelected);
      this->m_ToFImageRecorder->SetRecordMode(this->m_RecordMode);
      this->m_ToFImageRecorder->SetNumOfFrames(numOfFrames);

      this->m_ToFImageRecorder->StartRecording();
    }
    else
    {
      OnRecordingStopped();
    }
  }
  catch(std::exception& e)
  {
    QMessageBox::critical(NULL, "Error", QString(e.what()));
    OnRecordingStopped();
  }
}

QString QmitkToFRecorderWidget::getSaveFileName(mitk::ToFImageWriter::ToFImageType& tofImageType,
                                     bool& distanceImageSelected,
                                     bool& amplitudeImageSelected,
                                     bool& intensityImageSelected,
                                     bool& rawDataSelected,
                                     QWidget *parent,
                                     const QString &caption,
                                     const QString &dir,
                                     const QString &filter,
                                     QString *selectedFilter,
                                     QFileDialog::Options options
                                     )
{
  QString selectedFileName = "";
  QComboBox* combo = new QComboBox;
  combo->addItem("3D");
  combo->addItem("2D + t");

  QHBoxLayout* checkBoxGroup = new QHBoxLayout();

  QCheckBox* distanceImageCheckBox = new QCheckBox;
  distanceImageCheckBox->setText("Distance image");
  distanceImageCheckBox->setChecked(true);
  QCheckBox* amplitudeImageCheckBox = new QCheckBox;
  amplitudeImageCheckBox->setText("Amplitude image");
  amplitudeImageCheckBox->setChecked(true);
  QCheckBox* intensityImageCheckBox = new QCheckBox;
  intensityImageCheckBox->setText("Intensity image");
  intensityImageCheckBox->setChecked(true);
  QCheckBox* rawDataCheckBox = new QCheckBox;
  rawDataCheckBox->setText("Raw data");
  rawDataCheckBox->setChecked(false);
  rawDataCheckBox->setEnabled(false);

  checkBoxGroup->addWidget(distanceImageCheckBox);
  checkBoxGroup->addWidget(amplitudeImageCheckBox);
  checkBoxGroup->addWidget(intensityImageCheckBox);
  checkBoxGroup->addWidget(rawDataCheckBox);

  QFileDialog* fileDialog = new QFileDialog(parent, caption, dir, filter);

  QLayout* layout = fileDialog->layout();
  QGridLayout* gridbox = qobject_cast<QGridLayout*>(layout);

  if (gridbox) 
  {
    gridbox->addWidget(new QLabel("ToF-Image type:"));
    gridbox->addWidget(combo);
    int lastRow = gridbox->rowCount();
    gridbox->addLayout(checkBoxGroup, lastRow, 0, 1, -1);
  }

  fileDialog->setLayout(gridbox);
  fileDialog->setAcceptMode(QFileDialog::AcceptSave);

  if (selectedFilter)
  {
    fileDialog->selectNameFilter(*selectedFilter);
  }

  if (fileDialog->exec() == QDialog::Accepted) 
  {
    if (selectedFilter)
    {
      *selectedFilter = fileDialog->selectedFilter();
    }

    if (combo->currentIndex() == 0)
    {
      tofImageType = mitk::ToFImageWriter::ToFImageType3D;
    }

    else
    {
      tofImageType = mitk::ToFImageWriter::ToFImageType2DPlusT;
    }

    distanceImageSelected = distanceImageCheckBox->isChecked();
    amplitudeImageSelected = amplitudeImageCheckBox->isChecked();
    intensityImageSelected = intensityImageCheckBox->isChecked();
    rawDataSelected = rawDataCheckBox->isChecked();

    selectedFileName = fileDialog->selectedFiles().value(0);
  }

  delete fileDialog;
  return selectedFileName;
}

std::string QmitkToFRecorderWidget::prepareFilename(std::string dir, 
                                                    std::string baseFilename, 
                                                    std::string modulationFreq, 
                                                    std::string integrationTime, 
                                                    std::string numOfFrames, 
                                                    std::string extension, 
                                                    std::string imageType)
{
  std::string filenName("");
  filenName.append(dir);
  filenName.append("/");
  filenName.append(baseFilename);
  filenName.append("_MF");
  filenName.append(modulationFreq);
  filenName.append("_IT");
  filenName.append(integrationTime);
  filenName.append("_");
  filenName.append(numOfFrames);
  filenName.append("Images");
  filenName.append(imageType);
  filenName.append(extension);
  return filenName;
}

void QmitkToFRecorderWidget::OnChangeRecordModeComboBox(int index)
{
  if (index == 0)
  {
    this->m_RecordMode = mitk::ToFImageRecorder::PerFrames;
    m_Controls->m_NumOfFramesSpinBox->setEnabled(true);
  }
  else
  {
    this->m_RecordMode = mitk::ToFImageRecorder::Infinite;
    m_Controls->m_NumOfFramesSpinBox->setEnabled(false);
  }
}
