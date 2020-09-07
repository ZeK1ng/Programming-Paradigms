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
(list (car num-list));; package pivot as a list
(quicksort (cadr split)))))) ;; recursively sort second hlaf

(display (quicksort `( 1860	795	5565	4433	219 7962	647	6452	4138	2697)))(newline)