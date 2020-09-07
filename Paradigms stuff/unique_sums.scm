;; IMPLEMENT
(define (power-set set) 
  (if (null? set) '(()) 
      (let ((power-set-of-rest (power-set (cdr set)))) 
        (append power-set-of-rest 
                (map (lambda (subset) (cons (car set) subset)) 
                     power-set-of-rest))))) 
					 
(define (Notcontains seq n)
	(if (null? seq) #t
		 (if (=  (car seq) n) #f
			(Notcontains (cdr seq) n)
		 )
	)
)	
(define (sum ls)
  (if (null? ls) 0
      (+ (car ls) (sum (cdr ls)))))
	  

(define (unique-sums-helper sums n seq)
	(if (null? seq) n
		(if (Notcontains sums (sum (car seq))) (unique-sums-helper (cons (sum (car seq)) sums) (+ n 1) (cdr seq))		
			(unique-sums-helper sums n (cdr seq))
		)
	)
)
(define (unique-sums seq)
	(unique-sums-helper `() 0 (power-set seq))
)
(display (unique-sums '(1 2)))(newline)	
(display (= 4 (unique-sums '(1 2))))(newline)
(display (= 7 (unique-sums '(1 2 3))))(newline)
(display (= 5 (unique-sums '(2 2 4))))(newline)
(display (= 56 (unique-sums '(1 2 3 4 5 6 7 8 9 10))))(newline)
(display (= 22 (unique-sums '(1 2 3 5 10))))(newline)
(display (= 21 (unique-sums '(1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1))))(newline)
