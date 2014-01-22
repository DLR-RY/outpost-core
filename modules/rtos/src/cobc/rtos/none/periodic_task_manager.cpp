/*
 * Copyright (c) 2013, German Aerospace Center (DLR)
 * All Rights Reserved.
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#include "periodic_task_manager.h"

using namespace cobc::rtos;

PeriodicTaskManager::PeriodicTaskManager()
{
}

PeriodicTaskManager::~PeriodicTaskManager()
{
}

PeriodicTaskManager::Status::Type
PeriodicTaskManager::nextPeriod(time::Duration /*period*/)
{
	return Status::idle;
}

PeriodicTaskManager::Status::Type
PeriodicTaskManager::status()
{
	return Status::idle;
}

void
PeriodicTaskManager::cancel()
{
}
