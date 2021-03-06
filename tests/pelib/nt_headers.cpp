// Copyright (C) 2010-2015 Joshua Boyce
// See the file COPYING for copying permission.

#include <hadesmem/pelib/nt_headers.hpp>
#include <hadesmem/pelib/nt_headers.hpp>

#include <sstream>
#include <utility>

#include <hadesmem/detail/warning_disable_prefix.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <hadesmem/detail/warning_disable_suffix.hpp>

#include <hadesmem/config.hpp>
#include <hadesmem/error.hpp>
#include <hadesmem/module.hpp>
#include <hadesmem/module_list.hpp>
#include <hadesmem/pelib/pe_file.hpp>
#include <hadesmem/process.hpp>
#include <hadesmem/read.hpp>

void TestNtHeaders()
{
  hadesmem::Process const process(::GetCurrentProcessId());

  hadesmem::PeFile pe_file_1(
    process, ::GetModuleHandleW(nullptr), hadesmem::PeFileType::Image, 0);

  hadesmem::NtHeaders nt_headers_1(process, pe_file_1);

  hadesmem::NtHeaders nt_headers_2(nt_headers_1);
  BOOST_TEST_EQ(nt_headers_1, nt_headers_2);
  nt_headers_1 = nt_headers_2;
  BOOST_TEST_EQ(nt_headers_1, nt_headers_2);
  hadesmem::NtHeaders nt_headers_3(std::move(nt_headers_2));
  BOOST_TEST_EQ(nt_headers_3, nt_headers_1);
  nt_headers_2 = std::move(nt_headers_3);
  BOOST_TEST_EQ(nt_headers_1, nt_headers_2);

  hadesmem::ModuleList modules(process);
  for (auto const& mod : modules)
  {
    hadesmem::PeFile const cur_pe_file(
      process, mod.GetHandle(), hadesmem::PeFileType::Image, 0);

    hadesmem::NtHeaders cur_nt_headers(process, cur_pe_file);

    auto const nt_headers_raw =
      hadesmem::Read<IMAGE_NT_HEADERS>(process, cur_nt_headers.GetBase());

    BOOST_TEST_EQ(cur_nt_headers.IsValid(), true);
    cur_nt_headers.EnsureValid();
    cur_nt_headers.SetSignature(cur_nt_headers.GetSignature());
    cur_nt_headers.SetMachine(cur_nt_headers.GetMachine());
    cur_nt_headers.SetNumberOfSections(cur_nt_headers.GetNumberOfSections());
    cur_nt_headers.SetTimeDateStamp(cur_nt_headers.GetTimeDateStamp());
    cur_nt_headers.SetPointerToSymbolTable(
      cur_nt_headers.GetPointerToSymbolTable());
    cur_nt_headers.SetNumberOfSymbols(cur_nt_headers.GetNumberOfSymbols());
    cur_nt_headers.SetSizeOfOptionalHeader(
      cur_nt_headers.GetSizeOfOptionalHeader());
    cur_nt_headers.SetCharacteristics(cur_nt_headers.GetCharacteristics());
    cur_nt_headers.SetMagic(cur_nt_headers.GetMagic());
    cur_nt_headers.SetMajorLinkerVersion(
      cur_nt_headers.GetMajorLinkerVersion());
    cur_nt_headers.SetMinorLinkerVersion(
      cur_nt_headers.GetMinorLinkerVersion());
    cur_nt_headers.SetSizeOfCode(cur_nt_headers.GetSizeOfCode());
    cur_nt_headers.SetSizeOfInitializedData(
      cur_nt_headers.GetSizeOfInitializedData());
    cur_nt_headers.SetSizeOfUninitializedData(
      cur_nt_headers.GetSizeOfUninitializedData());
    cur_nt_headers.SetAddressOfEntryPoint(
      cur_nt_headers.GetAddressOfEntryPoint());
    cur_nt_headers.SetBaseOfCode(cur_nt_headers.GetBaseOfCode());
#if defined(HADESMEM_DETAIL_ARCH_X86)
    cur_nt_headers.SetBaseOfData(cur_nt_headers.GetBaseOfData());
#endif
    cur_nt_headers.SetImageBase(cur_nt_headers.GetImageBase());
    cur_nt_headers.SetSectionAlignment(cur_nt_headers.GetSectionAlignment());
    cur_nt_headers.SetFileAlignment(cur_nt_headers.GetFileAlignment());
    cur_nt_headers.SetMajorOperatingSystemVersion(
      cur_nt_headers.GetMajorOperatingSystemVersion());
    cur_nt_headers.SetMinorOperatingSystemVersion(
      cur_nt_headers.GetMinorOperatingSystemVersion());
    cur_nt_headers.SetMajorImageVersion(cur_nt_headers.GetMajorImageVersion());
    cur_nt_headers.SetMinorImageVersion(cur_nt_headers.GetMinorImageVersion());
    cur_nt_headers.SetMajorSubsystemVersion(
      cur_nt_headers.GetMajorSubsystemVersion());
    cur_nt_headers.SetMinorSubsystemVersion(
      cur_nt_headers.GetMinorSubsystemVersion());
    cur_nt_headers.SetWin32VersionValue(cur_nt_headers.GetWin32VersionValue());
    cur_nt_headers.SetSizeOfImage(cur_nt_headers.GetSizeOfImage());
    cur_nt_headers.SetSizeOfHeaders(cur_nt_headers.GetSizeOfHeaders());
    cur_nt_headers.SetCheckSum(cur_nt_headers.GetCheckSum());
    cur_nt_headers.SetSubsystem(cur_nt_headers.GetSubsystem());
    cur_nt_headers.SetDllCharacteristics(
      cur_nt_headers.GetDllCharacteristics());
    cur_nt_headers.SetSizeOfStackReserve(
      cur_nt_headers.GetSizeOfStackReserve());
    cur_nt_headers.SetSizeOfStackCommit(cur_nt_headers.GetSizeOfStackCommit());
    cur_nt_headers.SetSizeOfHeapReserve(cur_nt_headers.GetSizeOfHeapReserve());
    cur_nt_headers.SetSizeOfHeapCommit(cur_nt_headers.GetSizeOfHeapCommit());
    cur_nt_headers.SetLoaderFlags(cur_nt_headers.GetLoaderFlags());
    cur_nt_headers.SetNumberOfRvaAndSizes(
      cur_nt_headers.GetNumberOfRvaAndSizes());
    for (std::size_t i = 0; i < cur_nt_headers.GetNumberOfRvaAndSizes(); ++i)
    {
      auto data_dir = static_cast<hadesmem::PeDataDir>(i);
      cur_nt_headers.SetDataDirectoryVirtualAddress(
        data_dir, cur_nt_headers.GetDataDirectoryVirtualAddress(data_dir));
      cur_nt_headers.SetDataDirectorySize(
        data_dir, cur_nt_headers.GetDataDirectorySize(data_dir));
    }
    cur_nt_headers.UpdateWrite();
    cur_nt_headers.UpdateRead();

    auto const nt_headers_raw_new =
      hadesmem::Read<IMAGE_NT_HEADERS>(process, cur_nt_headers.GetBase());

    BOOST_TEST_EQ(
      std::memcmp(&nt_headers_raw, &nt_headers_raw_new, sizeof(nt_headers_raw)),
      0);

    std::stringstream test_str_1;
    test_str_1.imbue(std::locale::classic());
    test_str_1 << cur_nt_headers;
    std::stringstream test_str_2;
    test_str_2.imbue(std::locale::classic());
    test_str_2 << cur_nt_headers.GetBase();
    BOOST_TEST_EQ(test_str_1.str(), test_str_2.str());
    if (mod.GetHandle() != ::GetModuleHandle(L"ntdll"))
    {
      hadesmem::PeFile const pe_file_ntdll(
        process, ::GetModuleHandleW(L"ntdll"), hadesmem::PeFileType::Image, 0);
      hadesmem::NtHeaders const nt_headers_ntdll(process, pe_file_ntdll);
      std::stringstream test_str_3;
      test_str_3.imbue(std::locale::classic());
      test_str_3 << nt_headers_ntdll.GetBase();
      BOOST_TEST_NE(test_str_1.str(), test_str_3.str());
    }
  }
}

int main()
{
  TestNtHeaders();
  return boost::report_errors();
}
