// Copyright (C) 2010-2015 Joshua Boyce
// See the file COPYING for copying permission.

#include "gui.hpp"

#include <cstddef>
#include <cstdint>

#include <hadesmem/config.hpp>
#include <hadesmem/detail/dump.hpp>
#include <hadesmem/detail/trace.hpp>
#include <hadesmem/error.hpp>

#include <cerberus/ant_tweak_bar.hpp>
#include <cerberus/plugin.hpp>
#include <cerberus/render.hpp>

namespace
{
std::size_t g_on_ant_tweak_bar_initialize_callback_id =
  static_cast<std::size_t>(-1);

std::size_t g_on_ant_tweak_bar_cleanup_callback_id =
  static_cast<std::size_t>(-1);

TwBar* g_tweak_bar = nullptr;

void TW_CALL DumpCallbackTw(void* /*client_data*/)
{
  try
  {
    hadesmem::detail::DumpMemory();
  }
  catch (...)
  {
    HADESMEM_DETAIL_TRACE_A(
      boost::current_exception_diagnostic_information().c_str());
    HADESMEM_DETAIL_ASSERT(false);
  }
}

void OnAntTweakBarInitialize(
  hadesmem::cerberus::AntTweakBarInterface* ant_tweak_bar)
{
  HADESMEM_DETAIL_TRACE_A("Initializing AntTweakBar.");

  if (g_tweak_bar)
  {
    HADESMEM_DETAIL_TRACE_A("AntTweakBar is already initialized. Skipping.");
    return;
  }

  if (!ant_tweak_bar->IsInitialized())
  {
    HADESMEM_DETAIL_TRACE_A(
      "AntTweakBar is not initialized by Cerberus. Skipping.");
    return;
  }

  g_tweak_bar = ant_tweak_bar->TwNewBar("CXExample");
  if (!g_tweak_bar)
  {
    HADESMEM_DETAIL_THROW_EXCEPTION(hadesmem::Error{}
                                    << hadesmem::ErrorString{"TwNewBar failed."}
                                    << hadesmem::ErrorStringOther{
                                         ant_tweak_bar->TwGetLastError()});
  }

  auto const dump_button = ant_tweak_bar->TwAddButton(g_tweak_bar,
                                                      "CXExample_DumpBtn",
                                                      &DumpCallbackTw,
                                                      nullptr,
                                                      " label='Dump' ");
  if (!dump_button)
  {
    HADESMEM_DETAIL_THROW_EXCEPTION(
      hadesmem::Error{} << hadesmem::ErrorString{"TwAddButton failed."}
                        << hadesmem::ErrorStringOther{
                             ant_tweak_bar->TwGetLastError()});
  }
}

void OnAntTweakBarCleanup(
  hadesmem::cerberus::AntTweakBarInterface* ant_tweak_bar)
{
  HADESMEM_DETAIL_TRACE_A("Cleaning up AntTweakBar.");

  if (!ant_tweak_bar->IsInitialized())
  {
    HADESMEM_DETAIL_TRACE_A(
      "AntTweakBar is not initialized by Cerberus. Skipping.");
    return;
  }

  if (g_tweak_bar != nullptr)
  {
    ant_tweak_bar->TwDeleteBar(g_tweak_bar);
    g_tweak_bar = nullptr;
  }
}
}

void InitializeGui(hadesmem::cerberus::PluginInterface* cerberus)
{
  auto ant_tweak_bar = cerberus->GetAntTweakBarInterface();
  g_on_ant_tweak_bar_initialize_callback_id =
    ant_tweak_bar->RegisterOnInitialize(&OnAntTweakBarInitialize);
  g_on_ant_tweak_bar_cleanup_callback_id =
    ant_tweak_bar->RegisterOnCleanup(&OnAntTweakBarCleanup);

  OnAntTweakBarInitialize(ant_tweak_bar);
}

void CleanupGui(hadesmem::cerberus::PluginInterface* cerberus)
{
  auto ant_tweak_bar = cerberus->GetAntTweakBarInterface();

  if (g_on_ant_tweak_bar_initialize_callback_id != static_cast<std::size_t>(-1))
  {
    ant_tweak_bar->UnregisterOnInitialize(
      g_on_ant_tweak_bar_initialize_callback_id);
  }

  if (g_on_ant_tweak_bar_cleanup_callback_id != static_cast<std::size_t>(-1))
  {
    ant_tweak_bar->UnregisterOnCleanup(g_on_ant_tweak_bar_cleanup_callback_id);
  }

  OnAntTweakBarCleanup(ant_tweak_bar);
}
