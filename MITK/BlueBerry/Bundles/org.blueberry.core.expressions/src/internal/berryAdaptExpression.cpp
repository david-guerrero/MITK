/*=========================================================================

Program:   BlueBerry Platform
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

#include "berryAdaptExpression.h"

#include <berryPlatform.h>
#include <berryRuntime.h>
#include <berryIAdapterManager.h>
#include <berryObject.h>

#include "berryExpressions.h"
#include "berryDefaultVariable.h"

#include <Poco/Hash.h>
#include <Poco/Exception.h>
#include <Poco/Bugcheck.h>

namespace berry {

const std::string AdaptExpression::ATT_TYPE= "type"; //$NON-NLS-1$

/**
 * The seed for the hash code for all adapt expressions.
 */
const std::size_t AdaptExpression::HASH_INITIAL = Poco::Hash<std::string>()("berry::AdaptExpression");

AdaptExpression::AdaptExpression(IConfigurationElement::Pointer configElement)
{
  bool attr = configElement->GetAttribute(ATT_TYPE, fTypeName);
  Expressions::CheckAttribute(ATT_TYPE, attr);
}

AdaptExpression::AdaptExpression(Poco::XML::Node* /*element*/)
{
  throw Poco::NotImplementedException();
  //fTypeName = element->GetAttribute(ATT_TYPE);
  //Expressions::CheckAttribute(ATT_TYPE, fTypeName.length() > 0 ? fTypeName : null);
}

AdaptExpression::AdaptExpression(const std::string& typeName)
{
  poco_assert(typeName.size() != 0);
  fTypeName= typeName;
}

//bool
//AdaptExpression::equals(final Object object)
//{
//  if (!(object instanceof AdaptExpression))
//    return FALSE_EVAL;
//
//  final AdaptExpression that= (AdaptExpression)object;
//  return this.fTypeName.equals(that.fTypeName)
//      && equals(this.fExpressions, that.fExpressions);
//}

std::size_t
AdaptExpression::ComputeHashCode()
{
  throw Poco::NotImplementedException("ComputeHashCode not implemented");
  //return HASH_INITIAL * HASH_FACTOR + HashCode(fExpressions)
  //  * HASH_FACTOR + fTypeName.hashCode();
}

/* (non-Javadoc)
 * @see Expression#evaluate(IVariablePool)
 */
EvaluationResult
AdaptExpression::Evaluate(IEvaluationContext* context)
{
  if (fTypeName.size() == 0)
    return EvaluationResult::FALSE_EVAL;
  Object::Pointer var(context->GetDefaultVariable());
  Object::Pointer adapted;
  IAdapterManager::Pointer manager = Platform::GetServiceRegistry().GetServiceById<IAdapterManager>(Runtime::ADAPTER_SERVICE_ID);
  if (Expressions::IsInstanceOf(var, fTypeName)) {
    adapted= var;
  } else {
    if (!manager->HasAdapter(var->GetClassName(), fTypeName))
      return EvaluationResult::FALSE_EVAL;

    Poco::Any anyAdapted(manager->GetAdapter(var.GetPointer(), fTypeName));
    if (!anyAdapted.empty() && anyAdapted.type() == typeid(Object::Pointer))
    {
      adapted = Poco::AnyCast<Object::Pointer>(anyAdapted);
    }
  }
  // the adapted result is null but hasAdapter returned TRUE_EVAL check
  // if the adapter is loaded.
  if (adapted.IsNull()) {
    if (manager->QueryAdapter(var->GetClassName(), fTypeName) == IAdapterManager::NOT_LOADED) {
      return EvaluationResult::NOT_LOADED;
    } else {
      return EvaluationResult::FALSE_EVAL;
    }
  }
  return this->EvaluateAnd(new DefaultVariable(context, adapted));
}

void
AdaptExpression::CollectExpressionInfo(ExpressionInfo* info)
{
  // Although the default variable is passed to the children of this
  // expression as an instance of the adapted type it is OK to only
  // mark a default variable access.
  info->MarkDefaultVariableAccessed();
  CompositeExpression::CollectExpressionInfo(info);
}

}  // namespace berry
