/*=========================================================================

Program:   Medical Imaging & Interaction Toolkit
Language:  C++
Date:      $Date: 2009-02-10 18:08:54 +0100 (Di, 10 Feb 2009) $
Version:   $Revision: 16228 $

Copyright (c) German Cancer Research Center, Division of Medical and
Biological Informatics. All rights reserved.
See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/


#ifndef MITKNavigationDataSequentialPlayer_H_HEADER_INCLUDED_
#define MITKNavigationDataSequentialPlayer_H_HEADER_INCLUDED_

#include <mitkNavigationDataPlayerBase.h>
#include "tinyxml.h"


namespace mitk
{

  /**Documentation
  * \brief This class is a slightly changed reimplementation of the
  * NavigationDataPlayer which does not care about timestamps and just
  * outputs the navigationdatas in their sequential order
  *
  * \ingroup IGT
  */
  class MitkIGT_EXPORT NavigationDataSequentialPlayer
    : public NavigationDataPlayerBase
  {
  public:
    mitkClassMacro(NavigationDataSequentialPlayer, NavigationDataPlayerBase);
    itkNewMacro(Self);

    /**
    * \brief sets the file name and path (if XMLString is set, this is neglected)
    */
    void SetFileName(const std::string& _FileName);

    /**
    * \brief returns the file name and path
    */
    itkGetStringMacro(FileName);

    /**
    * \brief sets a xml string (by this, the xml string is not read from file)
    */
    void SetXMLString(const std::string& _XMLString);

    /**
    * \brief returns the current xml string
    */
    itkGetStringMacro(XMLString);

    ///
    /// set to true if the data player should repeat the outputs
    ///
    itkSetMacro(Repeat, bool);
    ///
    /// set if the data player should repeat the outputs
    ///
    itkGetMacro(Repeat, bool);
    ///
    /// \return the number of navigation data snapshots available in the file
    ///
    itkGetMacro(NumberOfSnapshots, unsigned int);

    ///
    /// advance the output to the i-th snapshot
    /// e.g. if you want to have the NavData of snapshot
    /// 17 then you can call GoToSnapshot(17). index begins at 1!
    /// you can then also go back to snapshot 1 with GoToSnapshot(1)
    ///
    void GoToSnapshot(int i);

    /**
    * \brief Used for pipeline update just to tell the pipeline
    * that we always have to update
    */
    virtual void UpdateOutputInformation();

  protected:
    NavigationDataSequentialPlayer();
    virtual ~NavigationDataSequentialPlayer();
    void ReinitXML();
    mitk::NavigationData::Pointer ReadVersion1();
    ///
    /// do the work here
    ///
    virtual void GenerateData();

    std::string m_FileName;
    std::string m_XMLString;
    TiXmlDocument* m_Doc;
    TiXmlElement* m_DataElem;
    TiXmlElement* m_CurrentElem;
    bool m_Repeat;
    unsigned int m_NumberOfSnapshots;
    int m_LastGoTo;
  };
} // namespace mitk

#endif /* MITKNavigationDataSequentialPlayer_H_HEADER_INCLUDED_ */
