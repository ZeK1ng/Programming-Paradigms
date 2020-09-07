; fibonacci (that one doesn't spark joy)
; (define (fib n)
;     (if (< n 2) n
;         (+ (fib (- n 1)) (fib (- n 2)))))


; ; cooler fibonacci
; (define (cooler_fib_h n f0 f1)
;     (cond ((null? n) f0)
;         ((= n 1) f1)
;         (else (cooler_fib_h (- n 1) f1 (+ f0 f1)))))

; (define (cooler_fib n)
;     (cooler_fib_h n 0 1))


; (display (cooler_fib 50)) (newline)
; (display (fib 50)) (newline)


; (define (sum1 ls)
;     (apply + ls))

; (define (sum2 ls)
;     (if (null? ls) 0
;         (+ (car ls) (sum2 (cdr ls)))))

; (display (sum1 '(1 2 3 4 5))) (newline)
; (display (sum2 '(1 2 3 4 5))) (newline)


; ; russel westbrook
; (define (triple1 seq)
;     (if (null? seq) '()
;         (cons (* 3 (car seq)) 
;             (triple1 (cdr seq)))))

; ; my first lambda + map! yeey ^^
; (define (triple2 seq)
;     (map (lambda (n) (* 3 n)) seq))

; (display (triple2 '(1 2 3 4 5))) (newline)
; (display (triple1 '(1 2 3 4 5))) (newline)

; ; power
; (define (pow n p)
;     (if (= 0 p) 1
;         (* n (pow n (- p 1)))))

; (define (fast_pow n p)
;     (if (= 0 p) 1
;         (let ((half_pow (fast_pow n (quotient p 2))))
;             (if (even? p) (* half_pow half_pow)
;                 (* n half_pow half_pow)))))

; (display (fast_pow 2 7000)) (newline)
; ;(display (pow 2 7000)) (newline)


; ; flatten 
; ; proud of myself
; (define (flat seq)
;     (if (not (list? seq)) (list seq)
;         (apply append (map flat seq))))

; (display (flat '(a (b (c d (e) f (g h))) (i j)))) (newline)
; (display (flat '(a b c (((d (e) f (g h))) (i j))))) (newline)
; (display (flat '())) (newline)


; ; quick sort
; (define (split ls piv)
;     (if (= (length ls) 0) '(() ())
;         (let ((rest (split (cdr ls) piv)))
;             (if (> piv (car ls))
;                 (list (cons (car ls) (car rest)) (cadr rest))
;                 (list (car rest) (cons (car ls) (cadr rest)))))))
        

; (define (qsrt ls)
;     (if (= (length ls) 0) '()
;         (let ((parts (split (cdr ls) (car ls))))
;             (append (qsrt (car parts)) (list (car ls)) (qsrt (cadr parts))))))
   

; ; (display (split '(5 3 7 8 1 2 9) 4)) (newline)
; ; (display (split '(5 3 7 8 1 2 9) 10)) (newline)
; (display (qsrt '(5 3 7 8 1 2 9))) (newline)
; (display (qsrt '(5 2 5 8 1 2 2))) (newline)


; ; mergesort
; ; merge two sorted lists
; (define (merge ls1 ls2)
;     (cond ((null? ls1) ls2)
;         ((null? ls2) ls1)
;         ((if (< (car ls1) (car ls2))
;             (cons (car ls1) (merge (cdr ls1) ls2))
;             (cons (car ls2) (merge ls1 (cdr ls2)))))))

; (define (split_first_n ls n)
;     (if (= n 0) (list '() ls)
;         (let ((split_rest (split_first_n (cdr ls) (- n 1))))
;             (list (append (list (car ls)) (car split_rest))
;                 (cadr split_rest)))))       

; (define (msort ls)
;     (if (< (length ls) 2) ls
;         (let ((spl (split_first_n ls (quotient (length ls) 2))))
;             (merge (msort (car spl))
;                 (msort (cadr spl))))))

; ; (display (merge '(1 3 6 7 9) '(2 4 5 8))) (newline)    
; ; (display (split_first_n '(1 3 6 7 9 2 4 5 8) 4)) (newline)    
; ; (display (msort '(1 3 6 7 9 2 4 5 8))) (newline)    

; ; depth
; (define (depth ls)
;     (if (or (not (list? ls)) (null? ls)) 0
;     (+ 1 (apply max (map depth ls)))))

; (display (depth '("this" "list" "is" "shallow"))) (newline)
; (display (depth '(this (lst (has (substance)))))) (newline)
; (display (depth '(1 (2 3 (4)) (((6) 7 ((((8 9)) 10) (11)))) '(4)))) (newline)

; ; translate / handout 31
; (define (translate ls delta)
;     (map (lambda (n) (+ n delta)) ls))

; (display (translate '(1 2 3 4 5 6) 2/3)) (newline)


; ; power-set
; (define (expand seq n)
;     (list seq
;         (cons n seq)))

; (define (pow_set ls)
;     (if (null? ls) '(())
;             (apply append 
;                 (map (lambda (seq) (expand seq (car ls)))
;                     (pow_set (cdr ls))))))

; ; (display (pow_set '(1 2 3 4))) (newline)


; permute
; ; it ain't beautiful but at least it's honest work

; (define (list_without ls n)
;     (apply append 
;         (map (lambda (m) 
;                 (if (= m n) '()
;                     (list m))) ls)))

; (define (expand_every ls n)
;     (map (lambda (seq) 
;             (if (list? seq) (append (list n) seq)
;                 (append (list n) (list seq))))
;         ls))

; (define (permute ls)
;     (if (> 2 (length ls)) ls
;         (apply append 
;             (map (lambda (n) 
;                 (expand_every
;                     (permute (list_without ls n))
;                     n)) 
;                 ls))))

; ; (display (list_without '(1 2 3 4 3 6) 3)) (newline)
; ; (display (expand_every '((1) (2) (3)) 4)) (newline)
; ; (display (permute '(1 2 3 4))) (newline)

; ; generic map
; (define (gen_map fn ls . others)
;     (if (null? ls) '()
;         (cons (apply fn (cons (car ls) (map car others)))
;             (apply gen_map (cons fn (cons (cdr ls) (map cdr others)))))))


; (display (gen_map + '(2 3 4) '(3 4 5) '(4 5 6))) (newline)



; ; ; k-subsets
; (define (k_subsets set k)
;     (cond ((> k (length set)) '())
;         ((= 0 k) '(()))
;         ((= k (length set)) (list set))
;         ((let ((rest (cdr set)))
;                 (append (k_subsets rest k)
;                     (map (lambda (s) (cons (car set) s))
;                         (k_subsets rest (- k 1))))))))

; ; (display (k_subsets '(1 2 3 4) 0)) (newline)
; ; (display (k_subsets '(1 2 3 4) 1)) (newline)
; ; (display (k_subsets '(1 2 3 4) 3)) (newline)
; ; (display (k_subsets '(1 2 3 4) 4)) (newline)
; ; (display (k_subsets '(1 2 3 4) 5)) (newline)


; ; sum-set. old final exam
; (define (sum-filter ls s)  
;     (if (null? ls) '()
;         (let ((rest (sum-filter (cdr ls) s)))
;             (if (= s (apply + (car ls)))
;                 (cons (car ls) rest)
;                 rest))))

; (define (equals l1 l2)
;     (if (null? l1)
;         (if (null? l2) #t #f)
;         (and 
;             (= (car l1) (car l2))
;             (equals (cdr l1) (cdr l2)))))

; (define (remove ls e)
;     (if (null? ls) '()
;         (let ((rest (remove (cdr ls) e)))
;             (if (equals e (car ls))
;                 rest
;                 (cons (car ls) rest)))))

; (define (unique ls)
;     (if (null? ls) '()
;         (let ((lst (map qsrt ls)))
;             (cons (car lst)
;                 (unique (remove (cdr lst) (car lst)))))))

; (define (sum-set ls k s)
;     (sum-filter 
;         (unique (k_subsets ls k))
;         s))

; (display (sum-set '(1 1 2 2 1 1) 3 3)) (newline)

; ;; mid_2017
; (define (odd_div_help n i)
;     (if (> i n) 0
;         (let ((rest (odd_div_help n (+ i 2))))
;             (if (= 0 (remainder n i))
;                 (+ i rest)
;                 rest))))

; (define (odd_divisors n)
;     (odd_div_help n 1))

; (display (odd_divisors 24)) (newline)
; (display (odd_divisors 17)) (newline)
; (display (odd_divisors 15)) (newline)


;; uniqum 2018 midterm
; (define (contains ls e)
;     (if (null? ls) #f
;         (or (= e (car ls))
;             (contains (cdr ls) e))))

; (define (isUniqum? seq)
;     (if (< (length seq) 2) #t
;         (and
;             (isUniqum? (cdr seq))
;             (not (contains (cdr seq) (car seq))))))

; (define (prefix seq k)
;     (if (= 0 k) '()
;         (cons (car seq)
;             (prefix (cdr seq) (- k 1)))))

; (define (uniqum seq i)
;     (if (> i (length seq)) '(1 1)
;         (if (isUniqum? (prefix seq i)) (reverse (prefix seq i))
;             (uniqum (cdr seq) i))))

; (define (longestUniqum_helper seq i)
;     (if (= i 0) '()
;         (if (isUniqum? (uniqum (reverse seq) i))
;             (uniqum (reverse seq) i)
;             (longestUniqum_helper seq (- i 1)))))

; (define (longestUniqum seq)
;     (longestUniqum_helper seq (length seq)))

; (display (longestUniqum '(1 2 4 3 5 6 5 4 3)))(newline)
; (display (longestUniqum '(1 2 3 4 2 3 4 1 2)))(newline)


(define (mult_martices m n)
    (map (lambda (row) (mult_row_mtx row n)) m))

(define (mult_row_mtx row n)
    (apply gen_map (cons + (gen_map (lambda (k ls) (scale ls k)) row n))))

(define (gen_map fn left . others)
    (if (null? left) '()
            (cons (apply fn (cons 
                            (car left) (map car others)))
            (apply gen_map (cons fn (cons 
                                    (cdr left) (map cdr others)))))))

(define (scale ls k)
    (map (lambda (x) (* k x)) ls))

(display (mult_martices '((1 2 3) (1 1 1)) '((2 -1) (7 0) (5 3))))(newline)