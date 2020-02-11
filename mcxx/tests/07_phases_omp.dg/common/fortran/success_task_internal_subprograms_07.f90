! <testinfo>
! test_generator=config/mercurium-omp
! </testinfo>
MODULE M
    INTEGER :: FOO
END MODULE M

PROGRAM MAIN
    USE M
    IMPLICIT NONE

    CALL INIT

    !$OMP TASK
    PRINT *, "HOLA"
    !$OMP END TASK

    CONTAINS

        SUBROUTINE INIT
            IMPLICIT NONE
            FOO = 3
        END SUBROUTINE INIT
END PROGRAM MAIN
