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

#ifndef QmitkMITKPeripheralConfigView_h
#define QmitkMITKPeripheralConfigView_h

#include <berryISelectionListener.h>

#include <QmitkFunctionality.h>

#include "ui_QmitkMITKPeripheralConfigViewControls.h"

#include "mitkPeripheralConfigFileWriter.h"




/*!
 * \class QmitkMITKPeripheralConfigView
 * \headerfile QmitkMITKPeripheralConfigView.h "QmitkMITKPeripheralConfigView.h"
 * \brief This the GUI for the Bundle Pripheral Configuration File Generator.
 * \sa QmitkFunctionality
 * \ingroup Functionalities
 */
class QmitkMITKPeripheralConfigView : public QmitkFunctionality
{  
  // this is needed for all Qt objects that should have a Qt meta-object
  // (everything that derives from QObject and wants to have signal/slots)
  Q_OBJECT
  
  public:  

    QmitkMITKPeripheralConfigView();

		//CTK Bundle Constructor
		QmitkMITKPeripheralConfigView(const QmitkMITKPeripheralConfigView& other)
    {
      Q_UNUSED(other)
      throw std::runtime_error("Copy constructor not implemented");
    }
   
		virtual ~QmitkMITKPeripheralConfigView();

		static const std::string VIEW_ID;			///< Holds the bundle ID.

    virtual void CreateQtPartControl(QWidget *parent);

    virtual void StdMultiWidgetAvailable (QmitkStdMultiWidget &stdMultiWidget);
    
		virtual void StdMultiWidgetNotAvailable();
		
		/*!
     * \brief Looks for a property called 'name' in the properties' global list and returns whether it exists or not. 
		 * \param name Name used to query the property list.
		 * \return Boolean indicating whether the property exists or not.
     */
		bool PropertyExists(QString name);
		
		/*!
     * \brief Generates an alert box with an OK button and a Cancel Button, prompting the user.
		 * \param message string used as the main message in the alert box.
		 * \return Integer indicating which button of the alert box was clicked.
     */
		int GenerateAlertBox(QString message);


  protected slots:

		/*!
     * \brief SLOT: Deselects current accessory and disables accessory GUI elements.
     */
		void OnAccessoryDataCleared();
  
		/*!
     * \brief SLOT: Initializes the GUI and generates QtComboBox contents.
     */
		void OnInitializingGUI();

		/*!
     * \brief SLOT: Loads Global lists from files.
     */
		void OnLoadingInformation();

		/*!
     * \brief SLOT: Generates the Device Selection tab's QtComboBox contents.
     */
		void OnGeneratedDeviceSelectionComboBoxOptions();
		
		/*!
     * \brief SLOT: Generates the Accessory Selection's QtComboBox contents.
     */
		void OnGeneratedAccessorySelectionComboBoxOptions();

		/*!
     * \brief SLOT: Generates the Existing Property's QtComboBox contents.
     */
		void OnGeneratedPropertySelectionComboBoxOptions();
   
		/*!
     * \brief SLOT: Generates the QFileDialog window to choose a path.
     */
		void OnButtonClickedCreateFileDialog();

		/*!
     * \brief SLOT: Enables or disables the GUI parts corresponding to the Existing Property section.
		 * \param on Boolean that will enable if it is true or disable if it is false.
     */
		void OnAddExistingPropertyToggled(bool on);

		/*!
     * \brief SLOT: Enables or disables the GUI parts corresponding to the New Property section.
		 * \param on Boolean that will enable if it is true or disable if it is false.
     */
		void OnNewPropertyToggled(bool on);

		/*!
     * \brief SLOT: Enables or disables the GUI parts depending on the type of property to be added (i.e. Initial Value line edit).
		 */
		void OnCommandBoxPropertyTypeSelected();

		/*!
     * \brief SLOT: Enables or disables the GUI parts depending on the type of the existing property.
		 */
		void OnExistingListToggled(int index);

		/*!
     * \brief SLOT: Activates all tabs of the GUI and generates the table with the properties loaded from the device's config file.
		 * File existance is checked, multiple possible errors are checked for and the respective message alert box is generated to prompt the user for correct behaviour.
		 */
		void OnSelectDeviceButtonClicked();

		/*!
     * \brief SLOT: Selects the newly created device or accessory with the passed name and adds it to the global list of devices or accessories. Creates an empty config file. 
		 */
		void OnAddNewDeviceButtonClicked();

		/*!
     * \brief SLOT: Enables or disables GUI elements of tabs, different to the device selection tab, when a device or accessory is selected or de-selected.
		 * \param on Boolean indicating whether the tabs of the toolbox should be enabled or not.
		 */
		void OnDeviceSelectedEnablePropertiesAndParameters(bool on);

		/*!
     * \brief SLOT: Enables or disables the GUI elements corresponding to one of 3 different options: No device selected, Add new device, or any existing device to be selected.
		 * \param index Integer passed by the device selection ComboBox indicating whether a device or accessory was chosen, or not; or if a new device is to be added. 
		 * It disables the tabs and tables if the selection is changed.
		 */
		void OnDeviceSelectionBoxToggled(int index);

		/*!
     * \brief SLOT: Enables or disables GUI elements corresponding to the accessory selection.
		 * \param on Boolean indicating whether the GUI elements should be enabled or not.
		 */
		void OnDeviceSelectedGenerateAccessoryGUI(bool on);
		
