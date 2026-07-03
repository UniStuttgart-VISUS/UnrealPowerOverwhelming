// <copyright file="PowerSink.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2026 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>


/*
 * FPowerSink::write_samples
 */
template<class TIterator>
void FPowerSink::write_samples(const TIterator begin, const TIterator end,
        const visus::pwrowg::sensor_description *sensors) {
    for (auto it = begin; it != end; ++it) {
        auto& desc = sensors[it->source];
        FString line;

        switch (desc.reading_type()) {
            case visus::pwrowg::reading_type::floating_point:
                line = FString::Printf(
                    TEXT("%lld,%d,%f%s"),
                    it->timestamp.value(),
                    it->source,
                    it->reading.floating_point,
                    LINE_TERMINATOR);
                break;

            case visus::pwrowg::reading_type::signed_integer:
                line = FString::Printf(
                    TEXT("%lld,%d,%d%s"),
                    it->timestamp.value(),
                    it->source,
                    it->reading.signed_integer,
                    LINE_TERMINATOR);
                break;

            case visus::pwrowg::reading_type::unsigned_integer:
                line = FString::Printf(
                    TEXT("%lld,%d,%u%s"),
                    it->timestamp.value(),
                    it->source,
                    it->reading.unsigned_integer,
                    LINE_TERMINATOR);
                break;
        }

        this->Write(line);
    }
}
