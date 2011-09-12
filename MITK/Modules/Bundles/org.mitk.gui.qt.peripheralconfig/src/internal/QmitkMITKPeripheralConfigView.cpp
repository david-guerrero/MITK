/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date$
Version:   $Revision$ 
 
Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// Blueberry
#include <berryISelectionService.h>
#include <berryIWorkbenchWindow.h>

// Qmitk
#include "QmitkMITKPeripheralConfigView.h"
#include "QmitkStdMultiWidget.h"
#include "mitkPeripheralConfigActivator.h"

// Qt
#include <QMessageBox>
#include <QFileDialog>
#include "mitkLogMacros.h"

const std::string QmitkMITKPeripheralConfigView::VIEW_ID = "org.mitk.views.mitkperipheralconfig";

QmitkMITKPeripheralConfigView::QmitkMITKPeripheralConfigView()
: QmitkFunctionality()
, m_Controls( 0 )
, m_MultiWidget( NULL )
, m_DeviceIsSelected(false)
, m_XMLPropertiesFileName("PerConfigProperties.tofconfig")
,	m_XMLAccessoriesFileName("PerConfigAccessories.tofconfig")
,	m_XMLDevicesFileName("PerConfigDevices.tofconfig")
{
	m_Reader = mitk::MITKPeripheralConfigFileReader::New();
	m_Writer = mitk::MITKPeripheralConfigFileWriter::New();

	m_DefaultPath = QString(mitk::PeripheralConfigActivator::m_PersistentDir.c_str());
	
}

QmitkMITKPeripheralConfigView::~QmitkMITKPeripheralConfigView()
{
}

void QmitkMITKPeripheralConfigView::CreateQtPartControl( QWidget *parent )
{
  // Build up qt view, unless already done
  if ( !m_Controls )
  {
    //Create GUI widgets from the Qt Designer's .ui file
    m_Controls = new Ui::QmitkMITKPeripheralConfigViewControls;
    m_Controls->setupUi( parent );
 
		//Generate config file button pressed
		QObject::connect( m_Controls->m_GenConfigFilePushButton, SIGNAL(clicked()), this, SLOT(OnGenerateConfigFileButtonClicked()) );

		//Browse File Path
		QObject::connect( m_Controls->m_BrowsePathButton, SIGNAL(clicked()), this, SLOT(OnButtonClickedCreateFileDialog()));
		
		//Existing property toggled
		QObject::connect( m_Controls->m_ExistingPropertyGroupBox, SIGNAL(toggled(bool)), 
											this, SLOT(OnAddExistingPropertyToggled(bool)));
		//Add new property toggled
		QObject::connect( m_Controls->m_NewPropertyGroupBox, SIGNAL(toggled(bool)), 
											this, SLOT(OnNewPropertyToggled(bool)));
		
		//Property type selected
		QObject::connect( m_Controls->m_NewPropertyTypeComboBox, SIGNAL(activated(int)), 
											this, SLOT(OnCommandBoxPropertyTypeSelected()));
		
		//Existing property combo box toggled
		QObject::connect( m_Controls->m_ExistingPropertyComboBox, SIGNAL(activated(int)),
											this, SLOT(OnExistingListToggled(int)));
		//Selecting device
		QObject::connect( m_Controls->m_DeviceSelectionSelectButton, SIGNAL(clicked()), 
											this, SLOT(OnSelectDeviceButtonClicked()));

		//Adding new device
		QObject::connect( m_Controls->m_NewDeviceAddButton, SIGNAL(clicked()), 
											this, SLOT(OnAddNewDeviceButtonClicked()));

		//Device combo box toggle
		QObject::connect( m_Controls->m_DeviceSelectionComboBox, SIGNAL(activated(int)), 
											this, SLOT(OnDeviceSelectionBoxToggled(int)));

		//Accessory combo box toggle
		QObject::connect( m_Controls->m_AccessorySelectionComboBox, SIGNAL(activated(int)), 
											this, SLOT(OnAccessorySelectionBoxToggled(int)));
	
		//Select accessory button clicked
		QObject::connect( m_Controls->m_AccessorySelectionSelectButton, SIGNAL(clicked()), 
											this, SLOT(OnSelectAccessoryButtonClicked()));

		//Select accessory button clicked
		QObject::connect( m_Controls->m_AddPropertyAddButton, SIGNAL(clicked()), 
											this, SLOT(OnAddPropertyButtonClicked()));
		
		//Device Property table was changed
		QObject::connect( m_Controls->m_EditPropertiesTableWidget, SIGNAL(currentCellChanged(int ,int, int, int)),
											this, SLOT(OnTableChanged(int, int, int, int)));

		//Accessory Property table was changed
		QObject::connect( m_Controls->m_EditAccessoryPropertiesTableWidget, SIGNAL(currentCellChanged(int ,int, int, int)),
											this, SLOT(OnAccessoryTableChanged(int, int, int, int)));

		//Edit properties table selected
		QObject::connect( m_Controls->toolBox, SIGNAL(currentChanged(int)),
											this, SLOT(OnPropertiesTableRefresh(int)));
		
		//Remove device button clicked
		QObject::connect( m_Controls->m_RemoveButton, SIGNAL(clicked()),
											this, SLOT(OnRemoveDeviceButtonClicked()));

		//Remove Property button clicked
		QObject::connect( m_Controls->m_RemovePropertiesButton, SIGNAL(clicked()),
											this, SLOT(OnRemovePropertyButtonClicked()));

		//Calibration file path button clicked
		QObject::connect( m_Controls->m_BrowseCalibrationFilePathButton, SIGNAL(clicked()),
											this, SLOT(OnCalibrationFilePathButtonClicked()));
		
		//Calibration file select button clicked
		QObject::connect( m_Controls->m_CalibrationFileSelectButton, SIGNAL(clicked()),
											this, SLOT(OnCalibrationFileSelectButtonClicked()));
		
		this->OnInitializingGUI();
		this->OnLoadingInformation();
			
	}
}

void QmitkMITKPeripheralConfigView::OnInitializingGUI()
{
	//Disable, hide and initialize GUI elements
	m_Controls->toolBox->setCurrentIndex(0);
	m_Controls->m_DefaultValueGroupBox->setVisible(false);
	m_Controls->m_AddPropertyAddButton->setEnabled(false);
	m_Controls->m_NewPropertyTypeComboBox->setCurrentIndex(-1);
	m_Controls->m_ExistingPropertyComboBox->setCurrentIndex(-1);
	m_Controls->m_NewDeviceConfigFilePathLineEdit->setEnabled(false);
	m_Controls->m_NewDeviceNameLineEdit->setEnabled(false);
	m_Controls->m_BrowsePathButton->setEnabled(false);
	m_Controls->m_NewDeviceAddButton->setEnabled(false);
	m_Controls->m_NewDeviceAddButton->setVisible(false);
	m_Controls->m_DeviceSelectionSelectButton->setEnabled(false);
	m_Controls->m_DeviceSelectionSelectButton->setVisible(false);
	m_Controls->m_NewDeviceConfigFilePathLineEdit->setText("");
	m_Controls->m_AccessoryFrame->setVisible(false);
	m_Controls->m_AccessoryFrame->setEnabled(false);
	m_Controls->m_AccessoryCheckBox->setEnabled(false);
	m_Controls->m_AccessoryCheckBox->setVisible(false);
	m_Controls->m_RemoveButton->setEnabled(false);
	m_Controls->m_RemoveButton->setVisible(false);
	m_Controls->m_RemovePropertiesButton->setEnabled(false);
	m_Controls->m_RemovePropertiesButton->setVisible(false);
	m_Controls->m_EditPropertiesTableWidget->setSortingEnabled(true);
	
	//Disable secondary tabs
	this->OnDeviceSelectedEnablePropertiesAndParameters(false);
	this->OnEnableCalibrationFileSection(false);

	

	//Initialize file paths
	m_XMLPropertiesFileName = QString(m_XMLPropertiesFileName.c_str()).prepend(m_DefaultPath).toStdString();
	m_XMLAccessoriesFileName = QString(m_XMLAccessoriesFileName.c_str()).prepend(m_DefaultPath).toStdString();
	m_XMLDevicesFileName = QString(m_XMLDevicesFileName.c_str()).prepend(m_DefaultPath).toStdString();
}


void QmitkMITKPeripheralConfigView::OnLoadingInformation()
{
	//Load Global lists from the xml files on the default folder.
	m_Reader->SetFileName(m_XMLPropertiesFileName.c_str());
	m_Properties = m_Reader->LoadPropertyList();
	m_Reader->SetFileName(m_XMLAccessoriesFileName.c_str());
	m_AccessoryContainer = m_Reader->LoadAccessoryList();
	m_Reader->SetFileName(m_XMLDevicesFileName.c_str());
	m_DeviceContainer = m_Reader->LoadDeviceList();

	//Generate device combo box lists
	this->OnGeneratedDeviceSelectionComboBoxOptions();
	//Generate accessory combo box lists
	this->OnGeneratedAccessorySelectionComboBoxOptions();
}

