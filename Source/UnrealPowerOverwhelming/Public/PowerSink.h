// <copyright file="PowerSink.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#include <visus/pwrowg/sample.h>
#include <visus/pwrowg/sensor_description.h>

#include "CoreMinimal.h"


/// <summary>
/// A specialised sink for power samples that uses Unreal-compatible I/O.
/// </summary>
class UNREALPOWEROVERWHELMING_API FPowerSink {

public:

    /// <summary>
    /// Initialises a new instance.
    /// </summary>
    /// <param name="path"></param>
    explicit FPowerSink(const FString& path);

    /// <summary>
    /// Finalises the instance.
    /// </summary>
    /// <param name=""></param>
    virtual ~FPowerSink(void) noexcept;

protected:

    /// <summary>
    /// Writes the samples <paramref name="begin" /> to <paramref name="end" />
    /// to the configured CSV file.
    /// </summary>
    template<class TIterator> void write_samples(const TIterator begin,
        _In_ const TIterator end,
        _In_ const visus::pwrowg::sensor_description *sensors);

private:

    void Write(const FString& line);

    TSharedPtr<IFileHandle> _file;

};

#include "PowerSink.inl"
