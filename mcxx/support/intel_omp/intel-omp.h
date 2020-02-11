#ifndef INTEL_OMP_H
#define INTEL_OMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Public interface for the Intel OpenMP RTL
 * used by Mercurium
 */

/* Basic types */

enum {
 KMP_IDENT_IMB = 0x01,
 KMP_IDENT_KMPC = 0x02,
 KMP_IDENT_AUTOPAR = 0x08,
 KMP_IDENT_ATOMIC_REDUCE = 0x10,
 KMP_IDENT_BARRIER_EXPL = 0x20,
 KMP_IDENT_BARRIER_IMPL = 0x40,
};

typedef int8_t kmp_int8;
typedef uint8_t kmp_uint8;
typedef int16_t kmp_int16;
typedef uint16_t kmp_uint16;
typedef int32_t kmp_int32;
typedef int64_t kmp_int64;
typedef uint32_t kmp_uint32;
typedef uint64_t kmp_uint64;
typedef intptr_t kmp_intptr_t;
typedef uintptr_t kmp_uintptr_t;

typedef struct ident {
 kmp_int32 reserved_1;
 kmp_int32 flags;
 kmp_int32 reserved_2;
 kmp_int32 reserved_3;
 const char *psource; // Note: Intel defines this as char*
} ident_t;

/* Atomic */

#include "intel-omp-atomics.h"

/* Startup and shutdown */

void __kmpc_begin(ident_t* loc, kmp_int32 flags);
void __kmpc_end(ident_t* loc);

/* Parallel fork/join */

typedef void (*kmpc_micro)(kmp_int32* global_tid, kmp_int32* bound_tid, ...);

void __kmpc_push_num_threads (ident_t *loc, kmp_int32 global_tid, kmp_int32 num_threads);
void __kmpc_fork_call (ident_t *loc, kmp_int32 argc, kmpc_micro microtask,...);
void __kmpc_push_num_teams (ident_t *loc, kmp_int32 global_tid, kmp_int32 num_teams, kmp_int32 num_threads);
#ifdef INTEL_OMP_SIMD
void __kmpc_push_estimated_reduction_info(ident_t *loc, kmp_int32 global_tid,
        kmp_uint32 estimated_red_vars, size_t * estimated_red_sizes);
#endif
void __kmpc_fork_teams (ident_t *loc, kmp_int32 argc, kmpc_micro microtask,...);
void __kmpc_serialized_parallel (ident_t *loc, kmp_int32 global_tid);
void __kmpc_end_serialized_parallel (ident_t *loc, kmp_int32 global_tid);

/* Thread information */

kmp_int32 __kmpc_global_thread_num (ident_t *loc);
kmp_int32 __kmpc_global_num_threads (ident_t *loc);
kmp_int32 __kmpc_bound_thread_num (ident_t *loc);
kmp_int32 __kmpc_bound_num_threads (ident_t *loc);
kmp_int32 __kmpc_in_parallel (ident_t *loc);

/* Worksharing */

enum sched_type {
    kmp_sch_lower                     = 32,   /**< lower bound for unordered values */
    kmp_sch_static_chunked            = 33,
    kmp_sch_static                    = 34,   /**< static unspecialized */
    kmp_sch_dynamic_chunked           = 35,
    kmp_sch_guided_chunked            = 36,   /**< guided unspecialized */
    kmp_sch_runtime                   = 37,
    kmp_sch_auto                      = 38,   /**< auto */
    kmp_sch_trapezoidal               = 39,

    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_sch_static_greedy             = 40,
    kmp_sch_static_balanced           = 41,
    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_sch_guided_iterative_chunked  = 42,
    kmp_sch_guided_analytical_chunked = 43,

    kmp_sch_static_steal              = 44,   /**< accessible only through KMP_SCHEDULE environment variable */

    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_sch_upper                     = 45,   /**< upper bound for unordered values */

    kmp_ord_lower                     = 64,   /**< lower bound for ordered values, must be power of 2 */
    kmp_ord_static_chunked            = 65,
    kmp_ord_static                    = 66,   /**< ordered static unspecialized */
    kmp_ord_dynamic_chunked           = 67,
    kmp_ord_guided_chunked            = 68,
    kmp_ord_runtime                   = 69,
    kmp_ord_auto                      = 70,   /**< ordered auto */
    kmp_ord_trapezoidal               = 71,
    kmp_ord_upper                     = 72,   /**< upper bound for ordered values */

