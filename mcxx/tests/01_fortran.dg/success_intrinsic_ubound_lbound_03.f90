! <testinfo>
! test_generator="config/mercurium-fortran run"
! </testinfo>
SUBROUTINE S(X, LN, UN, LM, UM)
    IMPLICIT NONE
    INTEGER :: LN, UN, LM, UM, A, B, C, D
    INTEGER :: X(LN:UN, LM:UM)

    A = LN
    B = UN
    C = LM
    D = UM

    PRINT *, LBOUND(X, 1), UBOUND(X, 1), LBOUND(X, 2), UBOUND(X, 2)

    IF (LBOUND(X, 1) /= A) STOP 11
    IF (UBOUND(X, 1) /= B) STOP 12
    IF (LBOUND(X, 2) /= C) STOP 13
    IF (UBOUND(X, 2) /= D) STOP 14

    IF (LBOUND(X, 1) /= LN) STOP 21
    IF (UBOUND(X, 1) /= UN) STOP 22
    IF (LBOUND(X, 2) /= LM) STOP 23
    IF (UBOUND(X, 2) /= UM) STOP 24
END SUBROUTINE S

PROGRAM MAIN
    IMPLICIT NONE

    INTEGER, ALLOCATABLE :: A(:, :)

    INTERFACE
        SUBROUTINE S(X, LN, UN, LM, UM)
            IMPLICIT NONE
            INTEGER :: LN, UN, LM, UM
            INTEGER :: X(LN, UN, LM, UM)
        END SUBROUTINE S
    END INTERFACE

    ALLOCATE (A(2:10, 3:20))

    PRINT *, LBOUND(A, 1), UBOUND(A, 1), LBOUND(A, 2), UBOUND(A, 2)
    CALL S(A, LBOUND(A, 1), UBOUND(A, 1), LBOUND(A, 2), UBOUND(A, 2))
END PROGRAM MAIN