//DEVICE SELECTION SECTION 
void QmitkMITKPeripheralConfigView::OnGeneratedDeviceSelectionComboBoxOptions()
{
	//Clear combo box list
	m_Controls->m_DeviceSelectionComboBox->clear();
	m_Controls->m_DeviceSelectionComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);
	
	std::map< std::string, mitk::MITKPeripheralInfo::Pointer >::iterator itDevice;
	std::map< std::string, mitk::MITKPeripheralAccessory::Pointer >::iterator itAccessory;
	QVariant deviceBool(false);
	QVariant accessoryBool(true);
	
	//Iterate through the device list and add each device's name and type as an entry in the combo box list.
	if(m_DeviceContainer.size() > 0)
	{
		for(itDevice = m_DeviceContainer.begin(); itDevice != m_DeviceContainer.end(); itDevice++)
		{
			m_Controls->m_DeviceSelectionComboBox->addItem(QString(itDevice->first.c_str()), deviceBool);
		}
	}
	
	//Iterate through the accessory list and add each accessory's name and type as an entry in the combo box list.
	if(m_AccessoryContainer.size() > 0)
	{
		for(itAccessory = m_AccessoryContainer.begin(); itAccessory != m_AccessoryContainer.end(); itAccessory++)
		{
			m_Controls->m_DeviceSelectionComboBox->addItem(QString(itAccessory->first.c_str()), accessoryBool);
		}
	}
	
	//Add the two default options
	m_Controls->m_DeviceSelectionComboBox->insertItem(0,"No Device Selected",deviceBool);
	m_Controls->m_DeviceSelectionComboBox->insertItem(1,"Add New Device",deviceBool);
	m_Controls->m_DeviceSelectionComboBox->setCurrentIndex(0);

}

void QmitkMITKPeripheralConfigView::OnDeviceSelectionBoxToggled(int index)
{
	//Reset GUI elements and flags
	m_DeviceIsSelected = false;
	m_FileName.clear();
	m_SelectedDevice = NULL;
	m_SelectedAccessory = NULL;

	//Disable Accessory GUI elements
	this->OnAccessoryDataCleared();

	//Disable properties value line
	m_Controls->m_ValueLineEdit->setEnabled(false);
	m_Controls->m_ValueLineEdit->clear();

	//Clear data from name and combo box for new property
	m_Controls->m_NewPropertyGroupBox->setChecked(false);
	m_Controls->m_NewPropertyNameLineEdit->clear();
	m_Controls->m_NewPropertyTypeComboBox->setCurrentIndex(-1);

	//Clear data from existing property
	m_Controls->m_ExistingPropertyGroupBox->setChecked(false);
	m_Controls->m_ExistingPropertyComboBox->setCurrentIndex(-1);

	//Disable remove button
	m_Controls->m_RemoveButton->setEnabled(false);
	m_Controls->m_RemoveButton->setVisible(false);
	m_Controls->m_AccessoryCheckBox->setCheckState(Qt::Unchecked);

	OnEnableCalibrationFileSection(false);
	
	//Clear file path
	m_Controls->m_NewDeviceConfigFilePathLineEdit->setText("");

	//Disable Accessory GUI elements
	this->OnDeviceSelectedGenerateAccessoryGUI(false);

	//Disable other tabs
	this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);

	
	if(index == 0)
	{
		//No Device Selected option
		//Disable line edits
		m_Controls->m_NewDeviceNameLineEdit->setEnabled(false);
		m_SelectedDevice = false;
		//turn off select button
		m_Controls->m_DeviceSelectionSelectButton->setEnabled(false);
		m_Controls->m_DeviceSelectionSelectButton->setVisible(false);
		//Turn off add button
		m_Controls->m_NewDeviceAddButton->setEnabled(false);
		m_Controls->m_NewDeviceAddButton->setVisible(false);
		//Turn off filedialog button
		m_Controls->m_BrowsePathButton->setEnabled(false);
		//Turn off checkbox
		m_Controls->m_AccessoryCheckBox->setEnabled(false);
		m_Controls->m_AccessoryCheckBox->setVisible(false);
		//Delete old data from line edits
		m_Controls->m_NewDeviceNameLineEdit->setText("");
		m_Controls->m_NewDeviceConfigFilePathLineEdit->setText("");
		
	}
	else if(index == 1)
	{
		//Add new device
		//Turn on checkbox
		m_Controls->m_AccessoryCheckBox->setEnabled(true);
		m_Controls->m_AccessoryCheckBox->setVisible(true);
		//Turn on filedialog button
		m_Controls->m_BrowsePathButton->setEnabled(true);
		m_Controls->m_BrowsePathButton->setVisible(true);
		//Turn on add button
		m_Controls->m_NewDeviceAddButton->setEnabled(true);
		m_Controls->m_NewDeviceAddButton->setVisible(true);
		//Turn off select button
		m_Controls->m_DeviceSelectionSelectButton->setEnabled(false);
		m_Controls->m_DeviceSelectionSelectButton->setVisible(false);
		//Enable qdialog and line edits
		m_Controls->m_NewDeviceNameLineEdit->setEnabled(true);
		m_Controls->m_BrowsePathButton->setEnabled(true);
		//Delete old data from global variables
		m_Controls->m_NewDeviceNameLineEdit->setText("");
		m_Controls->m_NewDeviceConfigFilePathLineEdit->setText("");
		m_FileName.clear();
	}
	else
	{
		//Device Selected
		m_Controls->m_NewDeviceNameLineEdit->setEnabled(false);
		m_Controls->m_BrowsePathButton->setEnabled(false);
		//Turn off checkbox
		m_Controls->m_AccessoryCheckBox->setEnabled(false);
		m_Controls->m_AccessoryCheckBox->setVisible(false);
		//Turn on filedialog button
		m_Controls->m_BrowsePathButton->setVisible(true);
		//Turn on select button
		m_Controls->m_DeviceSelectionSelectButton->setEnabled(true);
		m_Controls->m_DeviceSelectionSelectButton->setVisible(true);
		//Turn off add button
		m_Controls->m_NewDeviceAddButton->setEnabled(false);
		m_Controls->m_NewDeviceAddButton->setVisible(false);
		//Set new data
		std::string path;
		std::string deviceName = m_Controls->m_DeviceSelectionComboBox->currentText().toStdString();
		m_Controls->m_NewDeviceNameLineEdit->setText(QString(deviceName.c_str()));
		
	
		//Check if it is an accessory.
		bool isAccessory = m_Controls->m_DeviceSelectionComboBox->itemData(index).toBool();
		if(isAccessory)
		{
			//turn on checkbox
			m_Controls->m_AccessoryCheckBox->setCheckState(Qt::Checked);
			m_Controls->m_AccessoryCheckBox->setVisible(true);
			if(m_AccessoryContainer.find(deviceName)!= m_AccessoryContainer.end())
			{
				//Get the original file path
				mitk::MITKPeripheralAccessory::Pointer acc = m_AccessoryContainer.find(deviceName)->second;
				if(acc.IsNotNull() && acc->HasProperty("File Location"))
				{
					path = acc->GetPropertyStringValue("File Location");
				}
			}
		}
		else
		{
			//Get the original file path
			if(m_DeviceContainer.find(deviceName)!= m_DeviceContainer.end())
			{
				mitk::MITKPeripheralInfo::Pointer dev = m_DeviceContainer.find(deviceName)->second;
				if(dev.IsNotNull() && dev->HasProperty("File Location"))
				{
					path = dev->GetPropertyStringValue("File Location");
				}
			}
		}


		m_Controls->m_NewDeviceConfigFilePathLineEdit->setText(QString(path.c_str()));
	}
}


