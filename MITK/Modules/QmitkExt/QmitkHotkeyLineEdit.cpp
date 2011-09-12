/*=========================================================================

 Program:   Medical Imaging & Interaction Toolkit
 Language:  C++
 Date:      $Date: 2009-07-07 16:57:15 +0200 (Di, 07 Jul 2009) $
 Version:   $Revision: 18019 $

 Copyright (c) German Cancer Research Center, Division of Medical and
 Biological Informatics. All rights reserved.
 See MITKCopyright.txt or http://www.mitk.org/copyright.html for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#include "QmitkHotkeyLineEdit.h"

#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QKeyEvent>

const std::string QmitkHotkeyLineEdit::TOOLTIP = "Press any key (combination)";

QmitkHotkeyLineEdit::QmitkHotkeyLineEdit( QWidget* parent )
: QLineEdit(parent)
{
  this->Init();
  //this->setReadOnly(true);
}

QmitkHotkeyLineEdit::QmitkHotkeyLineEdit( const QKeySequence& _QKeySequence, QWidget* parent)
: QLineEdit(parent)
{
  this->Init();
  //this->setReadOnly(true);
  this->SetKeySequence(_QKeySequence);
}

QmitkHotkeyLineEdit::QmitkHotkeyLineEdit( const QString& _QKeySequenceAsString, QWidget* parent)
: QLineEdit(parent)
{
  this->Init();
  //this->setReadOnly(true);
  this->SetKeySequence(_QKeySequenceAsString);
}

void QmitkHotkeyLineEdit::Init()
{
  this->setToolTip(QString::fromStdString(QmitkHotkeyLineEdit::TOOLTIP));
  this->setReadOnly(true);
  connect( this, SIGNAL( textChanged(const QString &) ), this,
           SLOT( LineEditTextChanged(const QString &) ) );
}

void QmitkHotkeyLineEdit::keyPressEvent( QKeyEvent * event )
{
  if(event->key() == Qt::Key_unknown)
    return;
  else if(event->key() == Qt::Key_Escape)
    m_KeySequence = QKeySequence();

  else
  {
    m_KeySequence = QKeySequence(event->modifiers(), event->key());
    // if no modifier was pressed the sequence is now empty
    if(event->modifiers() == Qt::NoModifier)
      m_KeySequence = QKeySequence(event->key());
  }

  this->SetKeySequence(m_KeySequence);
}

void QmitkHotkeyLineEdit::SetKeySequence( const QKeySequence& _QKeySequence)
{
  this->setText(_QKeySequence.toString());
}

void QmitkHotkeyLineEdit::SetKeySequence( const QString& _QKeySequenceAsString )
{
  this->SetKeySequence(QKeySequence(_QKeySequenceAsString));
}

QKeySequence QmitkHotkeyLineEdit::GetKeySequence()
{
  return m_KeySequence;
}

QString QmitkHotkeyLineEdit::GetKeySequenceAsString()
{
  return m_KeySequence.toString();
}

bool QmitkHotkeyLineEdit::Matches( QKeyEvent * event )
{
  QKeySequence _KeySequence = QKeySequence(event->modifiers(), event->key());
  // if no modifier was pressed the sequence is now empty
  if(event->modifiers() == Qt::NoModifier)
    _KeySequence = QKeySequence(event->key());

  return _KeySequence == m_KeySequence;
}

void QmitkHotkeyLineEdit::LineEditTextChanged(const QString & text)
{
  m_KeySequence = QKeySequence(text.toUpper());
}
