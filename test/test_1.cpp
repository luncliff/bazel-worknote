#include <cstdint>
#include <cstdio>

#include <Windows.h>

uint32_t init(int argc, wchar_t *argv[]) noexcept;

// https://docs.microsoft.com/en-us/cpp/c-language/using-wmain
int wmain(int argc, wchar_t *argv[], wchar_t *envp[]) {
  if (auto ec = init(argc, argv))
    return ec;

  // application handle
  HANDLE const happ = GetModuleHandleW(argv[0]);
  wprintf_s(L"app: %p\n", happ);

  // environment variables
  for (auto *env = envp; *env != nullptr; ++env) {
    wprintf_s(L"env: %s\n", *env);
  }
  return GetLastError();
}

uint32_t init(int argc, wchar_t *argv[]) noexcept {
  // argument vector
  for (auto i = 0; i < argc; ++i) {
    wprintf_s(L"argv %2d: %s\n", i, argv[i]);
  }
  return GetLastError();
}
