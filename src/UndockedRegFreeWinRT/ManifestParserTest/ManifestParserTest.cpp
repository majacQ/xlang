// ManifestParserTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "..\UndockedRegFreeWinRT\catalog.h"
#include "..\UndockedRegFreeWinRT\catalog.cpp"
#include "..\UndockedRegFreeWinRT\typeresolution.h"
#include "..\UndockedRegFreeWinRT\typeresolution.cpp"
#include "pch.h"

#define RegFreeWinrtInprocComponents1 L"RegFreeWinrtInprocComponents.1.dll"
#define FakeDllName L"FakeDllName.dll"
#define OtherFakeDllName L"OtherFakeDllName.dll"

TEST_CASE("Manifest Parser")
{
    g_types.clear();
    PCWSTR exeFilePath = nullptr;
    UndockedRegFreeWinRT::GetProcessExeDir(&exeFilePath);
    SECTION("Multiple activatableClass")
    {
        std::wstring manifest = std::wstring(exeFilePath) + L"\\winrtActivation.dll1.manifest";
        REQUIRE(LoadFromSxSManifest(manifest.c_str()) == S_OK);
        std::wstring moduleName = RegFreeWinrtInprocComponents1;
        REQUIRE(g_types.size() == 4);
        auto component_iter = g_types.find(L"RegFreeWinrt.1.threading_both");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);
        REQUIRE(component_iter->second->threading_model == ABI::Windows::Foundation::ThreadingType::ThreadingType_BOTH);

        component_iter = g_types.find(L"RegFreeWinrt.1.threading_mta");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);
        REQUIRE(component_iter->second->threading_model == ABI::Windows::Foundation::ThreadingType::ThreadingType_MTA);

        component_iter = g_types.find(L"RegFreeWinrt.1.threading_sta");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);
        REQUIRE(component_iter->second->threading_model == ABI::Windows::Foundation::ThreadingType::ThreadingType_STA);

        component_iter = g_types.find(L"RegFreeWinrt.SharedName.threading_both");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);
        REQUIRE(component_iter->second->threading_model == ABI::Windows::Foundation::ThreadingType::ThreadingType_BOTH);
    }
    SECTION("Multiple file")
    {
        std::wstring manifest = std::wstring(exeFilePath) + L"\\basicParse.Positive.manifest";
        REQUIRE(LoadFromSxSManifest(manifest.c_str()) == S_OK);
        std::wstring moduleName = FakeDllName;
        REQUIRE(g_types.size() == 6);
        auto component_iter = g_types.find(L"FakeActivatableClass");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);

        component_iter = g_types.find(L"FakeActivatableClass2");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);

        component_iter = g_types.find(L"FakeActivatableClass3");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);

        component_iter = g_types.find(L"FakeActivatableClass4");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);

        moduleName = OtherFakeDllName;
        component_iter = g_types.find(L"OtherFakeActivatableClass");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);

        component_iter = g_types.find(L"OtherFakeActivatableClass2");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == moduleName);
    }
    SECTION("Dependent Assembly")
    {
        std::wstring manifest = std::wstring(exeFilePath) + L"\\validateDependentManifestMix.root.manifest";
        REQUIRE(LoadFromSxSManifest(manifest.c_str()) == S_OK);
        REQUIRE(g_types.size() == 4);
        auto component_iter = g_types.find(L"RootActivatableClass");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == L"root.dll");

        component_iter = g_types.find(L"sub1ActivatableClass");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == L"sub1.dll");

        component_iter = g_types.find(L"sub2ActivatableClass");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == L"sub2.dll");

        component_iter = g_types.find(L"sub1_1ActivatableClass");
        REQUIRE(component_iter != g_types.end());
        REQUIRE(component_iter->second->module_name == L"sub1_1.dll");
    }
}