		/*!
     * \brief SLOT: Enables or disables the GUI elements corresponding to one of 2 different options: No accessory selected, or any existing accessory to be selected.
		 * \param index Integer passed by the accessory selection ComboBox indicating whether an accessory was chosen, or not. 
		 * It disables the tabs and tables if the selection is changed.
		 */
		void OnAccessorySelectionBoxToggled(int index);

		/*!
     * \brief SLOT: Selects the accessory and adds it to the selected device, generates the accessory table and enables the tabs.
		 * Makes the accessory be the recipient of new properties added.
		 */
		void OnSelectAccessoryButtonClicked();

		/*!
     * \brief SLOT: Adds a property as defined in the GUI parameters passed to the selected device or selected accessory. Also adds it to the global list.
		 * If a property with the same name exists or if some GUI parameters have been left blank corresponding messages are displayed in alert boxes.
		 */
	  void OnAddPropertyButtonClicked();
		
		/*!
     * \brief SLOT: Saves the property value that was changed (corresponds to the property on row 'row') and updates the table.
		 * \param row Integer indicating index of row of item that was modified.
		 * \param column Integer indicating index of column of item that was modified.
		 * Called when the current cell changes.
		 */
		void OnTableChanged(int newRow, int newColumn, int row, int column);

		/*!
     * \brief SLOT: Saves the property value that was changed (corresponds to the property on row 'row') and updates the table.
		 * \param row Integer indicating index of row of item that was modified.
		 * \param column Integer indicating index of column of item that was modified.
		 * Called when the current cell changes.
		 */
		void OnAccessoryTableChanged(int newRow, int newColumn, int row, int column);

		/*!
     * \brief SLOT: Updates the device table with the current properties and values on the selected device.
		 * \param index Integer passed by the toolbox as it current index.
		 * If the integer does not indicate the 'Edit Table' tab, the function does nothing.
		 */
		void OnPropertiesTableRefresh(int index);

		/*!
     * \brief SLOT: Updates the accessory table with the current properties and values on the selected accessory.
		 * \param index Integer passed by the toolbox as it current index.
		 * If the integer does not indicate the 'Edit Table' tab, the function does nothing.
		 */
		void OnAccessoryPropertiesTableRefresh(int index);
		
		/*!
     * \brief SLOT: Prompts the user to choose a path and triggers the xml file serialization.
		 */
		void OnGenerateConfigFileButtonClicked();

		/*!
     * \brief SLOT: Removes the device from the global list.
		 * The file corresponding to the device is deleted if found. The GUI comboboxes are regenerated and updated. If it is an accessory, it is removed from all devices that have had it added to them.
		 * The user is prompted for confirmation.
		 */
		void OnRemoveDeviceButtonClicked();

		/*!
     * \brief SLOT: Removes the selected properties from the device, accessory or de accessory attached to the device.
		 */
		void OnRemovePropertyButtonClicked();

		/*!
     * \brief SLOT: Opens a file dialog window allowing the choosing of a .xml file to import for properties.
		 */
		void OnCalibrationFilePathButtonClicked();

		/*!
     * \brief SLOT: Enables or disables GUI elements from the calibration file section frame.
		 */
		void OnEnableCalibrationFileSection(bool on);

		/*!
     * \brief SLOT: Starts the deserialization of the Calibration File.
		 */
		void OnCalibrationFileSelectButtonClicked();



  protected:

    /// \brief called by QmitkFunctionality when DataManager's selection has changed
    virtual void OnSelectionChanged( std::vector<mitk::DataNode*> nodes );

    //GUI
    QmitkStdMultiWidget* m_MultiWidget;
		Ui::QmitkMITKPeripheralConfigViewControls* m_Controls;
		
    //Lists that holds all the Properties, Devices and Accessories ever created
		std::map< std::string, mitk::MITKPeripheralProperty::Pointer > m_Properties;					///< Holds the global list of properties.
		std::map< std::string, mitk::MITKPeripheralInfo::Pointer > m_DeviceContainer;					///< Holds the global list of devices.
		std::map< std::string, mitk::MITKPeripheralAccessory::Pointer> m_AccessoryContainer;	///< Holds the global list of accessories.
		
		//Current Information for working device
		mitk::MITKPeripheralInfo::Pointer m_SelectedDevice;						///< Holds the current or selected device.
		mitk::MITKPeripheralAccessory::Pointer m_SelectedAccessory;		///< Holds the current or selected accessory.

		bool m_DeviceIsSelected;		///< Holds the boolean indicating whether a device or accessory has been selected.
		QString m_FileName;					///< Holds the name of the file corresponding to the selected device or accessory.					
		QString m_DefaultPath;			///< Holds the default path where files are saved. 

		//READER and WRITER
		mitk::MITKPeripheralConfigFileReader::Pointer m_Reader; 	///< Holds the Reader object that Deserializes files into the data structure tree.
		mitk::MITKPeripheralConfigFileWriter::Pointer m_Writer;		///< Holds the Writer object that Serializes the datas tree into files.
		
		std::string m_XMLPropertiesFileName;		///< Holds the name of the global properties' xml file.
		std::string m_XMLAccessoriesFileName;		///< Holds the name of the global accessories' xml file.
		std::string m_XMLDevicesFileName;				///< Holds the name of the global devices' xml file.	

};



#endif // _QMITKMITKPERIPHERALCONFIGVIEW_H_INCLUDED

