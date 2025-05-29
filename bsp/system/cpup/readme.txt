cpup_core.c-----core code for cpu occupancy rate statistics
﻿cpup_diag_dfx.c-----command of cpu occupancy rate statistics. If the user does not use diag diagnostic tool, it is recommended to delete it.
﻿cpup_usr.c-----demo, users can modify according to their own needs, users need to understand.
﻿
﻿The module supports SDK development.
﻿1.This module only allows linking to the program through the process interface provided in cpup_usr.c and the secondary development interface provided in cpup_core.c.
﻿2.All code that calls the module interface needs to be open source.
