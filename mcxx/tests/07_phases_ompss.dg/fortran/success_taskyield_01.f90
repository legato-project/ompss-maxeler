! <testinfo>
! test_generator=(config/mercurium-ompss "config/mercurium-ompss-2 openmp-compatibility")
! test_compile_fail_nanos6_mercurium=yes
! test_compile_fail_nanos6_imfc=yes
! </testinfo>

PROGRAM P
    IMPLICIT NONE

    !$OMP TASK
        !$OMP TASKYIELD
    !$OMP END TASK

    !$OMP TASKWAIT
END PROGRAM P
