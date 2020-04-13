//#include "nanos-max.h"
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NANOS_API_DECL(Type, Name, Params) \
    extern Type Name##_ Params; \
    extern Type Name Params




const int N = 16;
const int NN = 16 * 16;
struct  nanos_args_0_t
{
  double *a;
  double *b;
  double *c;
};
typedef void *nanos_wd_t;
enum mcc_enum_anon_5
{
  NANOS_OK = 0,
  NANOS_UNKNOWN_ERR = 1,
  NANOS_UNIMPLEMENTED = 2,
  NANOS_ENOMEM = 3,
  NANOS_INVALID_PARAM = 4,
  NANOS_INVALID_REQUEST = 5
};
typedef enum mcc_enum_anon_5 nanos_err_t;
typedef unsigned int nanos_copy_id_t;
extern nanos_err_t nanos_get_addr(nanos_copy_id_t copy_id, void **addr, nanos_wd_t cwd);
extern void nanos_handle_error(nanos_err_t err);
static void nanos_xlate_fun_mmtestc_0(struct nanos_args_0_t *const arg, void *wd)
{
}
struct  mcc_struct_anon_19
{
  void (*outline)(void *);
  unsigned int type;
};
typedef struct mcc_struct_anon_19 nanos_max_args_t;
static void max_ol_gemm_1(struct nanos_args_0_t *const args);
struct  mcc_struct_anon_11
{
  _Bool mandatory_creation:1;
  _Bool tied:1;
  _Bool clear_chunk:1;
  _Bool reserved0:1;
  _Bool reserved1:1;
  _Bool reserved2:1;
  _Bool reserved3:1;
  _Bool reserved4:1;
};
typedef struct mcc_struct_anon_11 nanos_wd_props_t;
typedef unsigned long int size_t;
struct  nanos_const_wd_definition_tag
{
  nanos_wd_props_t props;
  size_t data_alignment;
  size_t num_copies;
  size_t num_devices;
  size_t num_dimensions;
  const char *description;
};
typedef struct nanos_const_wd_definition_tag nanos_const_wd_definition_t;
struct  mcc_struct_anon_14
{
  void *(*factory)(void *);
  void *arg;
};
typedef struct mcc_struct_anon_14 nanos_device_t;
struct  nanos_const_wd_definition_1
{
  nanos_const_wd_definition_t base;
  nanos_device_t devices[1L];
};
extern void *nanos_max_factory(void *args);
struct  mcc_struct_anon_12
{
  _Bool is_final:1;
  _Bool is_recover:1;
  _Bool is_implicit:1;
  _Bool reserved3:1;
  _Bool reserved4:1;
  _Bool reserved5:1;
  _Bool reserved6:1;
  _Bool reserved7:1;
};
typedef struct mcc_struct_anon_12 nanos_wd_dyn_flags_t;
typedef void *nanos_thread_t;
struct  mcc_struct_anon_13
{
  nanos_wd_dyn_flags_t flags;
  nanos_thread_t tie_to;
  int priority;
  void *callback;
  void *arguments;
};
typedef struct mcc_struct_anon_13 nanos_wd_dyn_props_t;
struct mcc_struct_anon_4;
typedef struct mcc_struct_anon_4 nanos_copy_data_internal_t;
typedef nanos_copy_data_internal_t nanos_copy_data_t;
struct mcc_struct_anon_0;
typedef struct mcc_struct_anon_0 nanos_region_dimension_internal_t;
typedef void *nanos_wg_t;
extern nanos_err_t nanos_create_wd_compact(nanos_wd_t *wd, nanos_const_wd_definition_t *const_data, nanos_wd_dyn_props_t *dyn_props, size_t data_size, void **data, nanos_wg_t wg, nanos_copy_data_t **copies, nanos_region_dimension_internal_t **dimensions);
extern nanos_wd_t nanos_current_wd(void);
struct  mcc_struct_anon_0
{
  size_t size;
  size_t lower_bound;
  size_t accessed_length;
};
typedef nanos_region_dimension_internal_t nanos_region_dimension_t;
struct  mcc_struct_anon_1
{
  _Bool input:1;
  _Bool output:1;
  _Bool can_rename:1;
  _Bool concurrent:1;
  _Bool commutative:1;
};
typedef struct mcc_struct_anon_1 nanos_access_type_internal_t;
typedef long int ptrdiff_t;
struct  mcc_struct_anon_2
{
  void *address;
  nanos_access_type_internal_t flags;
  short int dimension_count;
  const nanos_region_dimension_internal_t *dimensions;
  ptrdiff_t offset;
};
typedef struct mcc_struct_anon_2 nanos_data_access_internal_t;
typedef nanos_data_access_internal_t nanos_data_access_t;
enum mcc_enum_anon_0
{
  NANOS_PRIVATE = 0,
  NANOS_SHARED = 1
};
typedef enum mcc_enum_anon_0 nanos_sharing_t;
struct  mcc_struct_anon_5
{
  _Bool input:1;
  _Bool output:1;
};
typedef unsigned long int uint64_t;
typedef unsigned int reg_t;
struct  mcc_struct_anon_4
{
  void *address;
  nanos_sharing_t sharing;
  struct mcc_struct_anon_5 flags;
  short int dimension_count;
  const nanos_region_dimension_internal_t *dimensions;
  ptrdiff_t offset;
  uint64_t host_base_address;
  reg_t host_region_id;
  _Bool remote_host;
  void *deducted_cd;
};



 NANOS_API_DECL( void *, nanos_max_factory, ( void *args ) );
 
 NANOS_API_DECL( void, nanos_max_register_dfe, ( void *initFun, const char *name, unsigned int type) );
 
 NANOS_API_DECL( void, nanos_max_queue_input, ( const char* name, void *addr, size_t size ) );
 
 NANOS_API_DECL( void, nanos_max_queue_output, ( const char* name, void *addr, size_t size ) );
 

