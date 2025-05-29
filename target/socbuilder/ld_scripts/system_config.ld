. = ALIGN(8); __block_sysmemused_start = .; __sysmemused_start = .; . += 0x4; __sysmemused_end = .; . += (__sysmemused_end - __sysmemused_start)*((14 + 1) - 1); __block_sysmemused_end = .;
. = ALIGN(8); __block_errno_start = .; __errno_start = .; . += 0x4; __errno_end = .; . += (__errno_end - __errno_start)*((14 + 1) - 1); __block_errno_end = .;
