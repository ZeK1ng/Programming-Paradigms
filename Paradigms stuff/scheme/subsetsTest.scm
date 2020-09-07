(define (k-subsets set k)
  (cond ((eq? (length set) k) (list set))
	((zero? k) '(()))
	((or (negative? k) (> k (length set))) '())
	(else (let ((k-subsets-of-rest (k-subsets (cdr set) k))
		    (k-1-subsets-of-rest (k-subsets (cdr set) (- k 1))))
		(append (map (lambda (subset) (cons (car set) subset)) k-1-subsets-of-rest)
			k-subsets-of-rest)))))

(display (k-subsets `((1 (0 0)) (1 (1 0)) (1 (1 1))  ) 2
))(newline)