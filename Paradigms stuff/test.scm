(define (k_prefix ls k res)
    (if (= k 0) res
       (reverse (k_prefix (cdr ls )(- k 1) (cons  (car ls) res) ))
    )
)
(display (k_prefix `(1 2 3 4 5 6) 5 `()))(newline)