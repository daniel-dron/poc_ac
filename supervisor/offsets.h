#pragma once

namespace offsets {
namespace process {
constexpr ULONG64 thread_list_head = 0x5e0;
}
namespace thread {
constexpr ULONG64 start_address = 0x450;
constexpr ULONG64 win32_start_address = 0x4d0;
}

#define get_offset(name) offsets::process::##name

} // namespace offsets