extern void gemm_init();









typedef void (*nanos_translate_args_t)(void *, nanos_wd_t);
extern nanos_err_t nanos_set_translate_function(nanos_wd_t wd, nanos_translate_args_t translate_args);
typedef void *nanos_team_t;
extern nanos_err_t nanos_submit(nanos_wd_t wd, size_t num_data_accesses, nanos_data_access_t *data_accesses, nanos_team_t team);
extern nanos_err_t nanos_create_wd_and_run_compact(nanos_const_wd_definition_t *const_data, nanos_wd_dyn_props_t *dyn_props, size_t data_size, void *data, size_t num_data_accesses, nanos_data_access_t *data_accesses, nanos_copy_data_t *copies, nanos_region_dimension_internal_t *dimensions, nanos_translate_args_t translate_args);
extern nanos_err_t nanos_wg_wait_completion(nanos_wg_t wg, _Bool avoid_flush);


void taskWait()
{
    nanos_err_t nanos_err;
    nanos_wd_t nanos_wd_ = nanos_current_wd();
    nanos_err = nanos_wg_wait_completion(nanos_wd_, 0);
    if (nanos_err != NANOS_OK)
    {
        nanos_handle_error(nanos_err);
    }
}



void mm_wrapper(double *a, double *b, double *c)
{
  {
    double *mcc_arg_0 = a;
    double *mcc_arg_1 = b;
    double *mcc_arg_2 = c;
    {
      static nanos_max_args_t max_ol_gemm_1_args;
      nanos_wd_dyn_props_t nanos_wd_dyn_props;
      struct nanos_args_0_t *ol_args;
      nanos_err_t nanos_err;
      struct nanos_args_0_t imm_args;
      nanos_region_dimension_t dimensions_0[1L];
      nanos_data_access_t dependences[3L];
      nanos_region_dimension_t dimensions_1[1L];
      nanos_region_dimension_t dimensions_2[1L];
       /* device argument type */ 
      max_ol_gemm_1_args.outline = (void (*)(void *))(void (*)(struct nanos_args_0_t *))&max_ol_gemm_1;
      max_ol_gemm_1_args.type = 1495144348;
      static struct nanos_const_wd_definition_1 nanos_wd_const_data = {.base = {.props = {.mandatory_creation = 0, .tied = 0, .clear_chunk = 0, .reserved0 = 0, .reserved1 = 0, .reserved2 = 0, .reserved3 = 0, .reserved4 = 0}, .data_alignment = __alignof__(struct nanos_args_0_t), .num_copies = 3, .num_devices = 1, .num_dimensions = 3, .description = 0}, .devices = {[0] = {.factory = &nanos_max_factory, .arg = &max_ol_gemm_1_args}}};
      nanos_wd_dyn_props.tie_to = 0;
      nanos_wd_dyn_props.priority = 0;
      nanos_wd_dyn_props.flags.is_final = 0;
      nanos_wd_dyn_props.flags.is_implicit = 0;
      nanos_wd_dyn_props.flags.is_recover = 0;
      ol_args = (struct nanos_args_0_t *)0;
      nanos_wd_t nanos_wd_ = (nanos_wd_t)0;
      nanos_copy_data_t *ol_copy_data = (nanos_copy_data_t *)0;
      nanos_region_dimension_internal_t *ol_copy_dimensions = (nanos_region_dimension_internal_t *)0;
      nanos_err = nanos_create_wd_compact(&nanos_wd_, &nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_0_t), (void **)&ol_args, nanos_current_wd(), &ol_copy_data, &ol_copy_dimensions);
      if (nanos_err != NANOS_OK)
        {
          nanos_handle_error(nanos_err);
        }
      dimensions_0[0].size = 256L * sizeof(double);
      dimensions_0[0].lower_bound = (0L - 0L) * sizeof(double);
      dimensions_0[0].accessed_length = (255L - 0L - (0L - 0L) + 1) * sizeof(double);
      dependences[0].address = (void *)mcc_arg_0;
      dependences[0].offset = 0L;
      dependences[0].dimensions = dimensions_0;
      dependences[0].flags.input = 1;
      dependences[0].flags.output = 0;
      dependences[0].flags.can_rename = 0;
      dependences[0].flags.concurrent = 0;
      dependences[0].flags.commutative = 0;
      dependences[0].dimension_count = 1;
      dimensions_1[0].size = 256L * sizeof(double);
      dimensions_1[0].lower_bound = (0L - 0L) * sizeof(double);
      dimensions_1[0].accessed_length = (255L - 0L - (0L - 0L) + 1) * sizeof(double);
      dependences[1].address = (void *)mcc_arg_1;
      dependences[1].offset = 0L;
      dependences[1].dimensions = dimensions_1;
      dependences[1].flags.input = 1;
      dependences[1].flags.output = 0;
      dependences[1].flags.can_rename = 0;
      dependences[1].flags.concurrent = 0;
      dependences[1].flags.commutative = 0;
      dependences[1].dimension_count = 1;
      dimensions_2[0].size = 256L * sizeof(double);
      dimensions_2[0].lower_bound = (0L - 0L) * sizeof(double);
      dimensions_2[0].accessed_length = (255L - 0L - (0L - 0L) + 1) * sizeof(double);
      dependences[2].address = (void *)mcc_arg_2;
      dependences[2].offset = 0L;
      dependences[2].dimensions = dimensions_2;
      dependences[2].flags.input = 0;
      dependences[2].flags.output = 1;
      dependences[2].flags.can_rename = 0;
      dependences[2].flags.concurrent = 0;
      dependences[2].flags.commutative = 0;
      dependences[2].dimension_count = 1;
      if (nanos_wd_ != (nanos_wd_t)0)
        {
          (*ol_args).a = mcc_arg_0;
          (*ol_args).b = mcc_arg_1;
          (*ol_args).c = mcc_arg_2;
          ol_copy_dimensions[0 + 0].size = 256L * sizeof(double);
          ol_copy_dimensions[0 + 0].lower_bound = (0L - 0L) * sizeof(double);
          ol_copy_dimensions[0 + 0].accessed_length = (255L - 0L - (0L - 0L) + 1) * sizeof(double);
          ol_copy_data[0].sharing = NANOS_SHARED;
          ol_copy_data[0].address = (void *)mcc_arg_0;
          ol_copy_data[0].flags.input = 1;
          ol_copy_data[0].flags.output = 0;
          ol_copy_data[0].dimension_count = (short int)1;
          ol_copy_data[0].dimensions = &ol_copy_dimensions[0];
          ol_copy_data[0].offset = 0L;
          ol_copy_dimensions[1 + 0].size = 256L * sizeof(double);
          ol_copy_dimensions[1 + 0].lower_bound = (0L - 0L) * sizeof(double);
          ol_copy_dimensions[1 + 0].accessed_length = (255L - 0L - (0L - 0L) + 1) * sizeof(double);
          ol_copy_data[1].sharing = NANOS_SHARED;
          ol_copy_data[1].address = (void *)mcc_arg_1;
          ol_copy_data[1].flags.input = 1;
          ol_copy_data[1].flags.output = 0;
          ol_copy_data[1].dimension_count = (short int)1;
          ol_copy_data[1].dimensions = &ol_copy_dimensions[1];
          ol_copy_data[1].offset = 0L;
          ol_copy_dimensions[2 + 0].size = 256L * sizeof(double);
          ol_copy_dimensions[2 + 0].lower_bound = (0L - 0L) * sizeof(double);
          ol_copy_dimensions[2 + 0].accessed_length = (255L - 0L - (0L - 0L) + 1) * sizeof(double);
          ol_copy_data[2].sharing = NANOS_SHARED;
          ol_copy_data[2].address = (void *)mcc_arg_2;
          ol_copy_data[2].flags.input = 0;
          ol_copy_data[2].flags.output = 1;
          ol_copy_data[2].dimension_count = (short int)1;
          ol_copy_data[2].dimensions = &ol_copy_dimensions[2];
          ol_copy_data[2].offset = 0L;
          nanos_err = nanos_set_translate_function(nanos_wd_, (nanos_translate_args_t)nanos_xlate_fun_mmtestc_0);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
          nanos_err = nanos_submit(nanos_wd_, 3, &dependences[0], (nanos_team_t)0);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
        }
      else
        {
          nanos_region_dimension_internal_t imm_copy_dimensions[3L];
          nanos_copy_data_t imm_copy_data[3L];
          imm_args.a = mcc_arg_0;
          imm_args.b = mcc_arg_1;
          imm_args.c = mcc_arg_2;
          imm_copy_dimensions[0 + 0].size = 256L * sizeof(double);
          imm_copy_dimensions[0 + 0].lower_bound = (0L - 0L) * sizeof(double);
          imm_copy_dimensions[0 + 0].accessed_length = (255L - 0L - (0L - 0L) + 1) * sizeof(double);
          imm_copy_data[0].sharing = NANOS_SHARED;
          imm_copy_data[0].address = (void *)mcc_arg_0;
          imm_copy_data[0].flags.input = 1;
          imm_copy_data[0].flags.output = 0;
          imm_copy_data[0].dimension_count = (short int)1;
          imm_copy_data[0].dimensions = &imm_copy_dimensions[0];
          imm_copy_data[0].offset = 0L;
          imm_copy_dimensions[1 + 0].size = 256L * sizeof(double);
          imm_copy_dimensions[1 + 0].lower_bound = (0L - 0L) * sizeof(double);
          imm_copy_dimensions[1 + 0].accessed_length = (255L - 0L - (0L - 0L) + 1) * sizeof(double);
          imm_copy_data[1].sharing = NANOS_SHARED;
          imm_copy_data[1].address = (void *)mcc_arg_1;
          imm_copy_data[1].flags.input = 1;
          imm_copy_data[1].flags.output = 0;
          imm_copy_data[1].dimension_count = (short int)1;
          imm_copy_data[1].dimensions = &imm_copy_dimensions[1];
          imm_copy_data[1].offset = 0L;
          imm_copy_dimensions[2 + 0].size = 256L * sizeof(double);
          imm_copy_dimensions[2 + 0].lower_bound = (0L - 0L) * sizeof(double);
          imm_copy_dimensions[2 + 0].accessed_length = (255L - 0L - (0L - 0L) + 1) * sizeof(double);
          imm_copy_data[2].sharing = NANOS_SHARED;
          imm_copy_data[2].address = (void *)mcc_arg_2;
          imm_copy_data[2].flags.input = 0;
          imm_copy_data[2].flags.output = 1;
          imm_copy_data[2].dimension_count = (short int)1;
          imm_copy_data[2].dimensions = &imm_copy_dimensions[2];
          imm_copy_data[2].offset = 0L;
          nanos_err = nanos_create_wd_and_run_compact(&nanos_wd_const_data.base, &nanos_wd_dyn_props, sizeof(struct nanos_args_0_t), &imm_args, 3, &dependences[0], imm_copy_data, imm_copy_dimensions, (nanos_translate_args_t)nanos_xlate_fun_mmtestc_0);
          if (nanos_err != NANOS_OK)
            {
              nanos_handle_error(nanos_err);
            }
        }
    }
  }
}