    /* Schedules for Distribute construct */
    kmp_distribute_static_chunked     = 91,   /**< distribute static chunked */
    kmp_distribute_static             = 92,   /**< distribute static unspecialized */

    /*
     * For the "nomerge" versions, kmp_dispatch_next*() will always return
     * a single iteration/chunk, even if the loop is serialized.  For the
     * schedule types listed above, the entire iteration vector is returned
     * if the loop is serialized.  This doesn't work for gcc/gcomp sections.
     */
    kmp_nm_lower                      = 160,  /**< lower bound for nomerge values */

    kmp_nm_static_chunked             = (kmp_sch_static_chunked - kmp_sch_lower + kmp_nm_lower),
    kmp_nm_static                     = 162,  /**< static unspecialized */
    kmp_nm_dynamic_chunked            = 163,
    kmp_nm_guided_chunked             = 164,  /**< guided unspecialized */
    kmp_nm_runtime                    = 165,
    kmp_nm_auto                       = 166,  /**< auto */
    kmp_nm_trapezoidal                = 167,

    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_nm_static_greedy              = 168,
    kmp_nm_static_balanced            = 169,
    /* accessible only through KMP_SCHEDULE environment variable */
    kmp_nm_guided_iterative_chunked   = 170,
    kmp_nm_guided_analytical_chunked  = 171,
    kmp_nm_static_steal               = 172,  /* accessible only through OMP_SCHEDULE environment variable */

    kmp_nm_ord_static_chunked         = 193,
    kmp_nm_ord_static                 = 194,  /**< ordered static unspecialized */
    kmp_nm_ord_dynamic_chunked        = 195,
    kmp_nm_ord_guided_chunked         = 196,
    kmp_nm_ord_runtime                = 197,
    kmp_nm_ord_auto                   = 198,  /**< auto */
    kmp_nm_ord_trapezoidal            = 199,
    kmp_nm_upper                      = 200,  /**< upper bound for nomerge values */

    kmp_sch_default = kmp_sch_static  /**< default scheduling algorithm */
};


void __kmps_set_schedule( enum sched_type kind, int modifier );
void __kmps_get_schedule( enum sched_type *kind, int *modifier );

typedef kmp_int32 kmp_critical_name[8];

kmp_int32 __kmpc_master (ident_t *loc, kmp_int32 global_tid);
void __kmpc_end_master (ident_t *loc, kmp_int32 global_tid);
void __kmpc_ordered (ident_t *loc, kmp_int32 gtid);
void __kmpc_end_ordered (ident_t *loc, kmp_int32 gtid);
void __kmpc_critical (ident_t *loc, kmp_int32 global_tid, kmp_critical_name *crit);
void __kmpc_end_critical (ident_t *loc, kmp_int32 global_tid, kmp_critical_name *crit);
kmp_int32 __kmpc_single (ident_t *loc, kmp_int32 global_tid);
void __kmpc_end_single (ident_t *loc, kmp_int32 global_tid);
void __kmpc_for_static_fini (ident_t *loc, kmp_int32 global_tid);
void __kmpc_dispatch_init_4 (ident_t *loc, kmp_int32 gtid, enum sched_type schedule, kmp_int32 lb, kmp_int32 ub, kmp_int32 st, kmp_int32 chunk);
void __kmpc_dispatch_init_4u (ident_t *loc, kmp_int32 gtid, enum sched_type schedule, kmp_uint32 lb, kmp_uint32 ub, kmp_int32 st, kmp_int32 chunk);
void __kmpc_dispatch_init_8 (ident_t *loc, kmp_int32 gtid, enum sched_type schedule, kmp_int64 lb, kmp_int64 ub, kmp_int64 st, kmp_int64 chunk);
void __kmpc_dispatch_init_8u (ident_t *loc, kmp_int32 gtid, enum sched_type schedule, kmp_uint64 lb, kmp_uint64 ub, kmp_int64 st, kmp_int64 chunk);
int __kmpc_dispatch_next_4 (ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last, kmp_int32 *p_lb, kmp_int32 *p_ub, kmp_int32 *p_st);
int __kmpc_dispatch_next_4u (ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last, kmp_uint32 *p_lb, kmp_uint32 *p_ub, kmp_int32 *p_st);
int __kmpc_dispatch_next_8 (ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last, kmp_int64 *p_lb, kmp_int64 *p_ub, kmp_int64 *p_st);
int __kmpc_dispatch_next_8u (ident_t *loc, kmp_int32 gtid, kmp_int32 *p_last, kmp_uint64 *p_lb, kmp_uint64 *p_ub, kmp_int64 *p_st);
void __kmpc_dispatch_fini_4 (ident_t *loc, kmp_int32 gtid);
void __kmpc_dispatch_fini_8 (ident_t *loc, kmp_int32 gtid);
void __kmpc_dispatch_fini_4u (ident_t *loc, kmp_int32 gtid);
void __kmpc_dispatch_fini_8u (ident_t *loc, kmp_int32 gtid);
void __kmpc_for_static_init_4 (ident_t *loc, kmp_int32 gtid, enum sched_type schedtype, kmp_int32 *plastiter, kmp_int32 *plower, kmp_int32 *pupper, kmp_int32 *pstride, kmp_int32 incr, kmp_int32 chunk);
void __kmpc_for_static_init_4u (ident_t *loc, kmp_int32 gtid, enum sched_type schedtype, kmp_int32 *plastiter, kmp_uint32 *plower, kmp_uint32 *pupper, kmp_int32 *pstride, kmp_int32 incr, kmp_int32 chunk);
void __kmpc_for_static_init_8 (ident_t *loc, kmp_int32 gtid, enum sched_type schedtype, kmp_int32 *plastiter, kmp_int64 *plower, kmp_int64 *pupper, kmp_int64 *pstride, kmp_int64 incr, kmp_int64 chunk);
void __kmpc_for_static_init_8u (ident_t *loc, kmp_int32 gtid, enum sched_type schedtype, kmp_int32 *plastiter, kmp_uint64 *plower, kmp_uint64 *pupper, kmp_int64 *pstride, kmp_int64 incr, kmp_int64 chunk);

