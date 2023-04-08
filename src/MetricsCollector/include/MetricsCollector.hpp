#pragma once

#include <unordered_map>

namespace MetricsCollector {
	class DeviceMetricsCollector
	{
	public:
		DeviceMetricsCollector() = default;

	private:
		std::unordered_map<uint32_t, uint64_t> m_counterMap;
	};
}