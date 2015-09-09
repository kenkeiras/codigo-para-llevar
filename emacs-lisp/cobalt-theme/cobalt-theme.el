(require 'color-theme)

(defun cobalt-theme ()
  "Blue color theme. Based on the gtksourceview one."
  (interactive)
  (color-theme-install
   '(cobalt-theme
     ;; Base configuration
     ((foreground-color . "#d0d0d0")
      (background-color . "#001b33")
      (border-color . "#000d1a")
      (cursor-color . "#0088ff")
      (background-mode . blue))

     ;; General configuration
     (default ((:t (:foreground "#d0d0d0" :background "#001b33"))))
     (region ((t (:background "#0044bb"))))
     (highlight ((t (:background "#0044bb"))))
     (fringe ((t (:background "#000d1a"))))
     (show-paren-match-face ((t (:background "#0000FF"))))
     (isearch ((t (:bold t :foreground "#ffffff" :background "#00BF8C"))))
     (mode-line ((t (:bold t :foreground "#b9b9b9" :background "#002244"))))
     (mode-line-inactive ((t (:foreground "#708090" :background "#002244"))))
     (modeline ((t (:bold t :foreground "#b9b9b9" :background "#002244"))))
     (modeline-inactive ((t (:foreground "#708090" :background "#002244"))))
     (modeline-buffer-id ((t (:bold t :foreground "#d9d9d9" :background "#002244"))))
     (minibuffer-prompt ((t (:bold t :foreground "#0088ff"))))
     (hl-line ((t (:background "#001166"))))
     (gui-element ((t (:background "#FFFFFF" :foreground "#96CBFE"))))
     (buffers-tab ((t (:background "#000d1a" :foreground "#d9d9d9"))))
     (vhl/default-face ((t (:background "#000d1a" :foreground "#0066ee"))))
     (border-glyph ((t (nil))))

     ;; Flash on evaluate an expression
     (eval-sexp-fu-flash ((t (:background "#000d1a" :foreground "#0066ee"))))
     (nrepl-eval-sexp-fu-flash ((t (:background "#000d1a" :foreground "#0066ee"))))

     ;; Syntax highlight
     (font-lock-builtin-face ((t (:foreground "#ff9d00"))))
     (font-lock-comment-face ((t (:foreground "#0088ff"))))
     (font-lock-constant-face ((t (:bold t :foreground "#ffff00"))))
     (font-lock-doc-face ((t (:bold t :foreground "#0088ff"))))
     (font-lock-function-name-face ((t (:foreground "#cccccc"))))
     (font-lock-keyword-face ((t (:bold t :foreground "#ff9d00"))))
     (font-lock-preprocessor-face ((t (:foreground "#cccccc"))))
     (font-lock-reference-face ((t (:bold t :foreground "#3ad900"))))
     (font-lock-string-face ((t (:foreground "#3ad900"))))
     (font-lock-type-face ((t (:foreground "#ff6020"))))
     (font-lock-variable-name-face ((t nil)))
     (font-lock-warning-face ((t (:foreground "#ff0000"))))

     ;; Undo tree mode
     (undo-tree-visualizer-active-branch-face ((t (:foreground "#0088ff" :background "#001b33"))))

     ;; Window number mode
     (window-number-face ((t (:inherit))))

     ;; Minibuffer autocompletion
     (ido-first-match ((t (:foreground "#00BBff" :background "#000d1a"))))
     (ido-only-match ((t (:foreground "#00BBff" :background "#000d1a"))))
     (ido-subdir ((t (:background "#000d1a"))))
     (ido-indicator ((t (:foreground "#001b33" :background "#00BBff"))))

     ;; Completion pop-up
     (ac-yasnippet-candidate-face ((t (:background "#000d1a" :foreground "#0044bb"))))
     (ac-yasnippet-selection-face ((t (:background "#000d1a" :foreground "#0088ff"))))

     (ac-completion-face ((t (:foreground "#0088ff" :underline t))))
     (ac-candidate-face ((t (:background "#000d1a" :foreground "#0044bb"))))
     (ac-selection-face ((t (:background "#000d1a" :foreground "#0088ff"))))
     (popup-tip-face ((t (:background "#000d1a" :foreground "#0088ff"))))
     (popup-scroll-bar-foreground-face ((t (:background "#0044bb"))))
     (popup-scroll-bar-background-face ((t (:background "#0088ff"))))
     (popup-isearch-match ((t (:background "#000d1a" :foreground "#0088ff"))))

     ;; Rainbow mode
     (rainbow-delimiters-depth-1-face ((t (:foreground "dark red"))))
     (rainbow-delimiters-depth-2-face ((t (:foreground "dark green"))))
     (rainbow-delimiters-depth-3-face ((t (:foreground "deep pink"))))
     (rainbow-delimiters-depth-4-face ((t (:foreground "yellow"))))
     (rainbow-delimiters-depth-5-face ((t (:foreground "green"))))
     (rainbow-delimiters-depth-6-face ((t (:foreground "light blue"))))
     (rainbow-delimiters-depth-7-face ((t (:foreground "orange"))))
     (rainbow-delimiters-depth-8-face ((t (:foreground "slate blue"))))
     (rainbow-delimiters-depth-9-face ((t (:foreground "light gray"))))
     (rainbow-delimiters-unmatched-face ((t (:foreground "white")))))))