/* Synchronization */

void __kmpc_flush (ident_t *loc,...);
void __kmpc_barrier (ident_t *loc, kmp_int32 global_tid);
kmp_int32 __kmpc_barrier_master (ident_t *loc, kmp_int32 global_tid);
void __kmpc_end_barrier_master (ident_t *loc, kmp_int32 global_tid);
kmp_int32 __kmpc_barrier_master_nowait (ident_t *loc, kmp_int32 global_tid);
#ifndef INTEL_OMP_SIMD
kmp_int32 __kmpc_reduce_nowait (ident_t *loc, kmp_int32 global_tid, kmp_int32 num_vars, size_t reduce_size, void *reduce_data, void(*reduce_func)(void *lhs_data, void *rhs_data), kmp_critical_name *lck);
#else
kmp_int32 __kmpc_reduce_nowait (ident_t *loc, kmp_int32 global_tid, kmp_int32 num_vars, size_t* reduce_size, void **reduce_data, void(*reduce_func)(void *lhs_data, void *rhs_data), kmp_critical_name *lck);
#endif
void __kmpc_end_reduce_nowait (ident_t *loc, kmp_int32 global_tid, kmp_critical_name *lck);
#ifndef INTEL_OMP_SIMD
kmp_int32 __kmpc_reduce (ident_t *loc, kmp_int32 global_tid, kmp_int32 num_vars, size_t reduce_size, void *reduce_data, void(*reduce_func)(void *lhs_data, void *rhs_data), kmp_critical_name *lck);
#else
kmp_int32 __kmpc_reduce (ident_t *loc, kmp_int32 global_tid, kmp_int32 num_vars, size_t * reduce_size, void **reduce_data, void(*reduce_func)(void *lhs_data, void *rhs_data), kmp_critical_name *lck);
#endif
void __kmpc_end_reduce (ident_t *loc, kmp_int32 global_tid, kmp_critical_name *lck);

/* Threadprivate data support */

typedef void *(* kmpc_ctor )(void *);
typedef void(* kmpc_dtor )(void *);
typedef void *(* kmpc_cctor )(void *, void *);
typedef void *(* kmpc_ctor_vec )(void *, size_t);
typedef void(* kmpc_dtor_vec )(void *, size_t);
typedef void *(* kmpc_cctor_vec )(void *, void *, size_t);

void __kmpc_copyprivate (ident_t *loc, kmp_int32 gtid, size_t cpy_size, void *cpy_data, void(*cpy_func)(void *, void *), kmp_int32 didit);
void __kmpc_threadprivate_register (ident_t *loc, void *data, kmpc_ctor ctor, kmpc_cctor cctor, kmpc_dtor dtor);
void * __kmpc_threadprivate_cached (ident_t *loc, kmp_int32 global_tid, void *data, size_t size, void ***cache);
void __kmpc_threadprivate_register_vec (ident_t *loc, void *data, kmpc_ctor_vec ctor, kmpc_cctor_vec cctor, kmpc_dtor_vec dtor, size_t vector_length);

