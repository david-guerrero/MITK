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


#ifndef _MITK_SURFACE_VTK_WRITER__H_
#define _MITK_SURFACE_VTK_WRITER__H_

#include <iomanip>

#include <itkProcessObject.h>
#include <mitkFileWriter.h>
#include <mitkPointSet.h>
#include <mitkSurface.h>

#include <vtkSmartPointer.h>

class vtkTransformPolyDataFilter;

namespace mitk
{

/**
 * @brief VTK-based writer for mitk::Surface 
 *
 * The mitk::Surface is written using the VTK-writer-type provided as the
 * template argument. If the mitk::Surface contains multiple points of
 * time, multiple files are written. The life-span (time-bounds) of each
 * each point of time is included in the filename according to the 
 * following scheme: 
 * &lt;filename&gt;_S&lt;timebounds[0]&gt;E&lt;timebounds[1]&gt;_T&lt;framenumber&gt;
 * (S=start, E=end, T=time).
 * Writing of multiple files according to a given filename pattern is not
 * yet supported.
 * @ingroup Process
*/
template <class VTKWRITER>
class SurfaceVtkWriter : public mitk::FileWriter
{
public:

    mitkClassMacro( SurfaceVtkWriter, mitk::FileWriter );

    itkNewMacro( Self );

    mitkWriterMacro;

    typedef VTKWRITER VtkWriterType;

    /**
     * Sets the filename of the file to write.
     * @param _arg the name of the file to write.
     */
    itkSetStringMacro( FileName );

    /**
     * @returns the name of the file to be written to disk.
     */
    itkGetStringMacro( FileName );

    /**
     * \brief Explicitly set the extension to be added to the filename.
     * @param _arg to be added to the filename, including a "." 
     * (e.g., ".vtk").
     * 
     * Partial template specialization is used for some vtk-writer types 
     * to set a default extension.
     */
    itkSetStringMacro( Extension );

    /**
     * \brief Get the extension to be added to the filename.
     * @returns the extension to be added to the filename (e.g., 
     * ".vtk").
     */
    itkGetStringMacro( Extension );

    /**
     * \brief Set the extension to be added to the filename to the default
     * 
     * Partial template specialization is used for some vtk-writer types 
     * to define the default extension.
     */
    void SetDefaultExtension();

    /**
     * @warning multiple write not (yet) supported
     */
    itkSetStringMacro( FilePrefix );

    /**
     * @warning multiple write not (yet) supported
     */
    itkGetStringMacro( FilePrefix );

    /**
     * @warning multiple write not (yet) supported
     */
    itkSetStringMacro( FilePattern );

    /**
     * @warning multiple write not (yet) supported
     */
    itkGetStringMacro( FilePattern );

    /**
     * Sets the 0'th input object for the filter.
     * @param input the first input for the filter.
     */
    void SetInput( mitk::Surface* input );

    /**
     * @returns the 0'th input object of the filter.
     */
    const mitk::Surface* GetInput();

    /**
    * @brief Return the extension to be added to the filename.
    */
    virtual std::string GetFileExtension();

    /**
    * @brief Check if the Writer can write the Content of the DataTreenode.
    */
    virtual bool CanWriteDataType( DataNode* );

    /**
    * @brief Return the MimeType of the saved File.
    */
    virtual std::string GetWritenMIMEType();

    /**
    * @brief Set the DataTreenode as Input. Important: The Writer always have a SetInput-Function.
    */
    virtual void SetInput( DataNode* );

    VtkWriterType* GetVtkWriter()
    {
      return m_VtkWriter;
    }

    /**
    * @brief Return the possible file extensions for the data type associated with the writer
    */
    virtual std::vector<std::string> GetPossibleFileExtensions();

protected:

    /**
     * Constructor.
     */
    SurfaceVtkWriter();

    /**
     * Virtual destructor.
     */
    virtual ~SurfaceVtkWriter();

    virtual void GenerateData();
      
    void ExecuteWrite( VtkWriterType* vtkWriter );

    std::string m_FileName;

    std::string m_FilePrefix;

    std::string m_FilePattern;

    std::string m_Extension;

    std::string m_MimeType;

    vtkSmartPointer<VtkWriterType> m_VtkWriter;
  
    bool m_WriterWriteHasReturnValue;
};

}

#endif //_MITK_SURFACE_VTK_WRITER__H_
