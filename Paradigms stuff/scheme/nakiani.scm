(define (nakiani n)
    (cond ((zero? (remainder n 400 )) "nakiani")
        ((and (zero? (remainder n 4)) (not(zero?(remainder n 100))) ) "nakiani")
        (else "araNaikani")
    )
)

(display (nakiani 2019))(newline)