void QmitkMITKPeripheralConfigView::OnSelectDeviceButtonClicked()
{
	//Get the file path from the line edit.
	QString deviceFilePath = m_Controls->m_NewDeviceConfigFilePathLineEdit->text();
	//Get the name of the requested device/accessory
	QString deviceName = m_Controls->m_DeviceSelectionComboBox->currentText();

	if(deviceFilePath.isEmpty())
	{
		deviceFilePath = m_DefaultPath;
		if(m_FileName.isNull() || m_FileName.isEmpty() )
		{
			//Build proper file name
			m_FileName = deviceFilePath + deviceName.simplified() + ".tofconfig";
		}
	}
	else
	{
		//Get filename from the device properties
		m_FileName = deviceFilePath;
	}
		
	m_Controls->m_NewDeviceNameLineEdit->setText(deviceName);
	
	//Check if the file exists for the selected device.
	FILE* file = fopen(m_FileName.toStdString().c_str(),"r+");
	if(file == NULL)
	{
		this->GenerateAlertBox("There was an error opening the file or the file was not found.\t\n Please select the path of the corresponding config file!");
		
		//If it does not exist enable file dialog for user to look for containing file.
		m_Controls->m_BrowsePathButton->setEnabled(true);
		m_FileName.clear();
	}
	else
	{
		m_Controls->m_NewDeviceConfigFilePathLineEdit->setText(m_FileName);
		fclose(file);
		//Disable select button
		m_Controls->m_DeviceSelectionSelectButton->setVisible(false);
		m_Controls->m_DeviceSelectionSelectButton->setEnabled(false);
		bool isChecked = m_Controls->m_AccessoryCheckBox->checkState() == Qt::Checked;

		if(!isChecked)
		{
			//The selected option is a Device
			mitk::MITKPeripheralInfo::Pointer itDevice;
			m_Reader->SetFileName(m_FileName.toStdString().c_str());
			itDevice = m_Reader->LoadConfigFile();
			if(itDevice->GetDeviceName().compare("") == 0)
			{
				//After the decvice's config file is loaded, if the file is empty, look for it on the global list.
				if(m_DeviceContainer.find(deviceName.toStdString())!= m_DeviceContainer.end())
				{
					itDevice = m_DeviceContainer.find(deviceName.toStdString())->second;
				}
				else
				{
					//If it is in neither, it's an error so remove it from the combo box list.
					this->GenerateAlertBox("There has been an error with the selected device.\n The file might have been erased or moved.\n Please Select another device.\n\n The file might not exist yet or has not been generated!");
					this->m_DeviceIsSelected = false;
					this->OnGeneratedDeviceSelectionComboBoxOptions();
					this->OnDeviceSelectionBoxToggled(0);
					this->OnDeviceSelectedGenerateAccessoryGUI(false);
					this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);
					
					return;
				}
			}
			//Check if device exists
			if(itDevice.IsNotNull())
			{
				itDevice->RemoveProperty("File Location");
				itDevice->AddProperty("File Location", mitk::MITKPeripheralProperty::PString, m_Controls->m_NewDeviceConfigFilePathLineEdit->text().toStdString(), false, 0.0);
		
				//Update device on the global list. After this point, the global list and the current config file are synchronized.
				m_DeviceContainer.erase(m_DeviceContainer.find(deviceName.toStdString()));
				std::pair<std::string, mitk::MITKPeripheralInfo::Pointer> entry(deviceName.toStdString(), itDevice);
				m_DeviceContainer.insert(entry);
			}
			//Actually select the device
			if(m_DeviceContainer.find(deviceName.toStdString()) != m_DeviceContainer.end())
			{
				m_SelectedDevice = m_DeviceContainer.find(deviceName.toStdString())->second;
				m_DeviceIsSelected = true;
				//Enable remove button
				m_Controls->m_RemoveButton->setEnabled(true);
				m_Controls->m_RemoveButton->setVisible(true);
				//Change to the edit properties tab
				m_Controls->toolBox->setCurrentIndex(1);
			}
			else
			{
				this->GenerateAlertBox("There has been an error with the selected device.\n The file might have been erased or moved.\n Please Select another device.\n\n The file might not exist yet or has not been generated!");
				//Unselect everything
				this->m_DeviceIsSelected = false;
				this->OnGeneratedDeviceSelectionComboBoxOptions();
				this->OnDeviceSelectionBoxToggled(0);
				this->OnDeviceSelectedGenerateAccessoryGUI(false);
				this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);
				return;
			}
		}
		else
		{
			//The selected option is an accessory
			mitk::MITKPeripheralAccessory::Pointer itAccessory;

			m_Reader->SetFileName(m_FileName.toStdString().c_str());
			//Read file. If it is empty, look for it on the global list of accessories
			itAccessory = m_Reader->LoadAccessoryConfigFile();
			if(itAccessory->GetAccessoryName().compare("") == 0)
			{
				if(m_AccessoryContainer.find(deviceName.toStdString())!= m_AccessoryContainer.end())
				{
					itAccessory = m_AccessoryContainer.find(deviceName.toStdString())->second;
				}
				else
				{
					//If it is also not on the global list, remove it from the combo box list.
					this->GenerateAlertBox("There has been an error with the selected device.\n The file might have been erased or moved.\n Please Select another device.\n\n The file might not exist yet or has not been generated!");
					this->m_DeviceIsSelected = false;
					this->OnGeneratedDeviceSelectionComboBoxOptions();
					this->OnDeviceSelectionBoxToggled(0);
					this->OnDeviceSelectedGenerateAccessoryGUI(false);
					this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);
					return;
				}
			}

			//Check if device exists
			if(itAccessory.IsNotNull())
			{
				//Add the file location to the properties. IT always uses the current location of the file to assign it. Old locations are not saved.
				itAccessory->RemoveProperty("File Location");
				itAccessory->AddProperty("File Location", mitk::MITKPeripheralProperty::PString, m_Controls->m_NewDeviceConfigFilePathLineEdit->text().toStdString(), false, 0.0);
		
				//At this point, the file and global list are synchronized.
				m_AccessoryContainer.erase(m_AccessoryContainer.find(deviceName.toStdString()));
				std::pair<std::string, mitk::MITKPeripheralAccessory::Pointer> entry(itAccessory->GetAccessoryName(), itAccessory);
				m_AccessoryContainer.insert(entry);
			}
		
			//Actually select Accessory
			if(m_AccessoryContainer.find(deviceName.toStdString()) != m_AccessoryContainer.end())
			{
				m_SelectedAccessory = m_AccessoryContainer.find(deviceName.toStdString())->second;
				m_DeviceIsSelected = true;
				//Enable remove button
				m_Controls->m_RemoveButton->setEnabled(true);
				m_Controls->m_RemoveButton->setVisible(true);
				//Change to the edit properties tab
				m_Controls->toolBox->setCurrentIndex(1);
			}
			else
			{
				this->GenerateAlertBox("There has been an error with the selected device.\n The file might have been erased or moved.\n Please Select another device.");
				//Unselect everything
				this->m_DeviceIsSelected = false;
				this->OnGeneratedDeviceSelectionComboBoxOptions();
				this->OnDeviceSelectionBoxToggled(0);
				this->OnDeviceSelectedGenerateAccessoryGUI(false);
				this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);
				return;
			}
		}
		//If the data on the combo box is a bool indicating whether the option is an accessory or not.
		if(!m_Controls->m_DeviceSelectionComboBox->itemData(m_Controls->m_DeviceSelectionComboBox->currentIndex()).toBool())
		{
			//If it is not then generate the select accessory GUI elements
			this->OnDeviceSelectedGenerateAccessoryGUI(true);
		}
	
	}
	//Enable other tabs.
	this->OnEnableCalibrationFileSection(this->m_DeviceIsSelected);
	this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);		
}

void QmitkMITKPeripheralConfigView::OnAddNewDeviceButtonClicked()
{
	//Check if the name box was left blank
	QString newDeviceName = this->m_Controls->m_NewDeviceNameLineEdit->text();
	if(newDeviceName.compare("")==0)
	{
		this->GenerateAlertBox("Please enter a name for the device.");
		return;
	}
	
	//Check if device already exists
	if(m_DeviceContainer.size() > 0)
	{
		if(m_DeviceContainer.find(newDeviceName.toStdString()) != m_DeviceContainer.end())
		{
			this->GenerateAlertBox("A device with such a name already exists.");
			this->m_DeviceIsSelected = false;
			this->OnGeneratedDeviceSelectionComboBoxOptions();
			this->OnDeviceSelectionBoxToggled(1);
			this->OnDeviceSelectedGenerateAccessoryGUI(false);
			this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);
			return;
		}
		else
		{
			//If no device with such a name exist, check for accessories for the same name.
			if(m_AccessoryContainer.size() > 0 && m_AccessoryContainer.find(newDeviceName.toStdString()) != m_AccessoryContainer.end())
			{
				this->GenerateAlertBox("An accessory with such a name already exists.");
				this->m_DeviceIsSelected = false;
				this->OnGeneratedDeviceSelectionComboBoxOptions();
				this->OnDeviceSelectionBoxToggled(1);
				this->OnDeviceSelectedGenerateAccessoryGUI(false);
				this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);
				return;
			}
		}
	}
	else
	{
		//If device list is empty, check for accessories for the same name.
		if(m_AccessoryContainer.size() > 0 && m_AccessoryContainer.find(newDeviceName.toStdString()) != m_AccessoryContainer.end())
		{
			this->GenerateAlertBox("An accessory with such a name already exists.");
			this->m_DeviceIsSelected = false;
			this->OnGeneratedDeviceSelectionComboBoxOptions();
			this->OnDeviceSelectionBoxToggled(1);
			this->OnDeviceSelectedGenerateAccessoryGUI(false);
			this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);
			return;
		}
	}
	
	//Update line edit info and global variables
	if(!m_Controls->m_NewDeviceConfigFilePathLineEdit->text().isEmpty())
	{
		//Get name from saved path
		m_FileName = m_Controls->m_NewDeviceConfigFilePathLineEdit->text();
	}
	else
	{
		//Get name by building the path
		m_FileName = MITK_ROOT + newDeviceName + ".tofconfig";
		//m_Controls->m_NewDeviceConfigFilePathLineEdit->setText("..." + Poco::Path::separator() + newDeviceName + ".tofConfig");
		//this->OnButtonClickedCreateFileDialog();
	}
	
	//Check if file already exists
	FILE* fileExists = fopen(m_FileName.toStdString().c_str(),"r");
	
	if(fileExists != NULL)
	{
		//Load the contents of the existing file.
		this->GenerateAlertBox("Warning: The file already exists.\t\n Its contents will be assigned to the new peripheral.");
		fclose(fileExists);
	}	
	else
	{
		//Create file with said name
		FILE* file = fopen(m_FileName.toStdString().c_str(),"w+");
		if(file == NULL)
		{
			this->GenerateAlertBox("There was an error creating.\t\n Please check the name and path and try again!");
			m_Controls->m_NewDeviceConfigFilePathLineEdit->setText("");
			this->m_DeviceIsSelected = false;
			this->OnGeneratedDeviceSelectionComboBoxOptions();
			this->OnDeviceSelectionBoxToggled(1);
			this->OnDeviceSelectedGenerateAccessoryGUI(false);
			this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);
			return;
		}
		fclose(file);
		//std::remove(m_FileName.toStdString().c_str());
	}

	bool isChecked = m_Controls->m_AccessoryCheckBox->checkState() == Qt::Checked;
	QVariant accessory(isChecked);

	m_Controls->m_NewDeviceAddButton->setVisible(false);
	m_Controls->m_NewDeviceAddButton->setEnabled(false);
	m_Controls->m_AccessoryCheckBox->setEnabled(false);
	m_Controls->m_BrowsePathButton->setEnabled(false);
	m_Controls->m_NewDeviceNameLineEdit->setEnabled(false);

	if(!isChecked)
	{
		//Add the file location as a property of the device.
		mitk::MITKPeripheralInfo::Pointer newDevice = mitk::MITKPeripheralInfo::New();
		newDevice->SetDeviceName(newDeviceName.toStdString());
		newDevice->AddProperty("File Location", mitk::MITKPeripheralProperty::PString, m_FileName.toStdString(), false, 0.0);
		
		//Update the device information in the global list.
		m_DeviceContainer.insert(std::pair<std::string, mitk::MITKPeripheralInfo::Pointer>(newDeviceName.toStdString(),newDevice));
		m_SelectedDevice = (m_DeviceContainer.find(newDeviceName.toStdString()))->second;
		this->OnGeneratedDeviceSelectionComboBoxOptions();
		
	}
	else
	{
		//Add the file location as a property of the accessory.
		mitk::MITKPeripheralAccessory::Pointer newDevice = mitk::MITKPeripheralAccessory::New();
		newDevice->SetAccessoryName(newDeviceName.toStdString());
		newDevice->AddProperty("File Location", mitk::MITKPeripheralProperty::PString, m_FileName.toStdString(), false, 0.0);
		
		//Update the accessory information in the global list.
		m_AccessoryContainer.insert(std::pair<std::string, mitk::MITKPeripheralAccessory::Pointer>(newDeviceName.toStdString(), newDevice));
		m_SelectedAccessory = (m_AccessoryContainer.find(newDeviceName.toStdString()))->second;
		this->OnGeneratedDeviceSelectionComboBoxOptions();
		this->OnGeneratedAccessorySelectionComboBoxOptions();
	}
	
	//Update GUI
	m_Controls->m_DeviceSelectionComboBox->setCurrentIndex(m_Controls->m_DeviceSelectionComboBox->findText(newDeviceName));
	this->OnDeviceSelectionBoxToggled(m_Controls->m_DeviceSelectionComboBox->findText(newDeviceName));
	
}

