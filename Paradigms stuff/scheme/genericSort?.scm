(define (sorted? seq comp)
    (or (< (length seq ) 2)
        (and (comp (car seq) (cadr seq))
            (sorted? (cdr seq) comp)
        )
    )   
)


(define (isGreat x y)
    (if(< x  y) #t 
        #f
    )
)
(display (sorted? `(1 2 3 4 5) isGreat))(newline)