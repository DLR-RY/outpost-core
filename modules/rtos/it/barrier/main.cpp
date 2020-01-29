/*
 * Copyright (c) 2020, German Aerospace Center (DLR)
 *
 * This file is part of the development version of OUTPOST.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Authors:
 * - 2020, Jan Malburg (DLR RY-AVS)
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <array>
#include <outpost/rtos.h>

namespace testing
{
#include "../../arch/freertos/outpost/rtos/barrier.cpp"
}

class TesterThread : public outpost::rtos::Thread
{
public:
	TesterThread():outpost::rtos::Thread(100),mNumber(0){};

	testing::outpost::rtos::Barrier* mBarrier;
	uint32_t mNumber;

	void
	run()
	{
		mBarrier->wait();
		mNumber++;
		mBarrier->wait();
		mNumber++;
		mBarrier->wait();
		mNumber++;
		mBarrier->wait();
		mNumber++;
		mBarrier->wait();
		mNumber++;
		mBarrier->wait();
		while(true)
		{
			outpost::rtos::Thread::sleep(outpost::time::Seconds(1));
		}
	}
};

int
main(void)
{
	constexpr uint32_t threadcount = 100;
	std::array<TesterThread, threadcount>threads;

	// yourself also needed
	testing::outpost::rtos::Barrier barrier(threadcount+1);

	for(auto it=threads.begin();it!=threads.end();++it)
	{
		it->mBarrier = &barrier;
		it->start();
	}

	// give all time to start
	outpost::rtos::Thread::sleep(outpost::time::Seconds(1));

	for(auto it=threads.begin();it!=threads.end();++it)
	{
		if(it->mNumber != 0)
		{
			printf("Thread skipped Barrier value = %d\n",it->mNumber);
		}
	}
	barrier.wait();

	// give all time to count
	outpost::rtos::Thread::sleep(outpost::time::Seconds(1));

	printf("First wait done\n");

	for(auto it=threads.begin();it!=threads.end();++it)
	{
		if(it->mNumber != 1)
		{
			printf("Thread skipped Barrier value = %d\n",it->mNumber);
		}
	}
	barrier.wait();

	// give all time to count
	outpost::rtos::Thread::sleep(outpost::time::Seconds(1));

	printf("Second wait done\n");

	for(auto it=threads.begin();it!=threads.end();++it)
	{
		if(it->mNumber != 2)
		{
			printf("Thread skipped Barrier value = %d\n",it->mNumber);
		}
	}
	barrier.wait();

	// give all time to count
	outpost::rtos::Thread::sleep(outpost::time::Seconds(1));

	printf("Third wait done\n");

	for(auto it=threads.begin();it!=threads.end();++it)
	{
		if(it->mNumber != 3)
		{
			printf("Thread skipped Barrier value = %d\n",it->mNumber);
		}
	}
	barrier.wait();

	// give all time to count
	outpost::rtos::Thread::sleep(outpost::time::Seconds(1));

	printf("Forth wait done\n");

	for(auto it=threads.begin();it!=threads.end();++it)
	{
		if(it->mNumber != 4)
		{
			printf("Thread skipped Barrier value = %d\n",it->mNumber);
		}
	}
	barrier.wait();

	// give all time to count
	outpost::rtos::Thread::sleep(outpost::time::Seconds(1));

	printf("Fifth wait done\n");

	for(auto it=threads.begin();it!=threads.end();++it)
	{
		if(it->mNumber != 5)
		{
			printf("Thread skipped Barrier value = %d\n",it->mNumber);
		}
	}
	barrier.wait();

	// give all time to count
	outpost::rtos::Thread::sleep(outpost::time::Seconds(1));

	printf("Done\n");

	// outpost Threads cannot end
	exit(0);

	// prevents warning
	return 0;
}
