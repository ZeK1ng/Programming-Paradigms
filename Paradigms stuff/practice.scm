(define (notcontains seq elem)
    (if (null? seq) #t
        (if (equal? (car seq) elem) #f
            (contains (cdr seq) elem)
        )
    )
)
;(define (kailistia? lst sum)
 ;   (= sum (apply + lst ))

;)

;(define (kailisti seq sum )
 ;   (if (null? seq) `()
  ;      (if (kailistia? (car seq ) sum) (cons (car seq) (kailisti (cdr seq ) sum ))
   ;         (kailisti (cdr seq ) sum )
    ;))
;)

(define (kailisti  seq sum res)
    (if (null? seq) res
        
           (if  (and ( = sum (apply + (car seq)))
                (notcontains res (car seq)))
                    (cons (car seq) res)
            
            (kailisti (cdr seq) sum res))
        

    )
)

(display (kailisti `( (1 2 3 ) ( 2 3 4)(4 2 1 ) ) 9 `()) 
)(newline)