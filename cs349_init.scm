;; Routines of use to CS349 assignments

;; Can't find filter, so here's a quick version:
(define (filter pred xs)
  (reverse (foldr (lambda (acc x) (if (pred x) (cons x acc) acc)) '() xs)))

(define PI 3.14159265358979323846)

(define (deg->rad d)
  (* PI (/ d 180.0)))

(define (rad->deg r)
  (* 180.0 (/ r PI)))

(define (matrix-mul . matrices)
  (foldr (lambda (acc m) (_matrix-mul acc m)) (affine-make-identity) matrices))

(define affine-mul matrix-mul)

;; Returns the angle between the x-axis and the vector represented by
;; the two points given
(define (get-angle p1 p2)
  (let* ((x1 (car p1))
         (x2 (car p2))
         (y1 (cadr p1))
         (y2 (cadr p2))
         (dx (- x2 x1))
         (dy (- y2 y1))
         (hyp (sqrt (+ (* dx dx) (* dy dy))))
         (vector-cos (/ dx hyp))
         (theta (acos vector-cos)))
    (if (< dy 0)
        (- (* 2 3.14159265358979323846) theta)
        theta)))

;; Functions to set the WatGUI logging level
(define logging-functions
  '((logging-set-level (int))))

(define (logging-set-to-todo) (logging-set-level 70))
(define (logging-set-to-paint) (logging-set-level 60))
(define (logging-set-to-debug) (logging-set-level 50))
(define (logging-set-to-info) (logging-set-level 40))
(define (logging-set-to-warn) (logging-set-level 30))

;; Functions corresponding to the XWindow class
(define xwindow-functions
  '((xwindow-get-depth-first-interactor-tree (window-id))
    (xwindow-get-breadth-first-interactor-tree (window-id))
    (xwindow-new (string rect))
    (xwindow-delete (window-id))))

(define (xwindow-get-depth-first-interactor-tree-names window-id)
  (map component-get-name (xwindow-get-depth-first-interactor window-id)))

(define (xwindow-get-breadth-first-interactor-tree-names window-id)
  (map component-get-name (xwindow-get-breadth-first-interactor window-id)))

;; Functions corresponding to the Graphics class
(define graphics-functions
  '((graphics-draw-line (window-id point point))
    (graphics-draw-line-segments (window-id (list (list point))))
    (graphics-draw-point (window-id point))
    (graphics-draw-points (window-id (list point)))
    (graphics-draw-polygon (window-id (list point)))
    (graphics-draw-rect (window-id rect))
    (graphics-draw-text (window-id point string))
    (graphics-fill-polygon (window-id (list point)))
    (graphics-fill-rect (window-id rect))
    (graphics-white-color ())
    (graphics-black-color ())
    (graphics-set-background-color (window-id int))
    (graphics-set-foreground-color (window-id int))
    (graphics-get-background-color (window-id))
    (graphics-get-foreground-color (window-id))
    (graphics-flush ())
    (graphics-get-clip-rect (window-id))
    (graphics-set-clip-rect (window-id rect))
    (graphics-get-transform (window-id))
    (graphics-set-transform (window-id matrix))
    (graphics-get-debug-delay (window-id))
    (graphics-set-debug-delay (window-id int))))

;; Functions corresponding to the Component class
(define component-functions
  '((component-get-name (component-id))
    (component-get-background-color (component-id))
    (component-get-bounds (component-id))
    (component-get-foreground-color (component-id))
    (component-get-parent (component-id))
    (component-get-parent-window (component-id))
    (component-is-point-in-component? (component-id point))
    (component-is-visible? (component-id))
    (component-repaint (component-id))
    (component-set-background-color (component-id int))
    (component-set-foreground-color (component-id int))
    (component-set-bounds (component-id rect))
    (component-set-debug-delay (component-id int))
    (component-set-location (component-id point))
    (component-set-size (component-id int int))
    (component-set-visible (component-id boolean))
    (component-add-component (component-id component-id))
    (component-remove-component (component-id component-id))))

;; Convenience functions for Component class
(define (component-get-size component-id)
  (let ((component-bounds (component-get-bounds component-id)))
    (list (caddr component-bounds) (cadddr component-bounds))))

(define (component-get-x component-id)
  (car (component-get-bounds component-id)))

(define (component-get-y component-id)
  (cadr (component-get-bounds component-id)))

(define (component-get-location component-id)
  (let ((bounds (component-get-bounds component-id)))
    (list (car bounds) (cadr bounds))))

(define (component-get-width component-id)
  (caddr (component-get-bounds component-id)))

(define (component-get-height component-id)
  (cadddr (component-get-bounds component-id)))

;; Functions corresponding to the SchemeComponent class
;; Functions starting with an underscore should be invoked without the
;; underscore
(define scheme-component-functions
  '((scheme-component-new (string))
    (_scheme-component-add-instruction (component-id string))
    (scheme-component-clear-instructions (component-id))
    (_scheme-component-get-instructions (component-id))
    (_scheme-component-set-instructions (component-id (list string)))
    (is-scheme-component? (component-id))))

