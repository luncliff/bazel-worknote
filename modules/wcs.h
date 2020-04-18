/**
 * @file    wcs.h
 * @author  github.com/luncliff (luncliff@gmail.com)
 */
#pragma once
#include <cstdint>
#include <cstdio>

/**
 * @brief Primitive for operations in WCS
 *
 * @note  Its instance is lockable
 */
class wcs_context_t final {
  void *impl_1 = nullptr;
  void *impl_2 = nullptr;
  void *impl_3 = nullptr;
  void *impl_4 = nullptr;

public:
  /**
   * @throw std::runtime_error
   */
  wcs_context_t() noexcept(false);
  ~wcs_context_t() noexcept;
  wcs_context_t(const wcs_context_t &) = delete;
  wcs_context_t &operator=(const wcs_context_t &) = delete;
  wcs_context_t(wcs_context_t &&) = delete;
  wcs_context_t &operator=(wcs_context_t &&) = delete;

public:
  void lock() noexcept(false);
  void unlock() noexcept(false);
  bool try_lock() noexcept(false);
};

uint32_t wcs_insecure_get(wcs_context_t &ctx, //
                          const char *url, FILE *fout) noexcept;
