! <testinfo>
! test_generator=(config/mercurium-ompss "config/mercurium-ompss-2 openmp-compatibility")
! </testinfo>
MODULE MOD_TYPES
    IMPLICIT NONE

    TYPE, PUBLIC :: MY_TYPE
        INTEGER :: X
    END TYPE MY_TYPE
END MODULE MOD_TYPES

MODULE M
    IMPLICIT NONE
    INTERFACE
        !$OMP TASK
        SUBROUTINE FOO(VAR)
            USE MOD_TYPES
            IMPLICIT NONE
            TYPE(MY_TYPE), VALUE :: VAR
        END SUBROUTINE FOO
    END INTERFACE
END MODULE M

PROGRAM P
    USE M
    USE MOD_TYPES
    IMPLICIT NONE
    TYPE(MY_TYPE) :: VAR

    VAR % X = -1
    CALL FOO(VAR)

    !$OMP TASKWAIT

    IF (VAR % X /= -1) STOP 1
END PROGRAM P

SUBROUTINE FOO(VAR)
    USE MOD_TYPES
    IMPLICIT NONE
    TYPE(MY_TYPE), VALUE :: VAR
    VAR % X = 1
END SUBROUTINE FOO
