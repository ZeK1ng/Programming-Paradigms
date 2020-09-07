(define (fastFibHelper n base0 base1)
    (if (< n 1) base0
        (fastFibHelper (- n 1) base1 (+ base1 base0))
    )
)
(display (fastFibHelper 100 0 1))(newline)