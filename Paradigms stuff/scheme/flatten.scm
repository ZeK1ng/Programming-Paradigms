(define (flatten sequence)
    (cond((null? sequence) `())
            ((list? (car sequence))
                (append (flatten (car sequence))
                        (flatten (cdr sequence))))
        (else (cons (car sequence) (flatten(cdr sequence))))
    
    )
)
(display (flatten `(1 2 3 (7 7( 7 (7 8 ) 9) 2 ) 4 5))) (newline)