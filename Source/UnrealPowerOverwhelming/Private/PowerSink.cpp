// <copyright file="PowerSink.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "PowerSink.h"

#include <cassert>

#include "UnrealPowerOverwhelming.h"


/*
 * FPowerSink::FPowerSink
 */
FPowerSink::FPowerSink(void) { }


/*
 * FPowerSink::~FPowerSink
 */
FPowerSink::~FPowerSink(void) noexcept { }


/*
 * FPowerSink::Open
 */
bool FPowerSink::Open(const FString& path) {
    auto& file = FPlatformFileManager::Get().GetPlatformFile();
    this->_file = MakeShareable(file.OpenWrite(*path, false, true));
    if (this->_file == nullptr) {
        return false;
    }

    UE_LOG(LogPwrOwg, Verbose, TEXT("Power log %s opened."), *path);
    auto line = FString(TEXT("Timestamp,Sensor,Value")) + LINE_TERMINATOR;
    return this->Write(line);
}


/*
 * FPowerSink::Write
 */
bool FPowerSink::Write(const FString& line) {
    assert(this->_file.IsValid());
    auto ptr = reinterpret_cast<const uint8 *>(*line);
    auto len = line.Len() * sizeof(TCHAR);
    return this->_file->Write(ptr, len);
}