/* Task support */

typedef kmp_int32 (* kmp_routine_entry_t)( kmp_int32, void * );

typedef union kmp_cmplrdata {
    kmp_int32           priority;           /**< priority specified by user for the task */
    kmp_routine_entry_t destructors;        /* pointer to function to invoke deconstructors of firstprivate C++ objects */
    /* future data */
} kmp_cmplrdata_t;

/*  sizeof_kmp_task_t passed as arg to kmpc_omp_task call  */
typedef struct kmp_task {                   /* GEH: Shouldn't this be aligned somehow? */
    void *              shareds;            /**< pointer to block of pointers to shared vars   */
    kmp_routine_entry_t routine;            /**< pointer to routine to call for executing task */
    kmp_int32           part_id;            /**< part id for the task                          */
    kmp_cmplrdata_t data1;                  /* Two known optional additions: destructors and priority */
    kmp_cmplrdata_t data2;                  /* Process destructors first, priority second */
    /* future data */
    /*  private vars  */
} kmp_task_t;

typedef struct kmp_taskgroup {
    kmp_uint32            count;   // number of allocated and not yet complete tasks
    kmp_int32             cancel_request; // request for cancellation of this taskgroup
    struct kmp_taskgroup *parent;  // parent taskgroup
} kmp_taskgroup_t;

typedef struct kmp_depend_info {
     kmp_intptr_t               base_addr;
     size_t                     len;
     struct {
         bool                   in:1;
         bool                   out:1;
     } flags;
} kmp_depend_info_t;

typedef struct kmp_task_red_flags {
    unsigned lazy_priv : 1;     // hint: (1) use lazy allocation (big objects)
    unsigned reserved31 : 31;
} kmp_task_red_flags_t;

// structure sent us by compiler - one per reduction item
typedef struct kmp_task_red_input {
    void *reduce_shar;          // shared reduction item
    size_t reduce_size;         // size of data item
    void *reduce_init;          // data initialization routine
    void *reduce_fini;          // data finalization routine
    void *reduce_comb;          // data combiner routine
    kmp_task_red_flags_t flags; // flags for additional info from compiler
} kmp_task_red_input_t;

kmp_int32 __kmpc_omp_task(ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *new_task);
kmp_task_t* __kmpc_omp_task_alloc(ident_t *loc_ref,
                                  kmp_int32 gtid,
                                  kmp_int32 flags,
                                  size_t sizeof_kmp_task_t,
                                  size_t sizeof_shareds,
                                  kmp_routine_entry_t task_entry);

void __kmpc_omp_task_begin_if0(ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *task);
void __kmpc_omp_task_complete_if0(ident_t *loc_ref, kmp_int32 gtid, kmp_task_t *task);

kmp_int32 __kmpc_omp_taskyield(ident_t *loc_ref, kmp_int32 gtid, int end_part);

void __kmpc_taskloop(ident_t *loc,
                     kmp_int32 gtid,
                     kmp_task_t *task,
                     kmp_int32 if_val,
                     kmp_uint64 *lb,
                     kmp_uint64 *ub,
                     kmp_int64 st,
                     kmp_int32 nogroup,
                     kmp_int32 sched,
                     kmp_uint64 grainsize,
                     void *task_dup);


kmp_int32 __kmpc_omp_taskwait(ident_t *loc_ref, kmp_int32 gtid);
void __kmpc_taskgroup(ident_t *loc, int gtid);
void __kmpc_end_taskgroup(ident_t *loc, int gtid);

void *__kmpc_task_reduction_init(int gtid, int num_data, void *data);
void *__kmpc_task_reduction_get_th_data(int gtid, void *tg, void *d);

kmp_int32 __kmpc_omp_task_with_deps(ident_t *loc_ref,
                                    kmp_int32 gtid,
                                    kmp_task_t *new_task,
                                    kmp_int32 ndeps,
                                    kmp_depend_info_t *dep_list,
                                    kmp_int32 ndeps_noalias,
                                    kmp_depend_info_t *noalias_dep_list);

void __kmpc_omp_wait_deps(ident_t *loc_ref,
                          kmp_int32 gtid,
                          kmp_int32 ndeps,
                          kmp_depend_info_t *dep_list,
                          kmp_int32 ndeps_noalias,
                          kmp_depend_info_t *noalias_dep_list);


#ifdef __cplusplus
}
#endif

#endif // INTEL_OMP_H
