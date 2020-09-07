(define (partition pivot num-list)
(if (null? num-list) '(() ())
(let ((split-of-rest (partition pivot (cdr num-list))))
(if (< (car num-list) pivot)
(list (cons (car num-list) (car split-of-rest))
(cadr split-of-rest))
(list (car split-of-rest) (cons (car num-list)
(car (cdr split-of-rest))))))))

(define (quicksort num-list)
(if (<= (length num-list) 1) num-list
(let ((split (partition (car num-list) (cdr num-list))))
(append (quicksort (car split)) ;; recursively sort first half
(list (car num-list))
;; package pivot as a list
(quicksort (cadr split)))))) ;; recursively sort second half

(define (k-subsets set k)
(cond ((eq? (length set) k) (list set))
((zero? k) '(()))
((or (negative? k) (> k (length set))) '())
(else (let ((k-subsets-of-rest (k-subsets (cdr set) k))
(k-1-subsets-of-rest (k-subsets (cdr set) (- k 1))))
(append (map (lambda (subset)
(cons (car set) subset))
k-1-subsets-of-rest)
k-subsets-of-rest)))))


(define (notcontains seq elem)
    (if (null? seq) #t
        (if (equal? (car seq) elem) #f
            (notcontains (cdr seq) elem)
        )
    )
)

(define (kailisti  seq sum res)
    (if (null? seq) res
        
           (if  (and ( = sum (apply + (car seq)))
                (notcontains res (car seq)))
                   (kailisti (cdr seq) sum (cons (car seq) res)) 
            
            (kailisti (cdr seq) sum res))
    )
)


(define (sumSet seq leng sum)
    (reverse(kailisti (k-subsets (quicksort seq) leng) sum `()))  
)

(display (sumSet `(1 2 3 4 4 6 7 2 2 2 1 1 1  4 3 2) 3 9))(newline)