void QmitkMITKPeripheralConfigView::OnButtonClickedCreateFileDialog()
{
	//Open file dialog window and assign the contents to the GUI and global elements.
	m_FileName = QFileDialog::getOpenFileName(m_MultiWidget, "Open Config File", MITK_ROOT , tr("Configuration File (*.tofconfig)"), 0);
	m_Controls->m_NewDeviceConfigFilePathLineEdit->setText(m_FileName);
}

void QmitkMITKPeripheralConfigView::OnRemoveDeviceButtonClicked()
{
	//Ask for confirmation.
	int i = this->GenerateAlertBox("Are you sure you want to delete this device from the list?");
	if(i != QMessageBox::Ok)
	{
		return;
	}
	//Deselect accessory if not null
	std::string name;
	if(m_SelectedDevice.IsNotNull() && m_SelectedDevice.IsNotNull())
	{
		m_SelectedAccessory = mitk::MITKPeripheralAccessory::New();
	}
	//Disable all other tabs
	m_DeviceIsSelected = false;
	this->OnDeviceSelectedEnablePropertiesAndParameters(this->m_DeviceIsSelected);
	
	//Disable all other options in device selection tab
	this->OnDeviceSelectedGenerateAccessoryGUI(false);
	m_Controls->m_RemoveButton->setEnabled(false);
	m_Controls->m_RemoveButton->setVisible(false);
	int result = 0;

	//Remove from m_DeviceContainer if it is a device
	if(m_SelectedDevice.IsNotNull())
	{
		name = m_SelectedDevice->GetDeviceName();
		m_DeviceContainer.erase(name);
		
	}
	else
	{
		//Remove from m_AccessoryContainer if it is an accessory
		if(m_SelectedAccessory.IsNotNull())
		{
			name = m_SelectedAccessory->GetAccessoryName();
			m_AccessoryContainer.erase(name);
			m_SelectedAccessory = NULL;
		
			//Remove from all devices in the list?
			std::map< std::string, mitk::MITKPeripheralInfo::Pointer >::iterator itDevice;
			if(m_DeviceContainer.size() < 1)
			{
				return;
			}

			//If an accessory is removed, remove it from all the devices and rewrite their config files.
			for(itDevice = m_DeviceContainer.begin(); itDevice != m_DeviceContainer.end(); itDevice++)
			{
				itDevice->second->RemoveAccessory(name);

				std::string filepath = itDevice->second->GetPropertyStringValue("File Location");
				
				//Write config file.
				m_Writer->SetFileName(filepath.c_str());
				m_Writer->SetDevice(itDevice->second);
				result += m_Writer->WriteFile();
			}
		}
	}

	//Generate all lists and save the corresponding global files
	int resultp = 0;
	int resulta = 0;
	int resultd = 0;
	m_Writer->SetFileName(m_XMLPropertiesFileName.c_str());
	resultp = m_Writer->SavePropertyList(m_Properties);
	m_Writer->SetFileName(m_XMLAccessoriesFileName.c_str());
	resulta = m_Writer->SaveAccessoryList(m_AccessoryContainer);
	m_Writer->SetFileName(m_XMLDevicesFileName.c_str());
	resultd = m_Writer->SaveDeviceList(m_DeviceContainer);

	if(resultp != 0)
	{
		this->GenerateAlertBox("There was an error while writing the Properties' file.\n The file might have been deleted or moved.");
	}
	if(resulta != 0)
	{
		this->GenerateAlertBox("There was an error while writing the Accessories' file.\n The file might have been deleted or moved.");
	}
	if(resultd != 0)
	{
		this->GenerateAlertBox("There was an error while writing the Devices' file.\n The file might have been deleted or moved.");
	}
	if(result != 0)
	{
		this->GenerateAlertBox("The accessory was successfully removed.\n\n Warning:There were errors while writing one or more of the device files.");
	}
	
	this->OnLoadingInformation();
	i = this->GenerateAlertBox("The file might be shared by one or more devices. Do you want to remove it as well?");
	if(i != QMessageBox::Ok)
	{
		//Generate success alert box.
		if(std::remove(m_FileName.toStdString().c_str()) == 0)
		{
			
			std::string ss = "The peripheral " + name + " was successfully removed.";
			this->GenerateAlertBox(QString(ss.c_str()));
		}
		else
		{
			std::string ss = "The peripheral " + name + " was successfully removed.\n Its corresponding file was not found and was not removed.";
			this->GenerateAlertBox(QString(ss.c_str()));
		}
	}
	else
	{
		std::string ss = "The peripheral " + name + " was successfully removed.\n";
			this->GenerateAlertBox(QString(ss.c_str()));
	}

	//Deselect from selected device/accessory
	this->OnDeviceSelectionBoxToggled(0);
}

// ACCESSORY SELECTION SECTION 
void QmitkMITKPeripheralConfigView::OnGeneratedAccessorySelectionComboBoxOptions()
{
	//Clear Combo box list
	m_Controls->m_AccessorySelectionComboBox->clear();
	m_Controls->m_AccessorySelectionComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);

	std::map< std::string, mitk::MITKPeripheralAccessory::Pointer >::iterator itAccessory;
	QVariant deviceBool(false);
	QVariant accessoryBool(true);

	//Iterate though accessories on the container and add them to the list. Use a true boolean to show that the peripheral is an accessory.
	if(m_AccessoryContainer.size() > 0)
	{
		for(itAccessory = m_AccessoryContainer.begin(); itAccessory != m_AccessoryContainer.end(); itAccessory++)
		{
			m_Controls->m_AccessorySelectionComboBox->addItem(QString(itAccessory->first.c_str()), accessoryBool);
		}
	}
	
	//Add default option.
	m_Controls->m_AccessorySelectionComboBox->insertItem(0,"No Accessory Selected",deviceBool);
	m_Controls->m_AccessorySelectionComboBox->setCurrentIndex(0);

}
void QmitkMITKPeripheralConfigView::OnAccessoryDataCleared()
{
	//Clear global and GUI elements when a device or an accessory is deselected or changed; or no accessory is selected.
	m_Controls->m_AccessorySelectionComboBox->setCurrentIndex(0);
	m_Controls->m_ValueLineEdit->setEnabled(false);
	m_Controls->m_ValueLineEdit->clear();
	m_Controls->m_NewPropertyNameLineEdit->clear();
	m_Controls->m_NewAccessoryNameLineEdit->clear();
	m_Controls->m_NewPropertyTypeComboBox->setCurrentIndex(-1);
	m_Controls->m_ExistingPropertyGroupBox->setChecked(false);
	
	m_SelectedAccessory = NULL;

}
void QmitkMITKPeripheralConfigView::OnDeviceSelectedGenerateAccessoryGUI(bool on)
{
	if(on)
	{
		OnGeneratedAccessorySelectionComboBoxOptions();
	}
	//turn on Accessory GUI
	m_Controls->m_AccessoryFrame->setVisible(on);
	m_Controls->m_AccessoryFrame->setEnabled(on);
	m_Controls->m_AccessorySelectionSelectButton->setEnabled(false);
	m_Controls->m_AccessorySelectionSelectButton->setVisible(false);
	m_Controls->m_NewAccessoryNameLineEdit->setEnabled(false);
	
}

void QmitkMITKPeripheralConfigView::OnAccessorySelectionBoxToggled(int index)
{
	//Disable properties value line
	m_Controls->m_ValueLineEdit->setEnabled(false);
	m_Controls->m_ValueLineEdit->clear();
	//Clear data from name and combo box for new property
	m_Controls->m_NewPropertyGroupBox->setChecked(false);
	m_Controls->m_NewPropertyNameLineEdit->clear();
	m_Controls->m_NewPropertyTypeComboBox->setCurrentIndex(-1);
	//Clear data from existing property
	m_Controls->m_ExistingPropertyGroupBox->setChecked(false);
	m_Controls->m_ExistingPropertyComboBox->setCurrentIndex(-1);

	this->OnEnableCalibrationFileSection(false);

	if(index == 0)
	{
		//No Accessory Selected option chosen
		if(m_SelectedDevice.IsNotNull())
		{
			//Detaches all the accessories in the device.
			m_SelectedDevice->DetachAll();
		}
		//Reset accessory GUI and global variables
		m_Controls->m_NewAccessoryNameLineEdit->setText("");
		m_Controls->m_AccessorySelectionSelectButton->setEnabled(false);
		m_Controls->m_AccessorySelectionSelectButton->setVisible(false);
		m_SelectedAccessory = NULL;
		if(m_SelectedAccessory.IsNull() && m_SelectedDevice.IsNull())
		{
			m_DeviceIsSelected = false;
		}
		this->OnEnableCalibrationFileSection(true);
	}
	else
	{
		//Activate GUI elements
		m_Controls->m_NewAccessoryNameLineEdit->setText(m_Controls->m_AccessorySelectionComboBox->currentText());
		m_Controls->m_NewAccessoryNameLineEdit->setEnabled(false);
		m_Controls->m_AccessorySelectionSelectButton->setEnabled(true);
		m_Controls->m_AccessorySelectionSelectButton->setVisible(true);
		this->OnEnableCalibrationFileSection(true);
	}
}


