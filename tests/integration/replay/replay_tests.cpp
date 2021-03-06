//////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2018-2019, Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory
//
// Created by David Beckingsale, david@llnl.gov
// LLNL-CODE-747640
//
// All rights reserved.
//
// This file is part of Umpire.
//
// For details, see https://github.com/LLNL/Umpire
// Please also see the LICENSE file for MIT license.
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string>

#include "umpire/Allocator.hpp"
#include "umpire/ResourceManager.hpp"
#include "umpire/op/MemoryOperation.hpp"
#include "umpire/strategy/AllocationStrategy.hpp"
#include "umpire/strategy/AllocationAdvisor.hpp"
#include "umpire/strategy/DynamicPool.hpp"
#include "umpire/strategy/FixedPool.hpp"
#include "umpire/strategy/MixedPool.hpp"
#include "umpire/strategy/MonotonicAllocationStrategy.hpp"
#include "umpire/strategy/SlotPool.hpp"
#include "umpire/strategy/ThreadSafeAllocator.hpp"

class replayTest {
public:
  replayTest() : testAllocations(3), allocationSize(16)
  {
    auto& rm = umpire::ResourceManager::getInstance();

    rm.makeAllocator<umpire::strategy::MixedPool>(
          "host_mixedpool_default"
        , rm.getAllocator("HOST")
        //, 512             // smallest fixed block size (Bytes)
        //, 1*1024          // largest fixed block size 1KiB
        //, 4 * 1024 * 1024 // max fixed pool size
        //, 12.0            // size multiplier
        //, (256 * 1024 * 1204) // dynamic pool min initial allocation size
        //, (1 * 1024 * 1024)   // dynamic pool min allocation size
        //, heuristic75         // coallesce heuristic
    );
    allocatorNames.push_back("host_mixedpool_default");

    rm.makeAllocator<umpire::strategy::MixedPool>(
          "host_mixedpool_spec1"
        , rm.getAllocator("HOST")
        , 512             // smallest fixed block size (Bytes)
        //, 1*1024          // largest fixed block size 1KiB
        //, 4 * 1024 * 1024 // max fixed pool size
        //, 12.0            // size multiplier
        //, (256 * 1024 * 1204) // dynamic pool min initial allocation size
        //, (1 * 1024 * 1024)   // dynamic pool min allocation size
        //, heuristic75         // coallesce heuristic
    );
    allocatorNames.push_back("host_mixedpool_spec1");

    rm.makeAllocator<umpire::strategy::MixedPool>(
          "host_mixedpool_spec2"
        , rm.getAllocator("HOST")
        , 512             // smallest fixed block size (Bytes)
        , 1*1024          // largest fixed block size 1KiB
        //, 4 * 1024 * 1024 // max fixed pool size
        //, 12.0            // size multiplier
        //, (256 * 1024 * 1204) // dynamic pool min initial allocation size
        //, (1 * 1024 * 1024)   // dynamic pool min allocation size
        //, heuristic75         // coallesce heuristic
    );
    allocatorNames.push_back("host_mixedpool_spec2");

    rm.makeAllocator<umpire::strategy::MixedPool>(
          "host_mixedpool_spec3"
        , rm.getAllocator("HOST")
        , 512             // smallest fixed block size (Bytes)
        , 1*1024          // largest fixed block size 1KiB
        , 4 * 1024 * 1024 // max fixed pool size
        //, 12.0            // size multiplier
        //, (256 * 1024 * 1204) // dynamic pool min initial allocation size
        //, (1 * 1024 * 1024)   // dynamic pool min allocation size
        //, heuristic75         // coallesce heuristic
    );
    allocatorNames.push_back("host_mixedpool_spec3");

    rm.makeAllocator<umpire::strategy::MixedPool>(
          "host_mixedpool_spec4"
        , rm.getAllocator("HOST")
        , 512             // smallest fixed block size (Bytes)
        , 1*1024          // largest fixed block size 1KiB
        , 4 * 1024 * 1024 // max fixed pool size
        , 12.0            // size multiplier
        //, 256 * 1024 * 1024 // dynamic pool min initial allocation size
        //, 1 * 1024 * 1024   // dynamic pool min allocation size
        //, heuristic75         // coallesce heuristic
    );
    allocatorNames.push_back("host_mixedpool_spec4");

    rm.makeAllocator<umpire::strategy::MixedPool>(
          "host_mixedpool_spec5"
        , rm.getAllocator("HOST")
        , 512             // smallest fixed block size (Bytes)
        , 1*1024          // largest fixed block size 1KiB
        , 4 * 1024 * 1024 // max fixed pool size
        , 12.0            // size multiplier
        , 256 * 1024 * 1024 // dynamic pool min initial allocation size
        //, 1 * 1024 * 1024   // dynamic pool min allocation size
        //, heuristic75         // coallesce heuristic
    );
    allocatorNames.push_back("host_mixedpool_spec5");

    rm.makeAllocator<umpire::strategy::MixedPool>(
          "host_mixedpool_spec6"
        , rm.getAllocator("HOST")
        , 512             // smallest fixed block size (Bytes)
        , 1*1024          // largest fixed block size 1KiB
        , 4 * 1024 * 1024 // max fixed pool size
        , 12.0            // size multiplier
        , 256 * 1024 * 1024 // dynamic pool min initial allocation size
        , 1 * 1024 * 1024   // dynamic pool min allocation size
        //, heuristic75         // coallesce heuristic
    );
    allocatorNames.push_back("host_mixedpool_spec6");

    rm.makeAllocator<umpire::strategy::DynamicPool>(
        "host_simpool_defaults", rm.getAllocator("HOST"));
    allocatorNames.push_back("host_simpool_defaults");

    rm.makeAllocator<umpire::strategy::DynamicPool>(
        "host_simpool_spec1", rm.getAllocator("HOST"), 9876, 1234);
    allocatorNames.push_back("host_simpool_spec1");

    rm.makeAllocator<umpire::strategy::DynamicPool, false>(
        "host_simpool_spec2", rm.getAllocator("HOST"), 9876, 1234);
    allocatorNames.push_back("host_simpool_spec2");

    rm.makeAllocator<umpire::strategy::MonotonicAllocationStrategy>(
      "MONOTONIC 1024", 1024, rm.getAllocator("HOST"));
    allocatorNames.push_back("MONOTONIC 1024");

    rm.makeAllocator<umpire::strategy::SlotPool>(
      "host_slot_pool", 64, rm.getAllocator("HOST"));
    allocatorNames.push_back("host_slot_pool");

    rm.makeAllocator<umpire::strategy::ThreadSafeAllocator>(
      "thread_safe_allocator", rm.getAllocator("HOST"));
    allocatorNames.push_back("thread_safe_allocator");

#if 0
    //
    // Replay currently cannot support replaying FixedPool allocations.
    // This is because replay does its work at runtime and the FixedPool
    // is a template where sizes are generated at compile time.
    //
    struct data { char _[1024*1024]; };

    rm.makeAllocator<umpire::strategy::FixedPool<data>>(
        "fixed_pool_allocator", rm.getAllocator("HOST"));
    allocatorNames.push_back("fixed_pool_allocator");
#endif
  }

