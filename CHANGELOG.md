# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a
Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to
[Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- CI builds for Mac, Linux and Windows via Azure Pipelines

- HCC stage in Docker file.

- GitHub action to automatically delete merged branches.

- Enabled `FixedPool` allocator benchmarks.

- Mixed pool that uses faster fixed pools for smaller allocation sizes,
and a dynamic pool for those that are larger.

- Smoke tests for required third-party libraries.

- `util::FixedMallocPool` for internal use.

- Cookbook for enabling Umpire logging.

### Changed

- Replay uses JSON format for its I/O.

- OpenMP is off by default.

- Switched template parameters to runtime constructor arguments in `FixedPool`.

- Update `README.md` to better describe Umpire capability.

- Update BLT to fix CMake 3.13 warnings and MSVC compatibility.

- `util::AllocationMap` is significantly faster.

- `ResourceManager` de/registration pass `AllocationRecord` by value.

- `AllocationRecord` struct members are no longer prefixed by `m_`.

- `DynamicPool` directly incorporates Simpool's `DynamicSizePool` and
  uses `FixedMallocPool` internally for a small speedup.

### Removed

- `ENABLE_ASSERTS` option removed. `UMPIRE_ASSERT` should still be used.

- Merge the remaining classes in Simpool into the core of Umpire.

- Deprecated and unused `replay_allocation_map` tool.

### Fixed

- YAML file for ReadTheDocs to read in that will cause it to use
  Python 3.7 so that it quits producing build failures when it receives
  a deprecation warning when attempting to run older versions of python.

- Exclude third-party libraries from Doxygen to fix out-of-resources error on
  ReadTheDocs.

- Throw an error if attempting to deallocate with a different Allocator than
  performed the allocation.

- Build on Windows.

- Fixed compilation errors from Intel compiler for newly included third-party
  libraries for json and command line parsing (cxxopts).

- Update calls to allocate_pointer in the FORTRAN bindings to ensure that the
  correct variable type of C_SIZE_T is passed in.  This fixes compile errors in
  IBM XL.

## [0.3.3] - 2019-04-11

### Added

- NUMA strategy (umpire::strategy::NumaPolicy) that allows allocating memory
an specific NUMA nodes.

- Implemented << for Allocator, so that it can be printed directly.

- Update getAllocator methods to print list of available Allocators if the
requested Allocator cannot be found.

- Replay now captures coalesce operations from strategy::DynamicPool so that
these can be replayed.

- The replay tool can produce an output file that can be used to verify the
replayed events are correct.

- Cookbook example for creating a pool in pinned memory using FORTRAN.

- GitHub workflow to check for CHANGELOG updates.

- Ability to print allocation records that only match a predicate,
`print_allocator_records()` to get all records from a specific
allocator, and a cookbook recipe to do that.

- Dockerfile for multi-stage builds. Supports building Umpire with GCC, Clang,
and CUDA.

- GitHub action to run Clang static analysis.

- Replay now includes unique replay ID of the logging process to help
distinguish processes in an multi-process run.

- Umpire replay now takes a "--help" option and displays usage information.

- A const iterator for AllocationMap, a free function to pull out a vector of
allocation records for a specific allocator, and a method to calculate the
relative fragmentation.

### Changed

- Umpire now builds as a single library, libumpire.a or libumpire.so, rather
than having one library per source subdirectory.

- Removed shared_ptr usage entirely. Ownership of objects was never "shared"
and the smart pointers added unecessary overhead.

- Moved CHANGELOG to CHANGELOG.md.

### Removed

- The 'coalesce' method was removed from ResourceManager and now must be
accessed directory. examples/cookbook/recipe_coalesce_pool.cpp shows how to do
this.

### Fixed

- Bug in ResourceManager::copy/memset when given a pointer offset into an
allocation.

- Memory leak in judyL2Array.

- While replay already was recording release operations, the tool was not
actually replaying them.  A fix was implemented so that the replay tool will
now also replay any captured release operations.

- `make docs` used to fail, because the build was setup for Read the Docs. A fix
was implemented so Doxygen and Sphinx can be run locally, for instance to test
out new cookbooks.

- REPLAY previously recorded some operations with multiple print statements
causing REPLAY output from MPI runs to become mixed between multiple ranks.
REPLAY has been modified to output each operation onto a single line.