void QmitkMITKPeripheralConfigView::OnSelectAccessoryButtonClicked()
{
	QString accName = m_Controls->m_AccessorySelectionComboBox->currentText();
	mitk::MITKPeripheralAccessory::Pointer itDevice;
	//Check if device exists in device object
	if(m_SelectedDevice.IsNull())
	{
		return;
	}

	//Find the accessory in the accessory map of the device and attach the accessory to the device.
	std::map< std::string,mitk::MITKPeripheralAccessory::Pointer > aMap =  m_SelectedDevice->GetAccessoryMap();
	if(aMap.find(accName.toStdString())!= aMap.end())
	{
		itDevice = aMap.find(accName.toStdString())->second;
		itDevice->Attach();
	}
	else
	{
		//If it is not on the accessory map of the device, check if accessory exists on global list, clone it and add it.
		if(m_AccessoryContainer.find(accName.toStdString()) != m_AccessoryContainer.end())
		{
			itDevice = (m_AccessoryContainer.find(accName.toStdString()))->second->Clone();
		}
		m_SelectedDevice->AddAccessory(itDevice);
	}
	//Select accessory
	if(itDevice.IsNotNull())
	{
		m_SelectedAccessory = m_SelectedDevice->GetAttachedAccessory();
		m_Controls->toolBox->setCurrentIndex(1);
	}

	m_Controls->m_AccessorySelectionSelectButton->setEnabled(false);
}

// PROPERTY ADDITION SECTION 
void QmitkMITKPeripheralConfigView::OnGeneratedPropertySelectionComboBoxOptions()
{
	//Clear properties combo box.
	m_Controls->m_ExistingPropertyComboBox->clear();
	m_Controls->m_ExistingPropertyComboBox->setInsertPolicy(QComboBox::InsertAlphabetically);

	std::map< std::string, mitk::MITKPeripheralProperty::Pointer >::iterator itProperty;

	//Default values
	QVariant boolType(false);
	QVariant floatType(1.1);
	QVariant stringType("empty");
	QVariant noneType(2);

	QVariant info = noneType;
	
	if(m_Properties.size() > 0)
	{
		//Iterate through the property list and add them to the combo box
		for(itProperty = m_Properties.begin(); itProperty != m_Properties.end(); itProperty++)
		{
			switch(itProperty->second->GetPropertyType())
			{
			case mitk::MITKPeripheralProperty::PBoolean:
				info = boolType;
				break;

			case mitk::MITKPeripheralProperty::PString:
				info = stringType;
				break;

			case mitk::MITKPeripheralProperty::PNumeric:
				info = floatType;
				break;

			default:
				info = noneType;
				break;
			
			}
			m_Controls->m_ExistingPropertyComboBox->addItem(QString(itProperty->first.c_str()), info);
		}
	}

	//Add default option
	m_Controls->m_ExistingPropertyComboBox->insertItem(0,"No Property Selected", noneType);
	m_Controls->m_ExistingPropertyComboBox->setCurrentIndex(0);
}
void QmitkMITKPeripheralConfigView::OnDeviceSelectedEnablePropertiesAndParameters(bool on)
{
	//Toggle EDIT PROPERTY VALUES
	m_Controls->m_EditPropertiesScrollArea->setVisible(on);
	m_Controls->m_EditPropertiesScrollArea->setEnabled(on);
	
	//Toggle ADD NEW PROPERTY TAB
	m_Controls->m_AddPropertyScrollArea->setVisible(on);
	m_Controls->m_AddPropertyScrollArea->setEnabled(on);

}
void QmitkMITKPeripheralConfigView::OnExistingListToggled(int index)
{
	//Reset GUI
	m_Controls->m_ValueLineEdit->clear();
	m_Controls->m_ValueLineEdit->setEnabled(false);
	this->OnEnableCalibrationFileSection(false);
		
	if(index > 0)
	{
		//Activate Value fields
		m_Controls->m_ValueLineEdit->setEnabled(true);
		//Toggles add button on
		m_Controls->m_AddPropertyAddButton->setEnabled(true);
		int type = -1;
		//Get the property type
		switch(m_Controls->m_ExistingPropertyComboBox->itemData(m_Controls->m_ExistingPropertyComboBox->currentIndex()).type())
		{
		case QVariant::Bool:
			type = 0;
			m_Controls->m_ValueLineEdit->setEnabled(false);
			break;
		case QVariant::String:
				m_Controls->m_ValueLineEdit->setEnabled(true);
				type = 1;
			break;
		case QVariant::Double:
				m_Controls->m_ValueLineEdit->setEnabled(true);
				type = 2;
			break;
		case QVariant::Int:
			type = -1;
			break;
		}
		//Set information in the GUI elements
		m_Controls->m_NewPropertyNameLineEdit->setText(m_Controls->m_ExistingPropertyComboBox->currentText());
		m_Controls->m_NewPropertyTypeComboBox->setCurrentIndex(type);
	}
}

void QmitkMITKPeripheralConfigView::OnCommandBoxPropertyTypeSelected()
{
	//Reset GUI and get index.
	int index = m_Controls->m_NewPropertyTypeComboBox->currentIndex();
	m_Controls->m_ValueLineEdit->clear();

	switch(index)
	{
	case -1:
		//None selected
		m_Controls->m_ValueLineEdit->setEnabled(false);
		break;

	case 0: 
		//Boolean
		m_Controls->m_ValueLineEdit->setEnabled(false);
		break;

	case 1:
		//text value
		m_Controls->m_ValueLineEdit->setEnabled(true);
		break;

	case 2: 
		// Float value
		m_Controls->m_ValueLineEdit->setEnabled(true);
		break;
	}

}

void QmitkMITKPeripheralConfigView::OnNewPropertyToggled(bool on)
{
	//Reset GUI
	m_Controls->m_ExistingPropertyComboBox->setCurrentIndex(-1);
	m_Controls->m_NewPropertyTypeComboBox->setCurrentIndex(-1);
	m_Controls->m_ValueLineEdit->clear();
	m_Controls->m_ValueLineEdit->setEnabled(false);
		

	if(on)
	{
		//Deselect add new property box
		m_Controls->m_ExistingPropertyGroupBox->setChecked(false);
		//Activate Value fields
		m_Controls->m_ValueLineEdit->setEnabled(true);
		//Toggles add button on
		m_Controls->m_AddPropertyAddButton->setEnabled(true);
		
	}
	else
	{
		m_Controls->m_ValueLineEdit->setEnabled(false);
		//Toggles add button off
		m_Controls->m_AddPropertyAddButton->setEnabled(false);

	}
}



void QmitkMITKPeripheralConfigView::OnAddExistingPropertyToggled(bool on)
{
	//Reset GUI and generate list
	this->OnGeneratedPropertySelectionComboBoxOptions();
	m_Controls->m_ExistingPropertyComboBox->setCurrentIndex(-1);
	m_Controls->m_NewPropertyTypeComboBox->setCurrentIndex(-1);
	m_Controls->m_ValueLineEdit->clear();
	m_Controls->m_NewPropertyNameLineEdit->clear();
	
	if(on)
	{
		//Deselect add new property box
		m_Controls->m_NewPropertyGroupBox->setChecked(false);
		m_Controls->m_NewPropertyNameLineEdit->setText("");
		OnGeneratedPropertySelectionComboBoxOptions();
	}
	else
	{
	  m_Controls->m_ValueLineEdit->setEnabled(false);
		//Toggles addbutton off
		m_Controls->m_AddPropertyAddButton->setEnabled(false);
	}

}

