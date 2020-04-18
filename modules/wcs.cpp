#include <mutex>
#include <stdexcept>

#include "wcs.h"

#include <curl/curl.h>

using namespace std;

mutex wcs_mutex_impl{};

void wcs_context_t::lock() noexcept(false) { return wcs_mutex_impl.lock(); }
void wcs_context_t::unlock() noexcept(false) { return wcs_mutex_impl.unlock(); }
bool wcs_context_t::try_lock() noexcept(false) {
  return wcs_mutex_impl.try_lock();
}

once_flag wcs_init_flag{};

wcs_context_t::wcs_context_t() noexcept(false) {
  // with `std::call_once`, ensure `curl_global_init` is invoked only once!
  call_once(wcs_init_flag, []() {
    // defer curl_global_cleanup();
    auto ec = curl_global_init(CURL_GLOBAL_SSL | CURL_GLOBAL_WIN32);
    if (ec != CURLE_OK)
      throw runtime_error{curl_easy_strerror(ec)};
  });
}
wcs_context_t::~wcs_context_t() {
  // do nothing
}

uint32_t wcs_insecure_get(wcs_context_t &, //
                          const char *url, FILE *fout) noexcept {
  CURL *ctx = curl_easy_init();
  if (ctx == nullptr)
    return UINT32_MAX;
  curl_easy_setopt(ctx, CURLOPT_URL, url);
  curl_easy_setopt(ctx, CURLOPT_USERAGENT, curl_version());
  curl_easy_setopt(ctx, CURLOPT_WRITEFUNCTION, fwrite);
  curl_easy_setopt(ctx, CURLOPT_WRITEDATA, fout);

  CURLcode ec = curl_easy_perform(ctx);
  if (ec)
    fprintf(stderr, "[curl]: %s\n", curl_easy_strerror(ec));
  curl_easy_cleanup(ctx);
  return ec;
}
