#include "id.h"

#include <atomic>

namespace raptor {
	static std::atomic<uint64_t> _next_id = 1;

	auto new_id() -> uint64_t {
		return _next_id.fetch_add(1, std::memory_order_relaxed);
	}
}