void QmitkMITKPeripheralConfigView::OnAddPropertyButtonClicked()
{
	
	//Get booleans from GUI
	bool isNewProperty = m_Controls->m_NewPropertyGroupBox->isChecked();
	bool isExistingProperty = m_Controls->m_ExistingPropertyGroupBox->isChecked();
	
	//Get text from line edits
	QString newPropertyName = m_Controls->m_NewPropertyNameLineEdit->text();
 	QString initialValueText = m_Controls->m_ValueLineEdit->text();
		
	//Get info from Comboboxes
	int newPropertyType = m_Controls->m_NewPropertyTypeComboBox->currentIndex();
	QString selectedPropertyName = m_Controls->m_ExistingPropertyComboBox->currentText();
	

	if(initialValueText.isEmpty() && newPropertyType != 0)
	{
		this->GenerateAlertBox("Please enter a value for the property!");
		return;
	}
	//Check if property already exists
	if(isNewProperty)
	{
		//Empty name and type
		if(newPropertyName.isEmpty() || newPropertyName.isNull() || newPropertyType == -1)
		{
			this->GenerateAlertBox("Please provide a name and type for the new property!");
			return;
		}

		if(this->PropertyExists(newPropertyName))
		{
			//Add message about the property already existing
			this->GenerateAlertBox("The Property already exists. Please change the name or use the provided existing option!");
			return;
		}
		else
		{
			//Create a new property			
			mitk::MITKPeripheralProperty::Pointer createdProperty = mitk::MITKPeripheralProperty::New();
			createdProperty->SetPropertyName(newPropertyName.toStdString());

			//Define its type with standards values.
			mitk::MITKPeripheralProperty::PropertyType pType;
			switch(newPropertyType)
			{
			case 0:
				pType = mitk::MITKPeripheralProperty::PBoolean;
				createdProperty->SetValue(false);
				break;
			case 1:
				pType = mitk::MITKPeripheralProperty::PString;
				createdProperty->SetValue(std::string("empty"));
				break;
			case 2:
				pType = mitk::MITKPeripheralProperty::PNumeric;
				createdProperty->SetValue(0.0f);
				break;
			default: 
				pType = mitk::MITKPeripheralProperty::PNone;
				break;
			}
			createdProperty->SetPropertyType(pType);

			//Add the property to the global list.
			m_Properties.insert(std::pair<std::string, mitk::MITKPeripheralProperty::Pointer>(createdProperty->GetPropertyName(),createdProperty));
			
			//Add property to the Accessory both the selected and in the global list.
			//Accessories attached on a device get priority on the property (i.e. the property is assigned to the accessory).
			if(m_SelectedAccessory.IsNotNull())
			{
				m_SelectedAccessory->AddProperty(newPropertyName.toStdString(), pType, initialValueText.toStdString(), false, initialValueText.toFloat());
				(m_AccessoryContainer.find(m_SelectedAccessory->GetAccessoryName())->second)->AddProperty(newPropertyName.toStdString(), pType, "empty", false, 0.0);
			}
			else
			{
				//Add the property to the device.
				if(m_SelectedDevice.IsNotNull())
				{
 					m_SelectedDevice->AddProperty(newPropertyName.toStdString(), pType, initialValueText.toStdString(), false, initialValueText.toFloat());
				}
								
			}
			
			//Re-generate GUI and lists
 			this->OnGeneratedPropertySelectionComboBoxOptions();
			this->GenerateAlertBox("The property was added successfully.");

		}
	}
	else if(isExistingProperty)
	{
		//No property selected alert
		if(m_Controls->m_ExistingPropertyComboBox->currentIndex() == -1)
		{
			GenerateAlertBox("Please select a property before continuing!");
			return;
		}

		//Check that the accessory does not already have the property added to it.
		if(m_SelectedAccessory.IsNotNull())
		{
			if(m_SelectedAccessory->HasProperty(selectedPropertyName.toStdString()))
			{
				this->GenerateAlertBox("The property already exists in this peripheral.\n Use the Edit Property tab to modify the property's value.");
				return;	
			}
		}
		else
		{
			//Check if the peripheral already has the property in its map.
			if(m_SelectedDevice.IsNotNull() && m_SelectedDevice->HasProperty(selectedPropertyName.toStdString()))
			{
				this->GenerateAlertBox("The property already exists in this peripheral.\n Use the Edit Property tab to modify the property's value.");
				return;
			}
		}
			
		//Add property to device
		mitk::MITKPeripheralProperty::PropertyType pType;
		switch(newPropertyType)
		{
		case 0:
			pType = mitk::MITKPeripheralProperty::PBoolean;
			break;
		case 1:
			pType = mitk::MITKPeripheralProperty::PString;
			break;
		case 2:
			pType = mitk::MITKPeripheralProperty::PNumeric;
			break;
		default: 
			pType = mitk::MITKPeripheralProperty::PNone;
			break;
		}

		//Add property to the Accessory both the selected and in the global list.
		//Accessories attached on a device get priority on the property (i.e. the property is assigned to the accessory).
		if(m_SelectedAccessory.IsNotNull())
		{
			m_SelectedAccessory->AddProperty(selectedPropertyName.toStdString(), pType, initialValueText.toStdString(), false, initialValueText.toFloat());
			(m_AccessoryContainer.find(m_SelectedAccessory->GetAccessoryName())->second)->AddProperty(selectedPropertyName.toStdString(), pType, "empty", false, 0.0);
		}
		else
		{
			//Add property to the device
			if(m_SelectedDevice.IsNotNull())
			{
				m_SelectedDevice->AddProperty(selectedPropertyName.toStdString(), pType, initialValueText.toStdString(), false, initialValueText.toFloat());
			}		
		}
		
		//Re-generate GUI and lists
		this->OnGeneratedPropertySelectionComboBoxOptions();
		this->GenerateAlertBox("The property was added successfully.");
	}

	//Reset GUI elements
	m_Controls->m_ValueLineEdit->clear();
	m_Controls->m_NewPropertyNameLineEdit->clear();
	m_Controls->m_NewPropertyTypeComboBox->setCurrentIndex(-1);
}

void QmitkMITKPeripheralConfigView::OnRemovePropertyButtonClicked()
{
	if(m_SelectedDevice.IsNotNull())
	{
		for(int row = 0; row < m_Controls->m_EditPropertiesTableWidget->rowCount(); row++)
		{
			//Loop through table and find the rows that are set on device table
			bool mybool = Qt::Checked == m_Controls->m_EditPropertiesTableWidget->item(row,2)->checkState();
			std::string propertyName = m_Controls->m_EditPropertiesTableWidget->item(row,0)->text().toStdString();
			//Erase property by name from device
			if(mybool && propertyName.compare("File Location")!=0)
			{
				m_SelectedDevice->RemoveProperty(propertyName);
				//Update list
				m_DeviceContainer.erase(m_SelectedDevice->GetDeviceName());
				m_DeviceContainer.insert(std::pair< std::string, mitk::MITKPeripheralInfo::Pointer>(m_SelectedDevice->GetDeviceName(),m_SelectedDevice));
			}
		}
	}

	if(m_SelectedAccessory.IsNotNull())
	{
		for(int row = 0; row < m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount(); row++)
		{
			//loop through table and find the rows that are set on accessory table
			bool mybool = Qt::Checked == m_Controls->m_EditAccessoryPropertiesTableWidget->item(row,2)->checkState();
			std::string propertyName = m_Controls->m_EditAccessoryPropertiesTableWidget->item(row,0)->text().toStdString();
			//erase by name from accessory
			if(mybool && propertyName.compare("File Location")!=0)
			{
				m_SelectedAccessory->RemoveProperty(propertyName);
				//Update accessory list if it is a stand-alone accessory, else update device list.
				if(m_SelectedDevice.IsNull())
				{
					m_AccessoryContainer.erase(m_SelectedAccessory->GetAccessoryName());
					m_AccessoryContainer.insert(std::pair< std::string, mitk::MITKPeripheralAccessory::Pointer>(m_SelectedAccessory->GetAccessoryName(),m_SelectedAccessory));
				}
				else
				{
					m_DeviceContainer.erase(m_SelectedDevice->GetDeviceName());
					m_DeviceContainer.insert(std::pair< std::string, mitk::MITKPeripheralInfo::Pointer>(m_SelectedDevice->GetDeviceName(),m_SelectedDevice));
				}
			}
		}
	}
	//Reset GUI
	this->OnPropertiesTableRefresh(1);
	this->OnAccessoryPropertiesTableRefresh(1);
}

