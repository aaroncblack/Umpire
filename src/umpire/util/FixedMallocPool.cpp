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

// FixedMallocPool follows the algorithm presented in
// Fast Efficient Fixed-Size Memory Pool -- Ben Kenwright

#include "umpire/util/FixedMallocPool.hpp"

#include "umpire/util/Macros.hpp"

#include <cstring>
#include <cstdlib>
#include <algorithm>

namespace umpire {
namespace util {

inline unsigned char*
FixedMallocPool::addr_from_index(const FixedMallocPool::Pool& p, unsigned int i) const
{
  return p.data + i * m_obj_bytes;
}

inline unsigned int
FixedMallocPool::index_from_addr(const FixedMallocPool::Pool& p, const unsigned char* ptr) const
{
  return static_cast<unsigned int>(ptr - p.data) / m_obj_bytes;
}

FixedMallocPool::Pool::Pool(const size_t object_bytes,
                            const size_t objects_per_pool) :
  data(static_cast<unsigned char*>(std::malloc(object_bytes * objects_per_pool))),
  next(data),
  num_initialized(0),
  num_free(objects_per_pool) {}

FixedMallocPool::FixedMallocPool(const size_t object_bytes,
                                 const size_t objects_per_pool) :
  m_obj_bytes(object_bytes),
  m_obj_per_pool(objects_per_pool),
  m_data_bytes(m_obj_bytes * m_obj_per_pool),
  m_pool()
{
  newPool();
}

FixedMallocPool::~FixedMallocPool()
{
  for (auto& a : m_pool) std::free(a.data);
}

void
FixedMallocPool::newPool()
{
  m_pool.emplace_back(m_obj_bytes, m_obj_per_pool);
}

void*
FixedMallocPool::allocInPool(Pool& p) noexcept
{
  if (p.num_initialized < m_obj_per_pool) {
    unsigned int* ptr = reinterpret_cast<unsigned int*>(addr_from_index(p, p.num_initialized));
    *ptr = p.num_initialized + 1;
    p.num_initialized++;
  }

  void* ret = nullptr;
  if (p.num_free > 0) {
    ret = static_cast<void*>(p.next);
    --p.num_free;
    p.next = (p.num_free != 0) ? addr_from_index(p, *reinterpret_cast<unsigned int*>(p.next)) : nullptr;
  }

  return ret;
}

void*
FixedMallocPool::allocate(size_t bytes)
{
  void* ptr = nullptr;

  for (auto it = m_pool.rbegin(); it != m_pool.rend(); ++it) {
    ptr = allocInPool(*it);
    if (ptr) return ptr;
  }

  if (!ptr) {
    newPool();
    ptr = allocate(bytes);
  }

  // Could be an error, but FixedMallocPool is used internally and an
  // error would be unrecoverable
  UMPIRE_ASSERT(ptr);

  return ptr;
}

void
FixedMallocPool::deallocate(void* ptr)
{
  for (auto& p : m_pool) {
    const unsigned char* t_ptr = reinterpret_cast<unsigned char*>(ptr);
    const ptrdiff_t offset = t_ptr - p.data;
    if ((offset >= 0) && (offset < static_cast<ptrdiff_t>(m_data_bytes))) {
      if (p.next != nullptr) {
        *reinterpret_cast<unsigned int*>(ptr) = index_from_addr(p, p.next);
        p.next = reinterpret_cast<unsigned char*>(ptr);
      }
      else {
        *reinterpret_cast<unsigned int*>(ptr) = m_obj_per_pool;
        p.next = reinterpret_cast<unsigned char*>(ptr);
      }
      ++p.num_free;
      return;
    }
  }

  UMPIRE_ERROR("Could not find the pointer to deallocate");
}

size_t
FixedMallocPool::numPools() const noexcept
{
  return m_pool.size();
}

} // end of namespace strategy
} // end of namespace umpire
