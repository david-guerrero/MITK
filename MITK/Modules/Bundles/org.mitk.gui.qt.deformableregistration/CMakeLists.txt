PROJECT(org_mitk_gui_qt_deformableregistration)

MACRO_CREATE_MITK_CTK_PLUGIN(
  EXPORT_DIRECTIVE DEFORMABLEREGISTRATION_EXPORT
  EXPORTED_INCLUDE_SUFFIXES src
  MODULE_DEPENDENCIES QmitkExt MitkDeformableRegistration MitkDeformableRegistrationUI)