void usage(char *exec) {
    printf("Usage: %s <matrix size>\n", exec);
}


//double a[256L];
//double b[256L];
//double c[256L];
//double cpu_c[256L];

void setBlock(double *v, const double val) {
    for (int i=0; i<N*N; i++) {
        v[i] = val;
    }
}

void checkBlock (unsigned int * check_ok, double* v, const double val, const float threshold )
{
   const double maxv = val * (1.0 + (val < 0 ? -threshold : threshold));
   const double minv = val * (1.0 - (val < 0 ? -threshold : threshold));
   for (unsigned int i = 0; i < NN && ( *check_ok ); ++i) {
      double tmp = v[i];
      if (tmp > maxv || tmp < minv) {
         *check_ok = 0;
         fprintf(stderr, "ERROR:\t Expected a %lf but found %lf.\n", (double)val, (double)tmp);
      }
   }
}


void cpu_matmul(double *A, double *B, double *C) {
    for (size_t mm = 0; mm < N; ++mm) {
        for (size_t nn = 0; nn < N; ++nn) {
            for (size_t kk = 0; kk < N; ++kk) {
                C[mm*N+nn] += A[mm*N+kk] * B[kk*N+nn];
            }
        }
    }
}

#define VAL_A 587.0
#define VAL_B 437.0
#define VAL_C 0.0

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }
    const size_t msize = atoi(argv[1]);
    const size_t m2size = msize*msize;

    double *a, *b, *c, *cpu_c, *c_tmp;

    a = (double *)malloc(m2size*sizeof(double));
    b = (double *)malloc(m2size*sizeof(double));
    c = (double *)malloc(m2size*sizeof(double));
    c_tmp = (double *)malloc(msize*msize*msize*sizeof(double));
    cpu_c = (double *)malloc(m2size*sizeof(double));

    for (int i=0; i < m2size/NN; i++) {
        setBlock(&a[i*NN], VAL_A);
        setBlock(&b[i*NN], VAL_B);
        setBlock(&c[i*NN], VAL_C);
        setBlock(&cpu_c[i*NN], VAL_C);
        for (int j=0; j<msize/N; j++) {
            setBlock(&c_tmp[(i+j)*NN], VAL_C);
        }
    }

    double start, end;
    struct timespec t, t2;

    clock_gettime(CLOCK_MONOTONIC, &t);
    start = t.tv_sec + t.tv_nsec*1e-9;
    for (unsigned int i = 0; i < msize/N; i++) {
        for (unsigned int j = 0; j < msize/N; j++) {
            unsigned int const ci = j*NN + i*N*msize;
            for (unsigned int k = 0; k < msize/N; k++) {
                unsigned int const ai = k*NN + i*N*msize;
                unsigned int const bi = j*NN + k*N*msize;
                //matmulBlock(&a[ai], &b[bi], &c[ci]);
                mm_wrapper(&a[ai], &b[bi], &c_tmp[ci+k*NN]);
            }
        }
    }
    taskWait();
    clock_gettime(CLOCK_MONOTONIC, &t2);
    end = t2.tv_sec + t2.tv_nsec*1e-9;
    printf("%lf, %lf\n", start, end);
    printf("Mult in %lf s\n", end - start);

    clock_gettime(CLOCK_MONOTONIC, &t);
    start = t.tv_sec + t.tv_nsec*1e-9;
    //Reduce
    for (unsigned int i = 0; i < msize/N; i++) {
        for (unsigned int j = 0; j < msize/N; j++) {
            unsigned int const ci = j*NN + i*N*msize;
            for (unsigned int k = 0; k < msize/N; k++) {
                unsigned int const ai = k*NN + i*N*msize;
                unsigned int const bi = j*NN + k*N*msize;
                //mm_wrapper(&a[bi], &b[bi], &c[ci+k]);
                for (int x=0; x<N; x++) {
                    for(int y=0; y<N; y++) {
                        c[ci + x*N + y] += c_tmp[ci+k*NN + x*N + y];
                    }
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &t);
    end = t.tv_sec + t.tv_nsec*1e-9;
    printf("add in %lf s\n", end - start);


    int check_ok = 1;
    const float THRESHOLD = 1e-4;
    for (unsigned int i = 0; i < msize/N && check_ok; i++) {
        for (unsigned int j = 0; j < msize/N && check_ok; j++) {
            double val = VAL_C;
            for (unsigned int k = 0; k < msize/N; k++) {
                unsigned int const ai = k*NN + i*N*msize;
                unsigned int const bi = j*NN + k*N*msize;
                val += a[ai]*b[bi]*N;
            }
            unsigned int const ci = j*NN + i*N*msize;
            checkBlock(&check_ok, &c[ci], val, THRESHOLD);
        }
    }

    free(a);
    free(b);
    free(c);
    free(c_tmp);
    free(cpu_c);

    //cpu_matmul(a, b, cpu_c);

    //Check results
//    int errors = 0;
//    for (int i=0; i<N; i++) {
//        for (int j=0; j<N; j++) {
//            if ( fabs( c[i*N + j] - cpu_c[i*N + j] ) > 0.001 ) {
//                errors++;
//                printf("Error: c[%d][%d] = %lf (expected %lf)\n",
//                        i, j, c[i*N + j], cpu_c[i*N+j]);
//            }
//        }
//    }
}
void max_ol_gemm_1_unpacked(double *a, double *b, double *c)
{
    nanos_max_queue_input("A", a, 256*sizeof(double));
    nanos_max_queue_input("B", b, 256*sizeof(double));
    nanos_max_queue_output("C", c, 256*sizeof(double));
}
static void max_ol_gemm_1(struct nanos_args_0_t *const args)
{
  {
    max_ol_gemm_1_unpacked((*args).a, (*args).b, (*args).c);
  }
}
__attribute__((weak)) void nanos_needs_max_fun(void)
{
}

void __mcxx_max_register_gemm(void *p) {
    nanos_max_register_dfe( (void*)gemm_init, "TM", 1495144348);

}

typedef void nanos_init_func_t(void *);
struct  mcc_struct_anon_17
{
  nanos_init_func_t (*func);
  void *data;
};
typedef struct mcc_struct_anon_17 nanos_init_desc_t;
__attribute__((weak)) nanos_init_desc_t __nanos_init[1] = {[0] = {.func = __mcxx_max_register_gemm, .data = 0}};
__attribute__((weak)) nanos_init_desc_t *__nanos_init_begin = __nanos_init;
__attribute__((weak)) nanos_init_desc_t *__nanos_init_end = __nanos_init + sizeof(nanos_init_desc_t[1]) / sizeof(nanos_init_desc_t);
