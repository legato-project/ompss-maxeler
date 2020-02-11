! <testinfo>
! test_generator=(config/mercurium-ompss "config/mercurium-ompss-2 openmp-compatibility")
! </testinfo>
PROGRAM P
    IMPLICIT NONE
    INTEGER :: W(1:10)
    W = 41
    CALL AUX(W, 1, 10)
    !$OMP TASKWAIT

    IF (ANY(W /= 42)) STOP 1
END PROGRAM P

SUBROUTINE AUX(W, LOWER, UPPER)
    IMPLICIT NONE
    INTEGER :: LOWER, UPPER
    INTEGER :: W(LOWER : *)
    !$OMP TARGET DEVICE(smp) COPY_DEPS
    !$OMP TASK INOUT(W(LOWER : UPPER)) FIRSTPRIVATE(LOWER, UPPER)
    W(LOWER : UPPER) = W(LOWER : UPPER) + 1
    !$OMP END TASK
END SUBROUTINE AUX
