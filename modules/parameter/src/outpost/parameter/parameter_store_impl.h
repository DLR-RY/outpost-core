/*
 * parameter_stor_impl.h
 *
 *  Created on: 14.09.2020
 *      Author: user
 */

#ifndef OUTPOST_PARAMETER_PARAMETER_STOR_IMPL_H_
#define OUTPOST_PARAMETER_PARAMETER_STOR_IMPL_H_

#include "parameter_store.h"

namespace outpost
{
namespace parameter
{
template <typename T>
OperationResult
ParameterStore::getParameter(IDType id, const Parameter<T>*& store) const
{
    const ParameterBase* par = nullptr;
    auto ret = getParameter(id, par);

    // the initialized Test is in getParameter as well
    // checking par explicitly helps cppcheck
    if (ret != OperationResult::success || nullptr == par)
    {
        return ret;
    }

    if (par->getType() == Type::getType<T>())
    {
        store = static_cast<const Parameter<T>*>(par);
        return OperationResult::success;
    }
    else
    {
        return OperationResult::incorrectType;
    }
}

template <typename T>
OperationResult
ParameterStore::getValue(IDType id, T& store, outpost::time::SpacecraftElapsedTime* setTime) const
{
    const ParameterBase* par = nullptr;
    auto ret = getParameter(id, par);

    // the initialized Test is in getParameter as well
    // checking par explicitly helps cppcheck
    if (ret != OperationResult::success || nullptr == par)
    {
        return ret;
    }

    if (par->getType() == Type::getType<T>())
    {
        const Parameter<T>* tmp = static_cast<const Parameter<T>*>(par);
        return tmp->getValue(store, setTime);
    }
    else
    {
        return OperationResult::incorrectType;
    }
}

}  // namespace parameter
}  // namespace outpost

#endif