//EDIT TABLE SECTION 
void QmitkMITKPeripheralConfigView::OnPropertiesTableRefresh(int index)
{
	//Disable Table GUI and reset table variables and titles.
	m_Controls->m_RemovePropertiesButton->setEnabled(false);
	m_Controls->m_RemovePropertiesButton->setVisible(false);

	m_Controls->m_EditPropertiesTableWidget->setCurrentCell(-1,-1);
	m_Controls->m_EditPropertiesTableWidget->clear();
	m_Controls->m_EditPropertiesTableWidget->setRowCount(0);
	QTableWidgetItem* propertyName = new QTableWidgetItem("Property");
	QTableWidgetItem* propertyValue = new QTableWidgetItem("Value");
	QTableWidgetItem* propertySelection = new QTableWidgetItem("Selected");

	m_Controls->m_EditPropertiesTableWidget->setHorizontalHeaderItem(0,propertyName);
	m_Controls->m_EditPropertiesTableWidget->setHorizontalHeaderItem(1,propertyValue);
	m_Controls->m_EditPropertiesTableWidget->setHorizontalHeaderItem(2,propertySelection);

	//Do not generate it unless it is on table tab
	if(index != 1)
	{
		return;
	}

	std::map< std::string, mitk::MITKPeripheralProperty::Pointer >::iterator itProperty;
	std::map< std::string, mitk::MITKPeripheralProperty::Pointer > propMap;
	int pColIndex = -1;

	if(m_SelectedDevice.IsNotNull())
	{
		//Set table title label
		QString aLabel = "Active Device: ";
		aLabel.append(m_SelectedDevice->GetDeviceName().c_str());
		m_Controls->m_ActiveDeviceLabel->setText(aLabel);

		propMap = m_SelectedDevice->GetPropertyMap();

		//Go through the property map in the device and generate the property cells
		for(itProperty = propMap.begin(); itProperty != propMap.end(); itProperty++)
		{
			m_Controls->m_EditPropertiesTableWidget->setRowCount(m_Controls->m_EditPropertiesTableWidget->rowCount()+1);
			
			if(itProperty->second->GetPropertyType() == mitk::MITKPeripheralProperty::PBoolean)
			{			
				//Property name
				QTableWidgetItem *tableItem = new QTableWidgetItem();
				tableItem->setText(QString(itProperty->first.c_str()));
				tableItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
				m_Controls->m_EditPropertiesTableWidget->setItem(m_Controls->m_EditPropertiesTableWidget->rowCount()-1,0,tableItem);
				
				//Value for the boolean property
				QTableWidgetItem *tableItemValue = new QTableWidgetItem(); 

				if(itProperty->second->GetValueAsBool())
				{
					tableItemValue->setCheckState(Qt::Checked);
				}
				else
				{
					tableItemValue->setCheckState(Qt::Unchecked);
				}
				tableItemValue->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsTristate);
				m_Controls->m_EditPropertiesTableWidget->setItem(m_Controls->m_EditPropertiesTableWidget->rowCount()-1,1,tableItemValue);
				
			}
			
			if(itProperty->second->GetPropertyType() == mitk::MITKPeripheralProperty::PString)
			{	
				//Property name
				QTableWidgetItem *tableItem = new QTableWidgetItem();
				tableItem->setText(QString(itProperty->first.c_str()));
				tableItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
				m_Controls->m_EditPropertiesTableWidget->setItem(m_Controls->m_EditPropertiesTableWidget->rowCount()-1,0,tableItem);
				
				//Value for the property
				QTableWidgetItem *tableItemValue = new QTableWidgetItem(); 
				try
				{
					tableItemValue->setText(QString(itProperty->second->GetValueAsString().c_str()));
					tableItemValue->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
				}
				catch(int i)
				{
					std::stringstream exception;
					exception<< i;
					this->GenerateAlertBox(QString(exception.str().c_str()));
				}
				m_Controls->m_EditPropertiesTableWidget->setItem(m_Controls->m_EditPropertiesTableWidget->rowCount()-1,1,tableItemValue);
			}
			
			if(itProperty->second->GetPropertyType() == mitk::MITKPeripheralProperty::PNumeric)
			{
				//property name
				QTableWidgetItem *tableItem = new QTableWidgetItem();
				tableItem->setText(QString(itProperty->first.c_str()));
				tableItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
				m_Controls->m_EditPropertiesTableWidget->setItem(m_Controls->m_EditPropertiesTableWidget->rowCount()-1,0,tableItem);
				
				//value for the property
				QTableWidgetItem *tableItemValue = new QTableWidgetItem(); 
				try
				{
					std::stringstream strstream;
					strstream<< itProperty->second->GetValueAsFloat();
					std::string str = strstream.str();
					tableItemValue->setText(QString(str.c_str()));
				}
				catch(int i)
				{
					std::stringstream exception;
					exception<< i;
					this->GenerateAlertBox(QString(exception.str().c_str()));
				}

				tableItemValue->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
				m_Controls->m_EditPropertiesTableWidget->setItem(m_Controls->m_EditPropertiesTableWidget->rowCount()-1,1,tableItemValue);
			}
			
			//Extra checkbox cell used with the remove proeprty button
			QTableWidgetItem *selectBox = new QTableWidgetItem(); 
			selectBox->setCheckState(Qt::Unchecked);

			//users are not allowed to remove this property
			if(itProperty->first.compare("File Location")==0)
			{
				selectBox->setFlags(Qt::ItemIsTristate);
			}
			else
			{
				selectBox->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsTristate);
			}
			m_Controls->m_EditPropertiesTableWidget->setItem(m_Controls->m_EditPropertiesTableWidget->rowCount()-1,2,selectBox);
		}
	}
	else
	{
		QString aLabel = "Active Device: None";
		m_Controls->m_ActiveDeviceLabel->setText(aLabel);
	}

	//Disable if no properties are shown
	if(m_Controls->m_EditPropertiesTableWidget->rowCount() < 1)
	{
		m_Controls->m_EditPropertiesTableWidget->setEnabled(false);
		m_Controls->m_EditPropertiesTableWidget->setVisible(false);
	}
	else
	{
		m_Controls->m_EditPropertiesTableWidget->setEnabled(true);
		m_Controls->m_EditPropertiesTableWidget->setVisible(true);
	}
	//Also refresh accessory table
	this->OnAccessoryPropertiesTableRefresh(index);

	if(m_Controls->m_EditPropertiesTableWidget->rowCount() > 0 || m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount() > 0 )
	{
		m_Controls->m_RemovePropertiesButton->setEnabled(true);
		m_Controls->m_RemovePropertiesButton->setVisible(true);
	}
}


void QmitkMITKPeripheralConfigView::OnAccessoryPropertiesTableRefresh(int index)
{
	//Reset gui variables and GUI options
	m_Controls->m_EditAccessoryPropertiesTableWidget->setCurrentCell(-1,-1);
	m_Controls->m_EditAccessoryPropertiesTableWidget->clear();
	m_Controls->m_EditAccessoryPropertiesTableWidget->setRowCount(0);
	QTableWidgetItem* propertyName = new QTableWidgetItem("Property");
	QTableWidgetItem* propertyValue = new QTableWidgetItem("Value");
	QTableWidgetItem* propertySelection = new QTableWidgetItem("Selected");

	m_Controls->m_EditAccessoryPropertiesTableWidget->setHorizontalHeaderItem(0,propertyName);
	m_Controls->m_EditAccessoryPropertiesTableWidget->setHorizontalHeaderItem(1,propertyValue);
	m_Controls->m_EditAccessoryPropertiesTableWidget->setHorizontalHeaderItem(2,propertySelection);
	
	if(index != 1)
	{
		return;
	}
	std::map< std::string, mitk::MITKPeripheralProperty::Pointer >::iterator itProperty;
	std::map< std::string, mitk::MITKPeripheralProperty::Pointer > propMap;
	
	int pColIndex = -1;
	if(m_SelectedAccessory.IsNotNull())
	{
		//Active accessory title label
		QString aLabel = "Active Accessory: ";		
		aLabel.append(m_SelectedAccessory->GetAccessoryName().c_str());
		m_Controls->m_ActiveAccessoryLabel->setText(aLabel);
		
		propMap = m_SelectedAccessory->GetPropertyMap();

		for(itProperty = propMap.begin(); itProperty != propMap.end(); itProperty++)
		{
			m_Controls->m_EditAccessoryPropertiesTableWidget->setRowCount(m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount()+1);
			
			if(itProperty->second->GetPropertyType() == mitk::MITKPeripheralProperty::PBoolean)
			{			
				//Property name
				QTableWidgetItem *tableItem = new QTableWidgetItem();
				tableItem->setText(QString(itProperty->first.c_str()));
				tableItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
				m_Controls->m_EditAccessoryPropertiesTableWidget->setItem(m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount()-1,0,tableItem);
				
				//Value for the boolean property
				QTableWidgetItem *tableItemValue = new QTableWidgetItem(); 
				if(itProperty->second->GetValueAsBool())
				{
					tableItemValue->setCheckState(Qt::Checked);
				}
				else
				{
					tableItemValue->setCheckState(Qt::Unchecked);
				}
				tableItemValue->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsTristate);
				m_Controls->m_EditAccessoryPropertiesTableWidget->setItem(m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount()-1,1,tableItemValue);
				
			}

			if(itProperty->second->GetPropertyType() == mitk::MITKPeripheralProperty::PString )
			{	
				//Property name
				QTableWidgetItem *tableItem = new QTableWidgetItem();
				tableItem->setText(QString(itProperty->first.c_str()));
				tableItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
				m_Controls->m_EditAccessoryPropertiesTableWidget->setItem(m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount()-1,0,tableItem);
				
				//Value for the property
				QTableWidgetItem *tableItemValue = new QTableWidgetItem(); 
				try
				{
					tableItemValue->setText(QString(itProperty->second->GetValueAsString().c_str()));
				}
				catch(int i)
				{
					std::stringstream exception;
					exception<< i;
					this->GenerateAlertBox(QString(exception.str().c_str()));
				}

				tableItemValue->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
				m_Controls->m_EditAccessoryPropertiesTableWidget->setItem(m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount()-1,1,tableItemValue);
			}

			if(itProperty->second->GetPropertyType() == mitk::MITKPeripheralProperty::PNumeric)
			{
				//Property name
				QTableWidgetItem *tableItem = new QTableWidgetItem();
				tableItem->setText(QString(itProperty->first.c_str()));
				tableItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
				m_Controls->m_EditAccessoryPropertiesTableWidget->setItem(m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount()-1,0,tableItem);
				
				//Value for the property
				QTableWidgetItem *tableItemValue = new QTableWidgetItem(); 
				try
				{
					std::stringstream strstream;
					strstream<< itProperty->second->GetValueAsFloat();
					std::string str = strstream.str();
					tableItemValue->setText(QString(str.c_str()));
				}
				catch(int i)
				{
					std::stringstream exception;
					exception<< i;
					this->GenerateAlertBox(QString(exception.str().c_str()));
				}

				tableItemValue->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled);
				m_Controls->m_EditAccessoryPropertiesTableWidget->setItem(m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount()-1,1,tableItemValue);

			}

			QTableWidgetItem *selectBox = new QTableWidgetItem(); 
			selectBox->setCheckState(Qt::Unchecked);

			//Users are not allowed to remove this property
			if(itProperty->first.compare("File Location")==0)
			{
				selectBox->setFlags(Qt::ItemIsTristate);
			}
			else
			{
				selectBox->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsTristate);
			}
			m_Controls->m_EditAccessoryPropertiesTableWidget->setItem(m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount()-1,2,selectBox);

		}
	}
	else
	{
		QString aLabel = "Active Accessory: None";
		m_Controls->m_ActiveAccessoryLabel->setText(aLabel);
	}

	//Disable if no properties exist
	if(m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount() < 1)
	{
		m_Controls->m_EditAccessoryPropertiesTableWidget->setEnabled(false);
		m_Controls->m_EditAccessoryPropertiesTableWidget->setVisible(false);
	}
	else
	{
		m_Controls->m_EditAccessoryPropertiesTableWidget->setEnabled(true);
		m_Controls->m_EditAccessoryPropertiesTableWidget->setVisible(true);
	}
}
void QmitkMITKPeripheralConfigView::OnTableChanged(int newRow, int newColumn, int row, int column)
{
	//Border checking
	if(row >= m_Controls->m_EditPropertiesTableWidget->rowCount() || row < 0 ||
			column >= m_Controls->m_EditPropertiesTableWidget->columnCount() || column <= 0)
	{
		return;
	}
	QString propertyName = m_Controls->m_EditPropertiesTableWidget->item(row,0)->text();
	QTableWidgetItem* value = m_Controls->m_EditPropertiesTableWidget->item(row,column);

	//Find property by name
	if(m_SelectedDevice.IsNotNull() && m_SelectedDevice->HasProperty(propertyName.toStdString()))
	{
		mitk::MITKPeripheralProperty::PropertyType pType = m_SelectedDevice->GetPropertyMap().find(propertyName.toStdString())->second->GetPropertyType();
		
		//Update value according to type
		if(pType == mitk::MITKPeripheralProperty::PBoolean)
		{
			bool mybool = Qt::Checked == m_Controls->m_EditPropertiesTableWidget->item(row,column)->checkState();
			m_SelectedDevice->SetPropertyValue(propertyName.toStdString(), mybool);
		}
		if(pType == mitk::MITKPeripheralProperty::PString)
		{
			m_SelectedDevice->SetPropertyValue(propertyName.toStdString(), value->data(0).toString().toStdString());
		}
		if(pType == mitk::MITKPeripheralProperty::PNumeric)
		{
			m_SelectedDevice->SetPropertyValue(propertyName.toStdString(), value->data(0).toFloat());
		}
	}
	


}
void QmitkMITKPeripheralConfigView::OnAccessoryTableChanged(int newRow, int newColumn, int row, int column)
{
	if(row >= m_Controls->m_EditAccessoryPropertiesTableWidget->rowCount() || row < 0 ||
			column >= m_Controls->m_EditAccessoryPropertiesTableWidget->columnCount() || column <= 0)
	{
		return;
	}
	QString propertyName = m_Controls->m_EditAccessoryPropertiesTableWidget->item(row,0)->text();
	QTableWidgetItem* value = m_Controls->m_EditAccessoryPropertiesTableWidget->item(row,column);

	
	//Find property type by name and assign the corresponding new value
	if(m_SelectedAccessory.IsNotNull() && m_SelectedAccessory->HasProperty(propertyName.toStdString()))
	{
		mitk::MITKPeripheralProperty::PropertyType pType = m_SelectedAccessory->GetPropertyMap().find(propertyName.toStdString())->second->GetPropertyType();
		
		if(pType == mitk::MITKPeripheralProperty::PBoolean)
		{
			bool mybool = Qt::Checked == m_Controls->m_EditAccessoryPropertiesTableWidget->item(row,column)->checkState();
			m_SelectedAccessory->SetPropertyValue(propertyName.toStdString(), mybool);
		}
		if(pType == mitk::MITKPeripheralProperty::PString)
		{

			m_SelectedAccessory->SetPropertyValue(propertyName.toStdString(), value->data(0).toString().toStdString());
		}
		if(pType == mitk::MITKPeripheralProperty::PNumeric)
		{
			m_SelectedAccessory->SetPropertyValue(propertyName.toStdString(), value->data(0).toFloat());
		}
	}
	
}
// OTHER 
void QmitkMITKPeripheralConfigView::OnGenerateConfigFileButtonClicked()
{
	if(!m_DeviceIsSelected)
	{
		this->GenerateAlertBox("Please Select a device first.");
		return;
	}
	//Prompt the user to choose a path where to save the file.
	QString path = QFileDialog::getSaveFileName(m_MultiWidget, "Open Config File", MITK_ROOT , tr("Configuration File (*.tofconfig)"), 0);
	if(!path.isEmpty())
	{
		if(m_FileName.compare(path) != 0)
		{
			std::remove(m_FileName.toStdString().c_str());
		}
		m_FileName = path;
		m_Controls->m_NewDeviceConfigFilePathLineEdit->setText(m_FileName);
	}
	
	//Change file location property
	if(m_SelectedDevice.IsNotNull())
	{
		m_SelectedDevice->RemoveProperty("File Location");
		m_SelectedDevice->AddProperty("File Location", mitk::MITKPeripheralProperty::PString, m_Controls->m_NewDeviceConfigFilePathLineEdit->text().toStdString(), false, 0.0);
	}
	else
	{
		if(m_SelectedAccessory.IsNotNull())
		{
			m_SelectedAccessory->RemoveProperty("File Location");
			m_SelectedAccessory->AddProperty("File Location", mitk::MITKPeripheralProperty::PString, m_Controls->m_NewDeviceConfigFilePathLineEdit->text().toStdString(), false, 0.0);	
		}
	}

	m_Controls->m_EditPropertiesTableWidget->setCurrentCell(-1,-1);
	m_Controls->m_EditAccessoryPropertiesTableWidget->setCurrentCell(-1,-1);
	m_Writer = mitk::MITKPeripheralConfigFileWriter::New();
	m_Writer->SetFileName(m_FileName.toStdString().c_str());
	if(m_DeviceIsSelected)
	{
		m_Writer->GetInput(m_SelectedDevice, m_SelectedAccessory, m_Properties,m_DeviceContainer,m_AccessoryContainer);//m_Parameters);
		int result = m_Writer->WriteFile();
		int resultp = 0;
		int resulta = 0;
		int resultd = 0;

		//Write Global lists
		m_Writer->SetFileName(m_XMLPropertiesFileName.c_str());
		resultp = m_Writer->SavePropertyList(m_Properties);
		m_Writer->SetFileName(m_XMLAccessoriesFileName.c_str());
		resulta = m_Writer->SaveAccessoryList(m_AccessoryContainer);
		m_Writer->SetFileName(m_XMLDevicesFileName.c_str());
		resultd = m_Writer->SaveDeviceList(m_DeviceContainer);

		//Error displaying
		if(result == 0)
		{
			this->GenerateAlertBox("Config file successfully saved.");
		}
		else
		{
			this->GenerateAlertBox("There was an error while writing the file.\n The file might have been deleted or moved.");
		}
		if(resultp != 0)
		{
			this->GenerateAlertBox("There was an error while writing the Properties' file.\n The file might have been deleted or moved.");
		}
		if(resulta != 0)
		{
			this->GenerateAlertBox("There was an error while writing the Accessories' file.\n The file might have been deleted or moved.");
		}
		if(resultd != 0)
		{
			this->GenerateAlertBox("There was an error while writing the Devices' file.\n The file might have been deleted or moved.");
		}
	}
	
	this->OnLoadingInformation();
	this->OnPropertiesTableRefresh(1);
	if(m_SelectedDevice.IsNotNull())
	{
		m_Controls->m_DeviceSelectionComboBox->setCurrentIndex(m_Controls->m_DeviceSelectionComboBox->findText(QString(m_SelectedDevice->GetDeviceName().c_str())));
	}
	if(m_SelectedAccessory.IsNotNull())
	{
		m_Controls->m_ExistingPropertyComboBox->setCurrentIndex(m_Controls->m_ExistingPropertyComboBox->findText(QString(m_SelectedAccessory->GetAccessoryName().c_str())));
	}
}

