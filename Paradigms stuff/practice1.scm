(define (remove seq k)
    (if (= k 0)
        seq
        (remove (cdr seq) (- k 1)))
)
;(display ( remove `(1 2 3 4 5 6 7 ) 4))(newline)

(define (contains seq elem)
    (if (null? seq ) #f
        (if (equal? (car seq) elem)  #t
            (contains (cdr seq) elem)
        )
    )
)
(display (contains `(1 2 3 4 5) 2))(newline)


