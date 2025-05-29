#include <soc_types.h>
#include <soc_mdm_types.h>
#include <soc_mdm_symtab.h>
#include <fpb_lib.h>

/*liteos compilation options are different. config.mk does not take effect. */
#ifdef __MEM_HEAP_STAT_PATCH__
td_pvoid uapi_malloc_patch(td_u32 mod_id, td_u32 size);
td_pvoid uapi_free_patch(td_u32 mod_id, td_pvoid addr);
#endif

extern void osHwiInit_patch(void);
extern td_u32 LOS_HwiCreate_Patch( td_u32  uwHwiNum,td_u16   usHwiPrio,td_u16    usMode,td_pvoid pfnHandler,td_u32 uwArg );

extern void LOS_Panic_Patch(const char * fmt, ...);

extern td_u32 uapi_irq_request_patch(td_u32 vector, td_u16 priority, td_pvoid routine, td_u32 param);

extern td_u32 uapi_irq_free_patch(td_u32 vector);

extern td_void uapi_irq_enable_patch(td_u32 vector);
extern td_void uapi_irq_disable_patch(td_u32 vector);

extern td_u32 uapi_sem_wait_patch_for_track(td_u32 id, td_u32 timeout);

extern td_u32 uapi_event_wait_patch_for_track(td_u32 id, td_u32 mask, EXT_OUT td_u32* event_bits, td_u32 timeout, td_u32 flag);

extern td_u32 uapi_msg_queue_wait_patch_for_track(td_u32 id, td_pvoid msg, td_u32 timeout, td_u32 msg_size);

extern td_u32 irq_bottom_half_handler_patch_for_track(td_pvoid);
extern td_u32 osSwTmrTimeoutHandlePatchForTrack(td_void);

extern void osTimesliceCheckPatch(void);

extern td_pvoid uapi_malloc_patch_malloc_lr(td_u32 mod_id, td_u32 size);

extern td_void osMemSetMagicNumAndTaskidPatch(td_pvoid p);

__isr td_void patch_init(td_void)
{
    fpb_init();
    fpb_add_patch((td_u32)SYMTAB_HWI_INIT,(td_u32)osHwiInit_patch,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_HWI_CREATE,(td_u32)LOS_HwiCreate_Patch,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_LOS_PANIC,(td_u32)LOS_Panic_Patch,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_IRQ_REQ,(td_u32)uapi_irq_request_patch,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_IRQ_FREE,(td_u32)uapi_irq_free_patch,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_IRQ_EN,(td_u32)uapi_irq_enable_patch,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_IRQ_DIS,(td_u32)uapi_irq_disable_patch,FPB_TYPE_INSTR);    
#ifdef __MEM_HEAP_STAT_PATCH__
    fpb_add_patch((td_u32)SYMTAB_MALLOC,(td_u32)uapi_malloc_patch,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_FREE,(td_u32)uapi_free_patch,FPB_TYPE_INSTR);
#endif

    fpb_add_patch((td_u32)SYMTAB_SEM_WAIT,(td_u32)uapi_sem_wait_patch_for_track,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_EVT_WAIT,(td_u32)uapi_event_wait_patch_for_track,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_MSG_WAIT,(td_u32)uapi_msg_queue_wait_patch_for_track,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_IRQ_BOTTOM_HALF,(td_u32)irq_bottom_half_handler_patch_for_track,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_SWTMR_TIMEOUT,(td_u32)osSwTmrTimeoutHandlePatchForTrack,FPB_TYPE_INSTR);

    /* wrong judgment conditions of time debate */
    fpb_add_patch((td_u32)SYMTAB_TIMESLICE_CHECK,(td_u32)osTimesliceCheckPatch,FPB_TYPE_INSTR);


    /* memory application lr statistics */
    fpb_add_patch((td_u32)SYMTAB_MALLOC,(td_u32)uapi_malloc_patch_malloc_lr,FPB_TYPE_INSTR);
    fpb_add_patch((td_u32)SYMTAB_MAM_SET_MAGICNUM_TASKID,(td_u32)osMemSetMagicNumAndTaskidPatch,FPB_TYPE_INSTR);

}