int QmitkMITKPeripheralConfigView::GenerateAlertBox(QString message)
{
	//Generate box with passed text
	QMessageBox msgBox;
	msgBox.setText("\n " + message + "\t");
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Close);
	msgBox.setDefaultButton(QMessageBox::Close);
	//return clicked option
	int ret = msgBox.exec();
	return ret;
}

bool QmitkMITKPeripheralConfigView::PropertyExists(QString name)
{	
	return m_Properties.find(name.toStdString()) != m_Properties.end();
}

// CALIBRATION FILE SECTION
void QmitkMITKPeripheralConfigView::OnCalibrationFilePathButtonClicked()
{
	QString caliFileName = QFileDialog::getOpenFileName(m_MultiWidget, "Open Calibration File", MITK_ROOT , tr("Calibration File (*.xml)"), 0);
	m_Controls->m_CalibrationFileLineEdit->setText(caliFileName);
}

void QmitkMITKPeripheralConfigView::OnEnableCalibrationFileSection(bool on)
{
	//Enable and disable GUI elements
	m_Controls->m_BrowseCalibrationFilePathButton->setEnabled(on);
	m_Controls->m_CalibrationFileLineEdit->setText("");
	m_Controls->m_CalibrationFileSelectButton->setEnabled(on);
	m_Controls->m_CalibrationFileFrame->setEnabled(on);
	m_Controls->m_CalibrationFileFrame->setVisible(on);
}

void QmitkMITKPeripheralConfigView::OnCalibrationFileSelectButtonClicked()
{
	std::map< std::string, mitk::MITKPeripheralProperty::Pointer > caliList;
	QString name = m_Controls->m_CalibrationFileLineEdit->text();
	m_Reader->SetFileName(name.toStdString().c_str());
	//Load calibration file
	caliList = m_Reader->LoadCalibrationFile();
	
	if(caliList.size() < 1)
	{
		return;
	}
	//Add the new list to the devices/accessory list of properties
	if(m_SelectedAccessory.IsNotNull())
	{
		m_SelectedAccessory->AddMoreProperties(caliList);
	}
	else
	{
		if(m_SelectedDevice.IsNotNull())
		{
			m_SelectedDevice->AddMoreProperties(caliList);
		}
	}

	//Disable GUI elements
	m_Controls->m_CalibrationFileSelectButton->setEnabled(false);
	m_Controls->m_BrowseCalibrationFilePathButton->setEnabled(false);
	m_Controls->toolBox->setCurrentIndex(1);
}
//
void QmitkMITKPeripheralConfigView::StdMultiWidgetAvailable (QmitkStdMultiWidget &stdMultiWidget)
{
  m_MultiWidget = &stdMultiWidget;
}


void QmitkMITKPeripheralConfigView::StdMultiWidgetNotAvailable()
{
  m_MultiWidget = NULL;
}


void QmitkMITKPeripheralConfigView::OnSelectionChanged( std::vector<mitk::DataNode*> nodes )
{ 
  // iterate all selected objects, adjust warning visibility
  for( std::vector<mitk::DataNode*>::iterator it = nodes.begin();
       it != nodes.end();
       ++it )
  {
    mitk::DataNode::Pointer node = *it;
  
    if( node.IsNotNull() && dynamic_cast<mitk::Image*>(node->GetData()) )
    {
      m_Controls->lblWarning->setVisible( false );
      return;
    }
  }

  m_Controls->lblWarning->setVisible( true );
}