  ~replayTest( void )
  {
  }

  void runTest()
  {
    auto& rm = umpire::ResourceManager::getInstance();
    auto pooled_allocator = rm.getAllocator("host_simpool_spec1");
    auto strategy = pooled_allocator.getAllocationStrategy();
    auto tracker = dynamic_cast<umpire::strategy::AllocationTracker*>(strategy);

    if (tracker) {
      strategy = tracker->getAllocationStrategy();
    }

    auto dynamic_pool = dynamic_cast<umpire::strategy::DynamicPool*>(strategy);

    if (! dynamic_pool ) {
      std::cerr << "host_simpool_spec1 is not a dynamic pool!\n";
      exit(1);
    }

    for ( int i = 0; i < testAllocations; ++i ) {
      for ( auto n : allocatorNames ) {
        auto alloc = rm.getAllocator(n);
        allocations.push_back( std::make_pair(alloc.allocate( ++allocationSize ), n) );
      }
    }

    dynamic_pool->coalesce();

    for ( auto ptr : allocations ) {
      auto alloc = rm.getAllocator(ptr.second);
      alloc.deallocate( ptr.first );
    }

    dynamic_pool->coalesce();
    pooled_allocator.release();
  }
private:
  const int testAllocations;
  std::size_t allocationSize;
  std::vector<std::string> allocatorNames;
  std::vector<std::pair<void*, std::string>> allocations;
};


int main(int , char** )
{
  replayTest test;

  test.runTest();

  return 0;
}
