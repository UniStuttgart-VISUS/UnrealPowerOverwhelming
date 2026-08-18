// <copyright file="PowerSink.cpp" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#include "PowerSink.h"

#include <cassert>


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
void FPowerSink::Open(const FString& path) {
    auto& file = FPlatformFileManager::Get().GetPlatformFile();
    this->_file = MakeShareable(file.OpenWrite(*path, false, true));

    auto line = FString(TEXT("Timestamp,Sensor,Value")) + LINE_TERMINATOR;
    this->Write(line);
}


/*
 * FPowerSink::Write
 */
void FPowerSink::Write(const FString& line) {
    assert(this->_file.IsValid());
    auto ptr = reinterpret_cast<const uint8 *>(*line);
    auto len = line.Len() * sizeof(TCHAR);
    this->_file->Write(ptr, len);
}
