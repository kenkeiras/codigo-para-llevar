;; Es el paquete ARC4.
(defpackage :arc4)

;; Macro que intercambia dos variables.
(defmacro swap(x y)
  "Intercambia los valores de X e Y"
  (let ((tmp (gensym)))   ; Genera un símbolo para guardar el valor temporal.
    `(let ((,tmp ,x))     ; Guarda el valor temporal de X.
       (setf ,x ,y)       ; Asigna a X el valor de Y.
       (setf ,y ,tmp))))  ; Asigna a Y el valor guardado en el símbolo temporal.


;; Genera un rango al estilo de python.
(defun range(top)
  "Genera una lista de (0 1 .. top - 1)"
  (let ((rng '() ))                        ; Inicializa una lista vacía.
    (loop for i from (- top 1) downto 0 do ; Va descendiendo desde el valor indicado.
     (push i rng))                     ; Y lo introduce de primero en la lista.
    rng))                                  ; Por último devolvemos la lista.


;; Key Scheduling Algorithm.
(defun ksa (key)
  "Inicializa el array de estado"
  (let ((S (range 256))            ; Se inicializa S como una lista de 0 a 255 .
    (j 0)                      ; j se inicializa a 0 .
    (key-length (length key))) ; La longitud de la clave.

(loop for i from 0 to 255 do   ; De i desde 0 a 255 (ambos incluidos).
     (setf j (mod (+ j         ; j = j + S[i] + k[i % len(k)]
             (nth i S) 
             (char-code (char key (mod i key-length))))
              256))

     (swap (nth i S) (nth j S))) ; Intercambia S[i] y S[j]

S))  ; Devuelve S


;; Pseudo Random Generation Algorithm.
(defun cipher(S msg)
  "Cifra el mensaje `msg` a partir del estado S"
  (let ((i 0) (j 0) (code '())) ; Inicializa i y j a 0; y `code` como lista vacía.

(loop for c across msg do               ; Iterando sobre todas las letras del mensaje.
     (setf i (mod (+ i 1) 256))         ; i = (i + 1) % 256 .
     (setf j (mod (+ j (nth i S)) 256)) ; j = (j + S[i]) % 256 .
     (swap (nth i S) (nth j S))         ; Intercambia S[i] y S[j] .

 (let ((k (nth (mod (+ (nth i S)    ; k = S[(S[i] + S[j]) % 256] .
                   (nth j S)) 
                256)
               S)))

       (setf code (append code (list (logxor ; Añade a la lista `code` c XOR k .
                      (char-code c)
                      k))))))
    code)) ; Devuelve la lista `code`.


;; Envoltorio
(defun arc4(msg key)
  "Cifra con ARC4 el mensaje `msg` con la clave `key`"
  (let ((S (ksa key))) ; KSA
    (cipher S msg)))   ; PRNG


; Ejemplo
(format t "~a~%" (arc4 "pedia" "Wiki"))
