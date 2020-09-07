(define (flatten seq)
    (if (not (list? seq)) (list seq)
        (apply append (map flatten seq))
    )

)
(display (flatten `(1 2 3 (7 (7) (7 7 (8 9)) 2 ) 4 5)))(newline)