(define key-event-handlers '()) ;; List of: (list component-id callback-fn)
(define mouse-event-handlers '()) ;; List of: (list component-id callback-fn)

(define (scheme-component-handle-event-callback component-id event-data callback-list)
  (let ((callbacks (map (lambda (x) (cadr x))
                        (filter (lambda (x) (equal? component-id (car x)))
                                callback-list))))
    (foldr (lambda (acc x) (or acc x)) #f (map (lambda (fn) (apply fn (cons component-id event-data))) callbacks))))

;; Called from C++
(define (scheme-component-handle-key-event component-id . event-data)
  (scheme-component-handle-event-callback component-id event-data key-event-handlers))

;; Called from C++
(define (scheme-component-handle-mouse-event component-id . event-data)
  (scheme-component-handle-event-callback component-id event-data mouse-event-handlers))

(define (scheme-component-add-key-event-callback component-id callback-fn)
  (set! key-event-handlers (cons (list component-id callback-fn) key-event-handlers)))

(define (scheme-component-add-mouse-event-callback component-id callback-fn)
  (set! mouse-event-handlers (cons (list component-id callback-fn) mouse-event-handlers)))

(define (scheme-component-remove-key-event-callback component-id callback-fn)
  (set! key-event-handlers (filter (lambda (x) (not (equal? x (list component-id callback-fn)))))))

(define (scheme-component-remove-mouse-event-callback component-id callback-fn)
  (set! mouse-event-handlers (filter (lambda (x) (not (equal? x (list component-id callback-fn)))))))

;; Functions corresponding to the TurtleGraphics class
;; Functions starting with an underscore should be invoked without the
;; underscore
(define turtle-graphics-functions
  '((turtle-component-new (string))
    (is-turtle-graphics? (component-id))
    (turtle-rotate (component-id real optional: (boolean #t)))
    (turtle-forward (component-id real boolean optional: (boolean #t)))
    (turtle-draw-text (component-id string optional: (boolean #t)))
    (turtle-draw-point (component-id optional: (boolean #t)))
    (turtle-clear-instructions (component-id))
    (_turtle-get-instructions (component-id))
    (turtle-get-num-steps (component-id))
    (turtle-get-orientation (component-id int))
    (turtle-get-position (component-id int))
    (turtle-get-affine-transform (component-id int))
    (turtle-draw-step (component-id int))))

(define (turtle-rotate-in-degrees component-id deg . args)
  (let ((redraw-now (if (null? args) #t (car args)))
        (theta (deg->rad deg)))
    (turtle-rotate component-id theta redraw-now)))

(define (turtle-draw-all component-id)
  (turtle-draw-step component-id (turtle-get-num-steps component-id)))

(define (turtle-get-instructions component-id)
  (map string->data (_turtle-get-instructions component-id)))

(define (turtle-eval-instruction component-id op-args)
  (eval (cons (car op-args) (cons component-id (cdr op-args)))))


;; Experimental. Example file contents:
;; (turtle-clear-instructions)
;; (turtle-forward 30 #t)
;; (turtle-rotate-in-degrees 45)
;; (turtle-forward 30 #t)
(define (turtle-eval-file component-id fname)
  (let ((ifile (open-input-file fname)))
    (define (eval-line line)
      (if (eof-object? line)
          (close-input-port ifile)
          (begin
            (turtle-eval-instruction component-id line)
            (eval-line (read ifile)))))
    (if ifile
        (eval-line (read ifile))
        #f)))


;; Functions to simulate Events
(define (simulate-key-press-event window-id key-char)
  (simulate-key-event window-id #t key-char))

(define (simulate-key-release-event window-id key-char)
  (simulate-key-event window-id #f key-char))

(define (simulate-mouse-down-event window-id where)
  (simulate-mouse-event window-id 0 where))

(define (simulate-mouse-up-event window-id where)
  (simulate-mouse-event window-id 1 where))

(define (simulate-mouse-move-event window-id where)
  (simulate-mouse-event window-id 2 where))

(define (simulate-mouse-drag-event window-id where)
  (simulate-mouse-event window-id 3 where))


;; The SchemeComponent allows you to add painting instructions
;; executed when its PaintComponent method is called. These are all of
;; the graphics operations supported by the SchemeComponent. They
;; correspond directly to the operations found in the Graphics object.
;; They are used as instruction codes by
;; scheme-component-execute-instruction (defined below), and can be
;; added via scheme-component-add-* (e.g.,
;; (scheme-component-add-draw-line point1 point2)
(define graphics-operations
  '(draw-line
    draw-line-segments
    draw-point           
    draw-points          
    draw-polygon         
    draw-rect            
    draw-text            
    fill-polygon         
    fill-rect            
    set-background-color 
    set-bg-color         
    set-foreground-color 
    set-fg-color         
    flush                
    set-clip-rect        
    set-transform
    log-message))

(define (graphics-log-message ignored-window-id s)
  (display (string-append s "\n")))


;; From here on down, these functions are mostly for internal use. You
;; can peek at them, but you don't need to worry about them.


;; Marshalling Functions
;; These functions assist with communication between Scheme and C

;; This function will take a function signature (like from the
;; graphics-functions list above) and define the corresponding Scheme
;; function, plus the code to communicate with the C code
(define (define-c-function function-sig)
  (let ((fname (head function-sig)))
    (eval `(define (,fname . args)
             (if (validate-function-call ',function-sig args)
                 (apply _scheme-call-c-function (cons ,(symbol->string fname) (expand-optional-args ',function-sig args)))
                 '()))
          (interaction-environment))))

;; This actually defines all of the local Scheme functions
(for-each (lambda (function-sigs)
            (for-each define-c-function function-sigs))
          (list logging-functions
                xwindow-functions
                graphics-functions
                component-functions
                scheme-component-functions
                turtle-graphics-functions))


;; Prior to calling the C code, this function validates the data being
;; sent is of the expected type
(define (validate-arg arg-type arg)
  (if (list? arg-type)
      (if (not (list? arg))
          #f
          (all? (lambda (x) (validate-arg (cadr arg-type) x #f)) arg))
      (case arg-type
        ((boolean) (or (boolean? arg) (number? arg)))
        ((int) (integer? arg))
        ((real) (real? arg))
        ((string) (string? arg))
        ((point) (point? arg))
        ((rect) (rect? arg))
        ((matrix) (matrix? arg))
        ((window-id) (integer? arg))
        ((component-id) (integer? arg))
        (else (begin
                (display (string-append "Error: Bad arg-type: " (symbol->string arg-type) "\n"))
                #f)))))

;; Prior to calling the C code, this function validates the data being
;; sent is of the expected type
(define (validate-function-call function-sig args)
  (define (validate-arguments arg-types args processing-optional-args)
    (if (null? arg-types) #t
        (let ((arg-type (if processing-optional-args
                            (head (head arg-types))
                            (head arg-types))))
          (cond ((equal? 'optional: arg-type)
                 (validate-arguments (tail arg-types) args #t))
                ((null? args) (if processing-optional-args
                                  #t
                                  (begin (display (string-append
                                                   "Error: Not enough arguments to function. Missing arguments: "
                                                   (data->string arg-types) "\n"))
                                         #f)))
                (else (and (validate-arg arg-type (head args))
                           (validate-arguments (tail arg-types) (tail args) processing-optional-args)))))))
  (let ((fname (head function-sig))
        (arg-types (cadr function-sig))
        (processing-optional-args #f))
    (if (validate-arguments arg-types args processing-optional-args)
        #t
        (begin
          (display (string-append
                    "Error calling function " (symbol->string fname) ". Expecting: "
                    (data->string arg-types) "\n"
                    "Got: " (data->string args) "\n"))
          #f))))

;; The local function definitions can include optional arguments and
;; default values. This function expands the arguments to their
;; default values.
(define (expand-optional-args function-sig original-args)
  (define (expand-args arg-defs args args-acc processing-optional-args)
    (if (null? arg-defs)
        (reverse args-acc)
        (if (equal? 'optional: (head arg-defs))
            (expand-args (tail arg-defs) args args-acc #t)
            (let ((arg-val (if (null? args)
                               (cadar arg-defs)
                               (head args)))
                  (new-args-list (if (null? args) '() (tail args))))
              (expand-args (tail arg-defs) new-args-list (cons arg-val args-acc) processing-optional-args)))))
  (if (validate-function-call function-sig original-args)
      (let ((arg-defs (cadr function-sig)))
        (expand-args arg-defs original-args '() #f))
      '()))

;; Validation functions
(define (all? pred xs)
  (foldr (lambda (acc x) (and acc (pred x))) #t xs))

(define (number-list? expected-length xs)
  (and (list? xs)
       (= expected-length (length xs))
       (all? number? xs)))

(define (point? p)
  (number-list? 2 p))

(define (rect? r)
  (number-list? 4 r))

(define (matrix? m)
  (and (list? m)
       (all? (lambda (xs) (number-list? 3 xs)) m)))



(define (data->string v)
  (let ((prev-outport (current-output-port))
        (output-port (open-output-string))
        (output-str ""))
    (set-output-port output-port)
    (write v output-port)
    (set! output-str (get-output-string output-port))
    (close-output-port output-port)
    (if (output-port? prev-outport)
        (set-output-port prev-outport))
    output-str))

(define (string->data s)
  (let* ((prev-inport (current-input-port))
         (input-port (open-input-string s))
         (dummy (set-input-port input-port))
         (data (read input-port)))
    (close-input-port input-port)
    (if (input-port? prev-inport)
        (set-input-port prev-inport))
    data))

;; ***This function is not intended to be called interactively.***
;; This function takes a window ID and a list whose first element is
;; the instruction code (as defined above in graphics-operations), and
;; the rest are arguments for that particular function. The arguments
;; are then transformed as listed below.
;; Consider the following call:
;; (scheme-component-execute-instruction 1 "(draw-point (10 10))")
;; - The operation, 'draw-point, is taken from the op-args list and is
;; is concatenated with the prefix "graphics-" to turn it into the
;; symbol 'graphics-draw-point. This gives us the function to call
;; - The window ID (the first argument to the function) is cons'ed
;; with the arguments (10 10)
;; - The function then is called with those arguments. In this case,
;; it would be:
;; (eval (apply 'graphics-draw-point (list 1 (10 10))))
;; which is evaluated by the interpreter as:
;; (graphics-draw-point 1 (10 10))
;; This function is intended to be called by the C++ code with the
;; serialized operation and arguments (which are serialized as a string)

;; Maybe suppress interpreter output when doing these operations...
;; Or set output port to /dev/null or stg
;; If do it all on this side, can do a for-each and get a single output
(define (scheme-component-execute-instruction window-id op-args-list-string)
  (let* ((op-args (string->data op-args-list-string))
         (op (head op-args))
         (fn-symbol (string->symbol (string-append "graphics-" (symbol->string op))))
         (args (cons window-id (tail op-args))))
    (eval `(apply ,fn-symbol args))))

;; Adds the given instruction (op-symbol) and its arguments (args) to
;; the specified Scheme component (component-id)
(define (scheme-component-add-instruction component-id op-symbol . args)
  (_scheme-component-add-instruction component-id (data->string (cons op-symbol args))))

;; This function creates a set of convenience functions for adding
;; instructions (and their arguments) to a Scheme component. For
;; example, given the argument 'flush, it will create a function
;; scheme-component-add-flush, whose first argument is the component
;; ID, and its subsequent arguments are used for the command given
(define (define-scheme-component-instruction op-symbol)
  (eval `(define (,(string->symbol (string-append "scheme-component-add-" (symbol->string op-symbol)))
                  component-id . args)
           (apply scheme-component-add-instruction (cons component-id (cons ',op-symbol args))))
        (interaction-environment)))

;; Creates all of the convenience functions in the top level
(for-each define-scheme-component-instruction graphics-operations)

(define (scheme-component-get-instructions component-id)
  (map string->data (_scheme-component-get-instructions component-id)))

(define (scheme-component-set-instructions component-id xs)
  (_scheme-component-set-instructions component-id (map data->string xs)))

(define component-functions
  '(component-get-name
    component-get-background-color
    component-get-bounds
    component-get-foreground-color
    component-get-parent
    component-get-parent-window
    component-is-point-in-component?
    component-is-visible?
    component-repaint
    component-set-background-color
    component-set-foreground-color
    component-set-bounds
    component-set-location
    component-set-size
    component-set-visible
    component-add-component
    component-remove-component))

(define (define-scheme-component-fn component-fn)
  (eval `(define ,(string->symbol (string-append "scheme-" (symbol->string component-fn))) ,component-fn)
        (interaction-environment)))

;; Creates all of the convenience functions in the top level
(for-each define-scheme-component-fn component-functions)

;; Load user file
(load "student-scripts.scm")
